#include "../glad.h"
#include "opengl_util.hpp"
#include "opengl_texture.hpp"
#include "opengl_enum_conversion.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;

OpenGL_Texture::OpenGL_Texture() :
	m_filter(TextureFilterMode::Bilinear),
	m_format(TextureFormat::RGBA),
	m_tex(0),
	m_width(0),
	m_height(0),
	m_depth(0),
	tex_type(0),
	tex_filter(0),
	tex_internal_format(0),
	tex_wrap(0),
	tex_format(0),
	m_dimension(0)
{}

OpenGL_Texture::~OpenGL_Texture()
{
	destroy();
}

bool tiny::graphics::opengl::OpenGL_Texture::is_valid()
{
	return m_tex != 0;
}

void OpenGL_Texture::destroy()
{
	if (m_tex != 0)
	{
		GL_CHECK(glDeleteTextures(1, &m_tex));
		m_tex = 0;

	}
	CheckGLError();
}

void OpenGL_Texture::create1d(size_t w, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
{
	assert(!m_tex);
	m_mipcount = mipLevelCount;
	tex_type = GL_TEXTURE_1D;
	SetTexFilterGL(filter);
	SetTexFormatInternalGL(format);
	SetTexWrapGL(wrap);

	GL_CHECK(glGenTextures(1, &m_tex));
	m_width = (GLuint)w;
	m_dimension = 1;
	CheckGLError();
}

void OpenGL_Texture::create2d(size_t w, size_t h, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
{
	assert(!m_tex);
	m_mipcount = mipLevelCount;
	tex_type = GL_TEXTURE_2D;
	SetTexFilterGL(filter);
	SetTexFormatInternalGL(format);
	SetTexWrapGL(wrap);

	GL_CHECK(glGenTextures(1, &m_tex));
	m_width = (GLuint)w;
	m_height = (GLuint)h;
	m_dimension = 2;
}

void OpenGL_Texture::create2dArray(size_t w, size_t h, size_t layerCount, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
{
	assert(!m_tex);
	m_mipcount = mipLevelCount;
	tex_type = GL_TEXTURE_2D_ARRAY;
	SetTexFilterGL(filter);
	SetTexFormatInternalGL(format);
	SetTexWrapGL(wrap);

	GL_CHECK(glGenTextures(1, &m_tex));
	m_width = (GLuint)w;
	m_height = (GLuint)h;
	m_dimension = 2;
	m_is_array = true;
}

void OpenGL_Texture::create3d(size_t w, size_t h, size_t d, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
{
	assert(!m_tex);
	m_mipcount = mipLevelCount;
	tex_type = GL_TEXTURE_3D;
	SetTexFilterGL(filter);
	SetTexFormatInternalGL(format);
	SetTexWrapGL(wrap);

	GL_CHECK(glGenTextures(1, &m_tex));
	m_width = (GLuint)w;
	m_height = (GLuint)h;
	m_depth = (GLuint)d;
	m_dimension = 3;
}

inline GLenum FormatToDataType(TextureFormat fmt)
{
	switch (fmt)
	{
	case TextureFormat::R11F_G11F_G10F:
	case TextureFormat::RGBA32F:
	case TextureFormat::RGB32F:
	case TextureFormat::RGB16F:
	case TextureFormat::R16F:
	case TextureFormat::R32F:
	case TextureFormat::DEPTH:
	case TextureFormat::DEPTH16:
	case TextureFormat::DEPTH24:
		return GL_FLOAT;
		break;
	case TextureFormat::RGB10_A2UI:
	case TextureFormat::R16UI:
		return GL_UNSIGNED_SHORT;
		break;
	}
	return GL_UNSIGNED_BYTE;
}

void OpenGL_Texture::update(const char * imgdata, TextureFormat dataFormat, size_t level)
{
	assert(m_tex);
	BindTexture();
	SetTexFormatGL(dataFormat);

	GLenum type = FormatToDataType(m_data_format);
	switch (tex_type)
	{
	case GL_TEXTURE_1D:
		GL_CHECK(glTexImage1D(GL_TEXTURE_1D, GLsizei(level), tex_internal_format, m_width, 0, tex_format, type, imgdata));
		break;
	case GL_TEXTURE_2D:
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, GLsizei(level), tex_internal_format, m_width, m_height, 0, tex_format, type, imgdata));
		break;
	case GL_TEXTURE_3D:
		GL_CHECK(glTexImage3D(GL_TEXTURE_3D, GLsizei(level), tex_internal_format, m_width, m_height, m_depth, 0, tex_format, type, imgdata));
		break;
	default:
		NOT_IMPLEMENTED;
	}
	
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, tex_wrap));
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, tex_wrap));

	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, tex_filter));
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, tex_filter));
}

