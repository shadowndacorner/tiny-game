#ifdef MINGINE
#include "platform.hpp"
#include "../graphics_enums.hpp"
#include "opengl_util.hpp"
#include "opengl_texture.hpp"
#include "opengl_rendertexture.hpp"
#include "opengl_enum_conversion.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;

static RenderTextureHandle activeRTHandle;
static GLuint activeFBO;
void OpenGL_GraphicsInterface::SetActiveRenderTexture(RenderTextureHandle rt)
{
	if (rt.is_valid())
	{
		auto rtex = GFX_CAST<OpenGL_RenderTexture*>(rt.get_pointer());
		assert(rtex->GetFBOId() != 0);
		//rtex->Validate();
		if (activeFBO != rtex->GetFBOId())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, rtex->GetFBOId());
			CheckGLError();
			if (rtex->GetDrawbuffers().size() > 0)
			{
				glDrawBuffers(rtex->GetDrawbuffers().size(), rtex->GetDrawbuffers().data());
				CheckGLError();
			}

			activeFBO = rtex->GetFBOId();
			/*
			auto gfx = GFX_CAST<OpenGL_GraphicsInterface*>(graphics::GetInterface());

			math::screen_rect_t vp;
			vp.pos.x = 0;
			vp.pos.y = 0;
			vp.size.x = rtex->GetWidth();
			vp.size.y = rtex->GetHeight();

			gfx->SetViewport(vp);
			*/
			CheckGLError();
		}
	}
	else
	{
		activeFBO = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SetViewport(m_platform->get_screen_rect());
		CheckGLError();
	}
	activeRTHandle = rt;
}

RenderTextureHandle OpenGL_GraphicsInterface::GetActiveRenderTexture()
{
	if (activeRTHandle.is_valid())
		return activeRTHandle;
	return 0;
}

inline GLuint GetActiveOpenGLFBO()
{
	return activeFBO;
}

inline bool is_active_rt(OpenGL_RenderTexture& tex)
{
	return tex.GetFBOId() == GetActiveOpenGLFBO();
}

OpenGL_RenderTexture::OpenGL_RenderTexture() : m_fbo(0), m_depthrb(0) {}

OpenGL_RenderTexture::~OpenGL_RenderTexture()
{
	if (GetActiveOpenGLFBO() == m_fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Destroy();
}

void OpenGL_RenderTexture::Create()
{
	assert(m_fbo == 0);
	m_validated = false;
	glGenFramebuffers(1, &m_fbo);
	CheckGLError();
}

void OpenGL_RenderTexture::Create(int width, int height, int depth)
{
	Create();
	m_width = width;
	m_height = height;
	m_depth = depth;
}

void OpenGL_RenderTexture::Resize(int width, int height)
{
	if (m_width == width && m_height == height)
		return;

	m_width = width;
	m_height = height;
	int depth = m_depth;

	std::vector<TextureFormat> color_formats;
	std::vector<TextureFormat> depth_formats;
	for (auto& tex : m_color_textures)
	{
		color_formats.push_back(tex->get_internal_format());
	}

	for (auto& tex : m_depth_textures)
	{
		depth_formats.push_back(tex->get_internal_format());
	}

	bool createDepth = m_depthrb != 0;
	Destroy();
	Create(width, height, depth);
	
	auto old = activeFBO;
	activeFBO = m_fbo;
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		CheckGLError();
		if (GetDrawbuffers().size() > 0)
		{
			GLenum enums[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(GetDrawbuffers().size(), GetDrawbuffers().data());
			CheckGLError();
		}
	}

	for (auto it : color_formats)
	{
		CreateColorTexture(it);
	}

	if (createDepth)
	{
		CreateDepthTexture();
	}

	for (auto it : depth_formats)
	{
		// nothing?
	}
	activeFBO = old;
	
	// this is hacky
	auto handle = activeRTHandle;
	OpenGL_RenderTexture::SetActiveRenderTexture(NULL);
		
	if (handle.is_valid())
		OpenGL_RenderTexture::SetActiveRenderTexture(handle);
}

void OpenGL_RenderTexture::Resize(int width, int height, int depth)
{
	if (m_depth == depth)
		return;

	m_depth = depth;
	Resize(width, height);
}


inline enums::TextureFormat::Enum TextureFormatToRenderTextureInternal(enums::TextureFormat::Enum format)
{
	using namespace enums::TextureFormat;
	switch (format)
	{
	case Red:
	case R8:
	case R16:
	case R16F:
	case R16UI:
	case R32F:
		return Red;
		break;
	case RG16:
	case RG16UI:
		return RG;
		break;
	case RGB:
	case RGB8:
	case RGB16:
	case RGB16F:
	case RGB10_A2:
	case RGB10_A2UI:
	case RGB32F:
	case R11F_G11F_G10F:
		return RGB;
		break;
	case RGBA:
	case BGRA:
	case RGBA8:
	case RGBA16:
	case RGBA16F:
	case RGBA32F:
		return RGBA;
		break;
	case DEPTH16:
	case DEPTH24:
	case DEPTH32:
		return DEPTH;
		break;
	case Luminance:
	case LuminanceAlpha:
		return Luminance;
		break;
	}
	assert(false && "Attempted to use invalid enum");
	return Invalid;
}

int OpenGL_RenderTexture::CreateColorTexture(TextureFormat format)
{
	assert(is_active_rt(*this) && "Attempted to use inactive render texture");
	assert(m_width > 0 && m_height > 0);
	m_validated = false;

	auto gfx = graphics::GetInterface();
	auto handle = gfx->AllocateTexture();
	auto tex = GFX_CAST<OpenGL_Texture*>(handle.get_pointer());

	tex->create2d(m_width, m_height, TextureFilterMode::Bilinear, format, TextureWrap::Clamp);
	auto fmt = TextureFormatToRenderTextureInternal(format);
	tex->update(NULL, fmt);

	auto id = m_color_textures.size();
	auto attachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + id);

	m_drawbuffers.push_back(attachment);

	switch (tex->get_dimension())
	{
	case 1:
		GL_CHECK(glFramebufferTexture1D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_1D, tex->GetTextureID(), 0));
		break;
	case 2:
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->GetTextureID(), 0));
		break;
	case 3:
		// TODO: Write to other places
		GL_CHECK(glFramebufferTexture3D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_3D, tex->GetTextureID(), 0, 0));
		break;
	}
	m_color_textures.push_back(handle);
	return static_cast<int>(id);
}

