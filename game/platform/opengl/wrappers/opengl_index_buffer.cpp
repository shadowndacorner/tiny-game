#include "opengl_index_buffer.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;
OpenGL_IndexBuffer::OpenGL_IndexBuffer()
{}

OpenGL_IndexBuffer::OpenGL_IndexBuffer(OpenGL_IndexBuffer && rhs) :
	m_buffer(std::move(rhs.m_buffer)),
	m_indices(std::move(rhs.m_indices))
{}

OpenGL_IndexBuffer::~OpenGL_IndexBuffer()
{}

void OpenGL_IndexBuffer::Create(enums::BufferUsage::Enum usage)
{
	m_buffer.CreateBuffer(usage, enums::BufferMode::Draw);
}

void OpenGL_IndexBuffer::Destroy()
{
	m_buffer.DestroyBuffer();
}

bool tiny::graphics::opengl::OpenGL_IndexBuffer::IsValid()
{
	return m_buffer.IsValid();
}

void OpenGL_IndexBuffer::SetIndices(const std::vector<IndexType>& data)
{
	m_indices = data;
	m_buffer.SetData(m_indices);
}

void OpenGL_IndexBuffer::SetIndices(std::vector<IndexType>&& data)
{
	m_indices.swap(data);
	m_buffer.SetData(m_indices);
}

size_t OpenGL_IndexBuffer::IndexCount()
{
	return m_indices.size();
}

void OpenGL_IndexBuffer::SetUsage(enums::BufferUsage::Enum usage)
{
	m_buffer.SetGLBufferUsage(usage);
}

void OpenGL_IndexBuffer::BindBuffer()
{
	m_buffer.BindBuffer();
}