void OpenGL_Texture::update_partial_1d(const char * imgdata, size_t x, size_t width, TextureFormat fmt, size_t level)
{
	// Not implemented
	NOT_IMPLEMENTED;
}

void OpenGL_Texture::update_partial_2d(const char * imgdata, size_t x, size_t y, size_t width, size_t height, TextureFormat dataFormat, size_t level)
{
	BindTexture();
	assert(tex_type == GL_TEXTURE_2D);

	GLenum type = FormatToDataType(dataFormat);
	SetTexFormatGL(dataFormat);
	glTexSubImage2D(GL_TEXTURE_2D, 0, GLint(x), GLint(y), GLsizei(width), GLsizei(height), tex_format, type, imgdata);

	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, tex_wrap));
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, tex_wrap));

	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, tex_filter));
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, tex_filter));
}

void OpenGL_Texture::update_partial_3d(const char * imgdata, size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth, TextureFormat fmt, size_t level)
{
	// Not implemented
	NOT_IMPLEMENTED;
}

void OpenGL_Texture::resize(size_t w, size_t h, size_t d)
{
	assert(w >= 0 && h >= 0 && d >= 0);
	m_width = static_cast<GLuint>(w);
	m_height = static_cast<GLuint>(h);
	m_depth = static_cast<GLuint>(d);
	update(0, m_data_format);
}

size_t OpenGL_Texture::width()
{
	return m_width;
}

size_t OpenGL_Texture::height()
{
	return m_height;
}

size_t OpenGL_Texture::depth()
{
	return m_depth;
}

glm::ivec2 OpenGL_Texture::get_size()
{
	return glm::ivec2(m_width, m_height);
}

glm::ivec3 OpenGL_Texture::get_size_3()
{
	return glm::ivec3(m_width, m_height, m_depth);
}

void OpenGL_Texture::generate_mipmaps()
{
	BindTexture();
	GL_CHECK(glGenerateMipmap(tex_type));
}

uint8_t OpenGL_Texture::get_dimension()
{
	return m_dimension;
}

bool OpenGL_Texture::is_array()
{
	return m_is_array;
}

void OpenGL_Texture::set_filter_mode(TextureFilterMode mode)
{
	BindTexture();
	SetTexFilterGL(mode);

	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, tex_filter));
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, tex_filter));
}

void OpenGL_Texture::set_wrap_mode(TextureWrap wrap)
{
	BindTexture();
	SetTexWrapGL(wrap);

	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, tex_wrap));
	GL_CHECK(glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, tex_wrap));
}

void OpenGL_Texture::set_format(TextureFormat format)
{
	BindTexture();
	SetTexFormatInternalGL(format);

	char blank[1] = { 0 };
	m_width = 1;
	m_height = 1;
	m_depth = 1;
	update(blank, TextureFormat::RGBA);
}

TextureFormat OpenGL_Texture::get_format()
{
	return m_format;
}

TextureFormat tiny::graphics::opengl::OpenGL_Texture::get_internal_format()
{
	return m_data_format;
}

size_t tiny::graphics::opengl::OpenGL_Texture::get_texture_id()
{
	return m_texid;
}

static GLuint bound = 0;
void OpenGL_Texture::BindTexture()
{
	// TODO: Make this work with multiple binding slots
	//if (bound != m_tex)
	{
		bound = m_tex;
		GL_CHECK(glBindTexture(tex_type, m_tex));
	}
	//slib::exceptions::CheckGLError();
}

void OpenGL_Texture::SetTexFilterGL(TextureFilterMode filter)
{
	switch (filter)
	{
	case TextureFilterMode::NearestNeighbor:
		tex_filter = GL_NEAREST;
		break;
	case TextureFilterMode::Bilinear:
		tex_filter = GL_LINEAR;
		break;
	}

	m_filter = filter;
}

void OpenGL_Texture::SetTexFormatGL(TextureFormat format)
{
	tex_format = enum_conversion::TextureFormat(format);
	m_format = format;
}

void OpenGL_Texture::SetTexFormatInternalGL(TextureFormat format)
{
	tex_internal_format = enum_conversion::TextureFormat(format);
	m_data_format = format;
}

void OpenGL_Texture::SetTexWrapGL(TextureWrap format)
{
	using namespace enums::TextureWrap;
	switch (format)
	{
	case Repeat:
		tex_wrap = GL_REPEAT;
		break;
	case Clamp:
		tex_wrap = GL_CLAMP_TO_EDGE;
		break;
	}
}