int OpenGL_RenderTexture::CreateDepthTexture()
{
	assert(is_active_rt(*this) && "Attempted to use inactive render texture");
	assert(m_width > 0 && m_height > 0);
	assert(m_depth_textures.size() == 0);
	m_validated = false;

	auto gfx = graphics::GetInterface();

	// Create depth buffer
	GL_CHECK(glGenRenderbuffers(1, &m_depthrb));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_depthrb));
	GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	
	GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthrb));
	return static_cast<int>(0);
}

void OpenGL_RenderTexture::SetDrawTargets(const std::vector<uint32_t>& targetIndices)
{
	assert(is_active_rt(*this) && "Attempted to modify inactive render texture");
	if (GetDrawbuffers().size() >= targetIndices.size())
	{
		static thread_local std::vector<GLenum> targetBuffers;
		targetBuffers.clear();
		targetBuffers.reserve(targetIndices.size());
		for (auto it : targetIndices)
		{
			targetBuffers.push_back(m_drawbuffers[it]);
		}
		GL_CHECK(glDrawBuffers(targetBuffers.size(), targetBuffers.data()));
	}
	else
	{
		fprintf(stderr, "RenderTexture error: attempted to draw to more buffers than exist on the RenderTexture\n");
	}
	CheckGLError();
}

void OpenGL_RenderTexture::Destroy()
{
	if (m_fbo != 0)
	{
		if (activeFBO == m_fbo)
		{
			activeFBO = 0;
			activeRTHandle = RenderTextureHandle();
		}

		GL_CHECK(glDeleteFramebuffers(1, &m_fbo));
		m_fbo = 0;
	}

	if (m_depthrb != 0)
	{
		GL_CHECK(glDeleteRenderbuffers(1, &m_depthrb));
		m_depthrb = 0;
	}

	m_color_textures.clear();
	m_depth_textures.clear();
	m_drawbuffers.clear();
	m_validated = false;
	m_width = 0;
	m_height = 0;
}

TextureHandle OpenGL_RenderTexture::GetColorAttachment(int id)
{
	assert(id < m_color_textures.size());
	return m_color_textures[id];
}

TextureHandle OpenGL_RenderTexture::GetDepthAttachment(int id)
{
	assert(id < m_depth_textures.size());
	return m_depth_textures[id];
}

void * tiny::graphics::OpenGL_RenderTexture::GetDepthAttachment()
{
	return reinterpret_cast<void*>(size_t(m_depthrb));
}

void tiny::graphics::OpenGL_RenderTexture::SetDepthAttachment(void * data)
{
	size_t intermediate_depth = reinterpret_cast<size_t>(data);
	assert(intermediate_depth);

	GLuint rDepth = GLuint(intermediate_depth);
	m_depthrb = rDepth;
	
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_depthrb));
	GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthrb));
}

void OpenGL_RenderTexture::Validate()
{
	if (m_validated)
		return;

	assert(is_active_rt(*this) && "Attempted to use inactive render texture");
	auto status = GL_CHECK(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	switch (status)
	{
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cerr << "Framebuffer error: attachment point incomplete" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cerr << "Framebuffer error: no images attached" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cerr << "Framebuffer error: incomplete draw buffer" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cerr << "Framebuffer error: incomplete read buffer" << std::endl;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cerr << "Framebuffer error: current setup unsupported" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cerr << "Framebuffer error: inconsistent multisample settings for attachments or fixed smaple locations inconsistent" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		std::cerr << "Framebuffer error: incomplete layer targets" << std::endl;
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		m_validated = true;
		return;
	}
	throw std::runtime_error("Framebuffer incomplete");
}
#endif