#ifdef MINGINE
#include <tiny/graphics/opengl/glad/glad.h>
#include <tiny/graphics/opengl/opengl_enum_conversion.hpp>
#include <tiny/graphics/opengl/opengl_graphics_interface.hpp>

#include <tiny/graphics/opengl/opengl_index_buffer.hpp>
#include <tiny/graphics/opengl/opengl_rendertexture.hpp>
#include <tiny/graphics/opengl/opengl_shader.hpp>
#include <tiny/graphics/opengl/opengl_shader_program.hpp>
#include <tiny/graphics/opengl/opengl_texture.hpp>
#include <tiny/graphics/opengl/opengl_vertex_array.hpp>
#include <tiny/graphics/opengl/opengl_vertex_buffer.hpp>
#include <shader_utility.hpp>
#include <platform.hpp>
#include "opengl_constant_buffer.hpp"

using namespace tiny;
using namespace tiny::graphics;
using namespace tiny::graphics::opengl;


const char* ErrorVertexShaderSrc = R"(
#version 330 core
in vec3 vertexPosition_modelspace;

layout (std140) uniform global_data
{ 
  mat4 view_matrix;
  mat4 projection_matrix;
};

layout (std140) uniform per_object_data
{ 
  mat4 model_matrix;
};

void main(){
	gl_Position =  projection_matrix * view_matrix * model_matrix * vec4(vertexPosition_modelspace, 1);
}
)";

const char* ErrorFragmentShaderSrc = R"(
#version 330 core
out vec3 color;

void main(){
  color = vec3(1,0,1);	// error color
}
)";

OpenGL_GraphicsInterface::OpenGL_GraphicsInterface()
	: m_gl_prim_topo(GL_TRIANGLES)
{
}

OpenGL_GraphicsInterface::~OpenGL_GraphicsInterface()
{
}

bool OpenGL_GraphicsInterface::Initialize()
{
	return true;
}

bool tiny::graphics::opengl::OpenGL_GraphicsInterface::Shutdown()
{
	return true;
}

void OpenGL_GraphicsInterface::SetupFrame(platform::Platform * plat)
{
#ifdef tiny_IS_DLL
	SetInterface(this);
#endif

	m_platform = plat;
	
	SDL_Window* m_window = reinterpret_cast<SDL_Window*>(m_platform->get_window_handle());
	SDL_GL_MakeCurrent(m_window, m_context);
}

ShaderProgramHandle tiny::graphics::opengl::OpenGL_GraphicsInterface::GetErrorProgram()
{
	return m_error_program;
}

GraphicsHandle<IndexBuffer> OpenGL_GraphicsInterface::AllocateIndexBuffer()
{
	return create_handle<OpenGL_IndexBuffer>();
}

GraphicsHandle<VertexBuffer> OpenGL_GraphicsInterface::AllocateVertexBuffer()
{
	return create_handle<OpenGL_VertexBuffer>();
}

GraphicsHandle<VertexArray> tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateVertexArray()
{
	return create_handle<OpenGL_VertexArray>();
}

GraphicsHandle<Shader> tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateShader()
{
	return create_handle<OpenGL_Shader>();
}

GraphicsHandle<ShaderProgram> tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateShaderProgram()
{
	return create_handle<OpenGL_ShaderProgram>();
}

OpenGL_ShaderStorageBufferHandle tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateShaderStorageBuffer()
{
	return create_handle<OpenGL_ShaderStorageBuffer>();
}

TextureHandle tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateTexture()
{
	auto handle = create_handle<OpenGL_Texture>();
	size_t texid = ++m_next_texture_id;

/*
	if (m_freed_texture_ids.size() > 0)
	{
		texid = m_freed_texture_ids.back();
		m_freed_texture_ids.pop_back();
	}
	else
	{
		texid = m_textures.size() + 1;
	}
*/

	while (m_textures.count(texid) > 0)
		++texid;

	handle->internal_set_texture_id(texid);
	m_textures.insert({ texid, tiny::memory::create_refcounted_weak_ptr<Texture>(handle) });
	return handle;
}

ConstantBufferHandle tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateConstantBuffer()
{
	return create_handle<OpenGL_ConstantBuffer>();
}

RenderTextureHandle tiny::graphics::opengl::OpenGL_GraphicsInterface::AllocateRenderTexture()
{
	return create_handle<OpenGL_RenderTexture>();
}

