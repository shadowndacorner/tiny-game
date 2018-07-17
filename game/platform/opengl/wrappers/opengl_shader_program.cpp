#include "../glad.h"
#include <glm/gtc/type_ptr.hpp>
#include "opengl_util.hpp"
#include "opengl_shader_program.hpp"
#include "opengl_texture.hpp"
#include "opengl_constant_buffer.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;
OpenGL_ShaderProgram::OpenGL_ShaderProgram() :
	shader_id(0), m_linked(false), next_texture_slot(GL_TEXTURE0), is_opaque(false)
{

}

OpenGL_ShaderProgram::~OpenGL_ShaderProgram()
{
	Destroy();
}

void OpenGL_ShaderProgram::Create()
{
	assert(shader_id == 0 && "Attempted to create a shader more than once");
	shader_id = GL_CHECK(glCreateProgram());
	CheckGLError();
}

void OpenGL_ShaderProgram::Destroy()
{
	if (shader_id != 0)
	{
		GL_CHECK(glDeleteProgram(shader_id));
		shader_id = 0;

		for (int i = 0; i < enums::ShaderType::Count; ++i)
		{
			m_shaders[i].clear();
		}
		m_ids.clear();
		m_color_textures.clear();
		m_images.clear();
		m_param_types.clear();
		//m_const_buffers.clear();

		for (int i = 0; i < SHADER_PROGRAM_MAX_BOUND_TEXTURES; ++i)
		{
			bound_textures[i] = 0;
			bound_images[i] = 0;
		}

		next_texture_slot = GL_TEXTURE0;
		next_image_slot = 0;	// This might need to be done
		m_linked = false;
	}
	CheckGLError();
}

void OpenGL_ShaderProgram::Complete()
{
	if (!m_linked)
	{
		GL_CHECK(glLinkProgram(shader_id));

		int rvalue;
		GL_CHECK(glGetProgramiv(shader_id, GL_LINK_STATUS, &rvalue));
		if (!rvalue)
		{
			fprintf(stderr, "Error in linking shader program\n");
			GLchar log[10240];
			GLsizei length;
			GL_CHECK(glGetProgramInfoLog(shader_id, 10239, &length, log));
			fprintf(stderr, "Linker log:\n%s\n", log);
			exit(41);
		}

		m_linked = true;

		GLint i;
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		GL_CHECK(glGetProgramiv(shader_id, GL_ACTIVE_UNIFORMS, &count));
		//printf("Active Uniforms: %d\n", count);

		for (i = 0; i < count; i++)
		{
			GL_CHECK(glGetActiveUniform(shader_id, (GLuint)i, bufSize, &length, &size, &type, name));

			switch (type)
			{
			case GL_UNSIGNED_INT:
				m_param_types[name] = ShaderData::DataType::Uint;
				break;
			case GL_INT:
				m_param_types[name] = ShaderData::DataType::Int;
				break;
			case GL_FLOAT:
				m_param_types[name] = ShaderData::DataType::Float;
				break;
			case GL_FLOAT_VEC2:
				m_param_types[name] = ShaderData::DataType::Vec2;
				break;
			case GL_FLOAT_VEC3:
				m_param_types[name] = ShaderData::DataType::Vec3;
				break;
			case GL_FLOAT_VEC4:
				m_param_types[name] = ShaderData::DataType::Vec4;
				break;
			case GL_FLOAT_MAT3:
				m_param_types[name] = ShaderData::DataType::Mat3;
				break;
			case GL_FLOAT_MAT4:
				m_param_types[name] = ShaderData::DataType::Mat4;
				break;
			case GL_SAMPLER_1D:
			case GL_SAMPLER_3D:
			case GL_SAMPLER_CUBE:
			case GL_SAMPLER_2D:
				m_param_types[name] = ShaderData::DataType::Texture;
				break;
			default:
				m_param_types[name] = ShaderData::DataType::Unknown;
			}
		}
	}
}

bool tiny::graphics::opengl::OpenGL_ShaderProgram::IsValid()
{
	return shader_id != 0;
}

