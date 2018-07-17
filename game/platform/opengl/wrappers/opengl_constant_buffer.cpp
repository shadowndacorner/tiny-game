#include "opengl_util.hpp"
#include "opengl_enum_conversion.hpp"
#include "opengl_constant_buffer.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;
OpenGL_ConstantBuffer::OpenGL_ConstantBuffer()
	: m_bindpoint(0), m_datasize(0)
{}

OpenGL_ConstantBuffer::~OpenGL_ConstantBuffer()
{
	Destroy();
}

void OpenGL_ConstantBuffer::Create(enums::BufferUsage::Enum use)
{
	m_buffer.CreateBuffer(use);
}

void OpenGL_ConstantBuffer::Destroy()
{ 
	m_buffer.DestroyBuffer();
}

uint32_t OpenGL_ConstantBuffer::GetBindingPoint()
{
	return m_bindpoint;
}

static GLuint boundCBuffers[OGL_MAX_BOUND_CBUFFERS];
void OpenGL_ConstantBuffer::SetBindingPoint(uint32_t point)
{
	m_buffer.BindBuffer();
	m_bindpoint = point;
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindpoint, m_buffer.GetHandle());
	boundCBuffers[point] = m_buffer.GetHandle();

	CheckGLError();
}

bool OpenGL_ConstantBuffer::BindingPointValid()
{
	return (m_bindpoint < OGL_MAX_BOUND_CBUFFERS) && boundCBuffers[m_bindpoint] == m_buffer.GetHandle();
}

void OpenGL_ConstantBuffer::SetData(const void * data, size_t size)
{
	m_datasize = size;
	m_buffer.SetData((GLsizeiptr)size, data);
}

void OpenGL_ConstantBuffer::SetSubData(size_t offset, const void * data, size_t size)
{
	assert(size <= m_datasize);
	m_buffer.SetDataPartial((GLsizeiptr)size, (GLintptr)offset, data);
}

void OpenGL_ConstantBuffer::GetData(void * dest, size_t size)
{
	auto dat = m_buffer.MapBufferRange(0, size, GL_READ_BUFFER);
	memcpy(dest, dat, size);
	m_buffer.UnmapBuffer();
}

size_t OpenGL_ConstantBuffer::GetDataSize()
{
	return m_datasize;
}