void tiny::graphics::opengl::OpenGL_GraphicsInterface::SetPrimitiveTopology(PrimitiveTopology topology)
{
	m_prim_topo = topology;
	switch (m_prim_topo)
	{
	case PrimitiveTopology::Points:
		m_gl_prim_topo = GL_POINTS;
		break;
	case PrimitiveTopology::Lines:
		m_gl_prim_topo = GL_LINES;
		break;
	case PrimitiveTopology::Triangles:
		m_gl_prim_topo = GL_TRIANGLES;
		break;
	}
}

void tiny::graphics::opengl::OpenGL_GraphicsInterface::EnableConservativeRasterization(bool enabled)
{
	if (enabled)
	{
		GL_CHECK(glEnable(GL_CONSERVATIVE_RASTERIZATION_NV));
	}
	else
	{
		GL_CHECK(glDisable(GL_CONSERVATIVE_RASTERIZATION_NV));
	}
}

PrimitiveTopology tiny::graphics::opengl::OpenGL_GraphicsInterface::GetPrimitiveTopology()
{
	return m_prim_topo;
}

rendering::ShaderLib& tiny::graphics::opengl::OpenGL_GraphicsInterface::GetShaderLibInstance()
{
	return m_shaderlib;
}

GLenum tiny::graphics::opengl::OpenGL_GraphicsInterface::GetGLPrimitiveMode()
{
	return m_gl_prim_topo;
}

void OpenGL_GraphicsInterface::SetClearDepth(float depth)
{
	GL_CHECK(glClearDepth(depth));
}

void OpenGL_GraphicsInterface::SetClearColor(float r, float g, float b, float a)
{
	GL_CHECK(glClearColor(r, g, b, a));
}

void OpenGL_GraphicsInterface::Clear(int flags)
{
	GL_CHECK(glDepthMask(GL_TRUE));
	GL_CHECK(glClear(enum_conversion::ClearFlags(flags)));
}

void OpenGL_GraphicsInterface::SetDepthStencilState(const DepthStencilStateDescription & desc)
{
	using namespace graphics;
	GL_CHECK(glDepthMask(desc.DepthWriteMask == DepthWriteMask::All ? GL_TRUE : GL_FALSE));
	GL_CHECK(glDepthFunc(enum_conversion::ComparisonFunction(desc.DepthFunc)));
	if (desc.DepthEnable)
	{
		GL_CHECK(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_DEPTH_TEST));
	}
	
	if (desc.StencilEnable)
	{
		// TODO: Implement stencil stuff
		assert(false && "OpenGL Backend Error: Stencil functionality not yet enabled");
		GL_CHECK(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_STENCIL_TEST));
	}
}

void OpenGL_GraphicsInterface::SetRasterizerState(const RasterizerStateDescription & desc)
{
	using namespace tiny::graphics;
	if (desc.CullMode == CullMode::None)
	{
		GL_CHECK(glDisable(GL_CULL_FACE));
	}
	else
	{
		GL_CHECK(glEnable(GL_CULL_FACE));
		GL_CHECK(glCullFace(enum_conversion::CullMode(desc.CullMode)));
	}

	if (desc.ScissorEnable)
	{
		GL_CHECK(glEnable(GL_SCISSOR_TEST));
	}
	else
	{
		GL_CHECK(glDisable(GL_SCISSOR_TEST));
	}
	
	switch (desc.FillMode)
	{
	case FillMode::Wireframe:
		GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		break;
	case FillMode::Solid:
		GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	}
}

void OpenGL_GraphicsInterface::SetBlendState(const BlendStateDescription& rhs)
{
	if (rhs.BlendEnable)
	{
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendEquationSeparate(enum_conversion::BlendOp(rhs.BlendOp), enum_conversion::BlendOp(rhs.BlendOpAlpha)));
		GL_CHECK(glBlendFuncSeparate(enum_conversion::Blend(rhs.SrcBlend), enum_conversion::Blend(rhs.DestBlend), enum_conversion::Blend(rhs.SrcBlendAlpha), enum_conversion::Blend(rhs.DestBlendAlpha)));
	}
	else
	{
		GL_CHECK(glDisable(GL_BLEND));
	}
}

void OpenGL_GraphicsInterface::SetScissorRect(const math::screen_rect_t & rhs)
{
	GL_CHECK(glScissor(rhs.pos.x, rhs.pos.y, rhs.size.x, rhs.size.y));
}

void OpenGL_GraphicsInterface::SetViewport(const math::screen_rect_t& rect)
{
	GL_CHECK(glViewport(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y));
}

TextureHandle OpenGL_GraphicsInterface::GetTextureByID(size_t texid)
{
	if (m_textures.count(texid) > 0)
		return m_textures[texid];

	return NULL;
}
#endif