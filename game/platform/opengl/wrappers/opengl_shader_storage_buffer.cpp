#ifdef MINGINE
#include "opengl_shader_storage_buffer.hpp>
#include "opengl_shader_program.hpp>

tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::OpenGL_ShaderStorageBuffer()
{

}

tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::~OpenGL_ShaderStorageBuffer()
{
	Destroy();
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::Create(enums::BufferUsage::Enum usage)
{
	m_buffer.CreateBuffer(usage);
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::Destroy()
{
	m_buffer.DestroyBuffer();
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::SetData(const void * data, size_t size)
{
	m_buffer.SetData(size, data);
	m_size = size;
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::SetSubData(size_t offset, const void * data, size_t size)
{
	m_buffer.SetDataPartial(size, offset, data);
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::GetData(void * dest, size_t size)
{
	// todo
}

size_t tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::GetDataSize()
{
	return m_size;
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::SetBindPoint(uint32_t bind_point)
{
	m_binding_point = bind_point;
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::BindToShader(ShaderProgramHandle program, const std::string & name)
{
	program->Activate();
	m_buffer.BindBuffer();
	auto oglp = GFX_CAST<OpenGL_ShaderProgram*>(program.get_pointer());
	auto int_id = oglp->GetProgramID();

	auto block_index = GL_CHECK(glGetProgramResourceIndex(int_id, GL_SHADER_STORAGE_BLOCK, name.c_str()));
	if (block_index == GL_INVALID_INDEX)
		return;

	GL_CHECK(glShaderStorageBlockBinding(int_id, block_index, m_binding_point));
	GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding_point, m_buffer.GetHandle()));
	m_buffer.Unbind();
}

/*
void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::SetBindingPoint(uint32_t point, GLuint program)
{
	m_binding_point = point;
	glShaderStorageBlockBinding(program, GetBlockInd, );
}

GLuint tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::GetBlockIndex(GLuint program, const std::string& name)
{
	return glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, name.c_str());
}

void tiny::graphics::opengl::OpenGL_ShaderStorageBuffer::BindToProgram()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding_point, m_buffer.GetHandle());
}
*/
#endif