static GLuint bound_id = 0;	// TODO: Share?  Does it matter?
void OpenGL_ShaderProgram::Activate()
{
	if (bound_id == shader_id)
		return;

	for (size_t i = 0; i < SHADER_PROGRAM_MAX_BOUND_TEXTURES; ++i)
		bound_textures[i] = 0;

	for (size_t i = 0; i < SHADER_PROGRAM_MAX_BOUND_TEXTURES; ++i)
		bound_images[i] = 0;

	bound_id = shader_id;
	assert(shader_id);

	glUseProgram(shader_id);
	// Const buffers don't exist yet
	/*
	for (siz-		this	0x0000024f2d9b67c0 {m_shaders=0x0000024f2d9b67c8 {empty, shared_ptr {m_shader_type=Vertex (1) m_gl_shadertype=35633 m_shader=1 ...} [1 strong ref] [make_shared], ...} ...}	tiny::graphics::opengl::OpenGL_ShaderProgram *
e_t i = 0; i < m_const_buffers.size(); ++i)
	{
		OpenGL_ConstBufferBinding& buf = m_const_buffers[i];
		if (buf.buffer.IsValid())
		{
			// If we need to rebind...
			auto ptr = GFX_CAST<OpenGL_ConstantBuffer*>(buf.buffer.GetPointer());
			if (!(ptr->BindingPointValid() && ptr->GetBindingPoint() == buf.slot))
				ptr->SetBindingPoint(buf.slot);
		}
		else
		{
			// TOOD: ensure correctness here
			m_const_buffers.erase(m_const_buffers.begin() + i);
			--i;
		}
	}
	*/
	CheckGLError();
}

void OpenGL_ShaderProgram::Attach(GraphicsHandle<Shader> shader)
{
	assert(shader_id && "Attempted to use shader program before it was created");
	auto type = shader->GetType();
	assert(type != enums::ShaderType::Invalid && "Attempted to attach invalid shader to shader program");

	if (m_shaders[type])
	{
		fprintf(stderr, "Attempting to attach a shader type twice (type == %d)\n", type);
		assert(false && "Attempting to attach a shader type twice");
	}

	m_shaders[type] = shader;

	assert(m_shaders[type]);
	auto& ogl_shad = GFX_CAST<OpenGL_Shader&>(*m_shaders[type]);
	glAttachShader(shader_id, ogl_shad.GetShaderID());
	CheckGLError();
}

void OpenGL_ShaderProgram::Detach(enums::ShaderType::Enum type)
{
	if (m_shaders[type])
	{
		assert(m_shaders[type]);
		auto& ogl_shad = GFX_CAST<OpenGL_Shader&>(*m_shaders[type]);
		glDetachShader(shader_id, ogl_shad.GetShaderID());
	}
	CheckGLError();
}

bool tiny::graphics::opengl::OpenGL_ShaderProgram::IsCompute()
{
	return m_shaders[ShaderType::Compute].is_valid();
}

void OpenGL_ShaderProgram::DispatchCompute(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ)
{
#ifdef glDispatchCompute
	Activate();
	glDispatchCompute(groupsX, groupsY, groupsZ);
	CheckGLError();
#else
	assert(false && "Compute shaders not supported by OpenGL version");
#endif
}

bool OpenGL_ShaderProgram::HasLocation(const std::string & loc)
{
	Activate();
	if (m_ids.count(loc) > 0)
		return true;

	if (GetUniformLocation(loc) >= 0)
		return true;

	int block = GL_CHECK(glGetUniformBlockIndex(shader_id, loc.c_str()));
	if (block != GL_INVALID_INDEX)
		return true;

	return false;
}

void OpenGL_ShaderProgram::SetInt(const std::string & loc, int32_t val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniform1i(id, val);
	m_param_types[loc] = ShaderData::DataType::Int;
	CheckGLError();
}

void OpenGL_ShaderProgram::SetFloat(const std::string & loc, float val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniform1f(id, val);
	m_param_types[loc] = ShaderData::DataType::Float;
	CheckGLError();
}

void OpenGL_ShaderProgram::SetVector(const std::string & loc, const glm::vec2 & val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniform2fv(id, 1, glm::value_ptr(val));
	m_param_types[loc] = ShaderData::DataType::Vec2;
}

void OpenGL_ShaderProgram::SetVector(const std::string & loc, const glm::vec3 & val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniform3fv(id, 1, glm::value_ptr(val));
	m_param_types[loc] = ShaderData::DataType::Vec3;
	CheckGLError();
}

