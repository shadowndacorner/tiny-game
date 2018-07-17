#include "opengl_vertex_buffer.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;
OpenGL_VertexBuffer::OpenGL_VertexBuffer() :
	m_vert_count(0)
{
}
OpenGL_VertexBuffer::~OpenGL_VertexBuffer()
{
	Destroy();
}

void OpenGL_VertexBuffer::Create(enums::BufferUsage::Enum usage)
{
	m_buffer.CreateBuffer(usage, enums::BufferMode::Draw);
}

void OpenGL_VertexBuffer::Destroy()
{
	m_buffer.DestroyBuffer();
}

bool tiny::graphics::opengl::OpenGL_VertexBuffer::IsValid() const
{
	return m_buffer.IsValid();
}

void OpenGL_VertexBuffer::SetData(size_t vCount, size_t size, const void * data)
{
	m_vert_count = vCount;
	m_buffer.SetData(static_cast<GLsizeiptr>(size), data);
}

void OpenGL_VertexBuffer::SetSubData(size_t vCount, size_t start, size_t length, const void * data)
{
	m_vert_count = vCount;
	m_buffer.SetDataPartial(static_cast<GLsizeiptr>(length), static_cast<GLsizeiptr>(start), data);
}

size_t OpenGL_VertexBuffer::GetVertexCount() const
{
	return m_vert_count;
}

void OpenGL_VertexBuffer::BindBuffer()
{
	m_buffer.BindBuffer();
}
