#include <cassert>
#include "opengl_util.hpp"
#include "opengl_shader.hpp"
using namespace tiny::graphics;
using namespace tiny::graphics::opengl;
OpenGL_Shader::OpenGL_Shader() : 
	m_shader(0),
	m_shader_type(enums::ShaderType::Invalid),
	m_gl_shadertype(0),
	m_initialized(false)
{}

OpenGL_Shader::~OpenGL_Shader()
{
	Destroy();
}

bool OpenGL_Shader::IsValid()
{
	return m_shader != 0;
}

bool OpenGL_Shader::Ready()
{ 
	return m_initialized;
}

void OpenGL_Shader::CreateFromSource(enums::ShaderType::Enum type, const std::string& source_string)
{
	m_shader_type = type;

	SetShaderTypeGL(type);

	m_shader = GL_CHECK(glCreateShader(m_gl_shadertype));

	const char* src = source_string.c_str();
	GLint g_len = (GLint)source_string.length();
	GL_CHECK(glShaderSource(m_shader, 1, &src, &g_len));
	GL_CHECK(glCompileShader(m_shader));

	if (GetShaderError())
	{
		fprintf(stderr, "Shader compilation failed, check application log\n");
		Destroy();
		return;
	}

	m_initialized = true;
	return;
}

void OpenGL_Shader::Destroy()
{
	if (m_shader != 0)
	{
		glDeleteShader(m_shader);
		m_shader = 0;
		m_initialized = false;
		m_shader_type = ShaderType::Invalid;
	}
}

enums::ShaderType::Enum OpenGL_Shader::GetType()
{
	return m_shader_type;
}