void OpenGL_ShaderProgram::SetVector(const std::string & loc, const glm::vec4 & val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniform4fv(id, 1, glm::value_ptr(val));
	m_param_types[loc] = ShaderData::DataType::Vec4;
	CheckGLError();
}

void OpenGL_ShaderProgram::SetMatrix(const std::string & loc, const glm::mat3 & val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniformMatrix3fv(id, 1, false, glm::value_ptr(val));
	m_param_types[loc] = ShaderData::DataType::Mat3;
	CheckGLError();
}

void OpenGL_ShaderProgram::SetMatrix(const std::string & loc, const glm::mat4 & val)
{
	assert(bound_id == shader_id && "Attempted to set shader uniform while not activated");
	auto id = GetUniformLocation(loc);
	glUniformMatrix4fv(id, 1, false, glm::value_ptr(val));
	m_param_types[loc] = ShaderData::DataType::Mat4;
	CheckGLError();
}

void OpenGL_ShaderProgram::SetTexture(const std::string & loc, TextureHandle tex)
{
	auto id = GetUniformLocation(loc);
	CheckGLError();

	auto slot = GetTextureSlot(id);
	CheckGLError();

	if (tex.is_valid())
	{
		OpenGL_Texture* ntex = GFX_CAST<OpenGL_Texture*>(tex.get_pointer());
		assert(ntex);
		if (bound_textures[slot & TEXTURE_BINDING_MASK] == ntex->GetTextureID())
			return;

		bound_textures[slot & TEXTURE_BINDING_MASK] = ntex->GetTextureID();

		GL_CHECK(glActiveTexture(slot));

		ntex->BindTexture();
	}
	else
	{
		GL_CHECK(glActiveTexture(slot));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}
	m_param_types[loc] = ShaderData::DataType::Texture;
	//glBindTexture(ntex->GetTextureTypeGL(), ntex->GetTextureID());
	CheckGLError();
}

/*
void OpenGL_ShaderProgram::SetImage(int location, TextureHandle tex, int level, bool read, bool write)
{
	CheckGLError();

	assert((read || write) && "Attempted to set image without read or write mode");
	GLenum readWriteMode = (read && write) ? GL_READ_WRITE : (read ? GL_READ_ONLY : GL_WRITE_ONLY);
	if (tex.is_valid())
	{
		OpenGL_Texture* ntex = GFX_CAST<OpenGL_Texture*>(tex.get_pointer());
		assert(ntex);
		GL_CHECK(glBindImageTexture(location, ntex->m_tex, 0, ntex->get_dimension() == 3, 0, readWriteMode, ntex->tex_internal_format));

		int i = 0;
	}
	else
	{
		GL_CHECK(glBindImageTexture(location, 0, level, false, 0, readWriteMode, GL_RGBA));
	}
	CheckGLError();
}

void OpenGL_ShaderProgram::SetConstantBuffer(const std::string & loc, ConstantBufferHandle buf)
{
	// TODO: this might eventually break if we keep setting buffers on a shader deferred_program
	// Validate that the "created" fix works
	Activate();
	bool created;
	auto id = GetUniformBlockIndex(loc, &created);
	
	if (id == GL_INVALID_INDEX)
		return;

	auto ptr = GFX_CAST<OpenGL_ConstantBuffer*>(buf.get_pointer());
	assert(ptr);

	// Nothing has bound this buffer yet
	if (ptr->GetBindingPoint() == 0)
		ptr->SetBindingPoint(static_cast<uint32_t>(m_const_buffers.size()));

	if (created)
		m_const_buffers.push_back({ buf, ptr->GetBindingPoint(), id });

	CheckGLError();
	glUniformBlockBinding(id, ptr->GetBindingPoint());
	m_param_types[loc] = ShaderData::DataType::ConstBuf;
	CheckGLError();
}*/

void tiny::graphics::opengl::OpenGL_ShaderProgram::SetOpaque(bool opaque)
{
	is_opaque = opaque;
}

bool tiny::graphics::opengl::OpenGL_ShaderProgram::IsOpaque()
{
	return is_opaque;
}

std::unordered_map<std::string, ShaderData::DataType> OpenGL_ShaderProgram::GetParams()
{
	return m_param_types;
}
