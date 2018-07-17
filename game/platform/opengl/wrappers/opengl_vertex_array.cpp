#include "opengl_util.hpp"
#include "opengl_vertex_array.hpp"
#include "opengl_index_buffer.hpp"
#include "opengl_vertex_buffer.hpp"
#include "opengl_enum_conversion.hpp"
#include "opengl_graphics_interface.hpp"

using namespace tiny::graphics;
using namespace tiny::graphics::opengl;

static GLuint bound_array;

// TODO: Make this work properly
inline static GLenum GetActiveFillMode()
{
	return GL_TRIANGLES;
}

OpenGL_VertexArray::OpenGL_VertexArray()
{
}

OpenGL_VertexArray::~OpenGL_VertexArray()
{
}

void OpenGL_VertexArray::Create()
{
	glGenVertexArrays(1, &m_varr);
	CheckGLError();
}

void OpenGL_VertexArray::Destroy()
{
	if (m_varr)
	{
		glDeleteVertexArrays(1, &m_varr);
		m_varr = 0;
	}
	CheckGLError();
}

void OpenGL_VertexArray::Draw()
{
	BindArray();
	glDrawArrays(GetActiveFillMode(), 0, static_cast<GLsizei>(vertex_count));
	CheckGLError();
}

void tiny::graphics::opengl::OpenGL_VertexArray::Draw(uint32_t count)
{
	BindArray();
	glDrawArrays(GetActiveFillMode(), 0, static_cast<GLsizei>(count));
	CheckGLError();
}

void OpenGL_VertexArray::DrawInstanced(uint32_t cnt)
{
	BindArray();
	glDrawArraysInstanced(GetActiveFillMode(), 0, static_cast<GLsizei>(vertex_count), cnt);
	CheckGLError();
}

void OpenGL_VertexArray::DrawIndexed()
{
	assert(m_varr);

	// No reason to do any rendering if nothing will get rendered
	if (m_ibo->IndexCount() == 0)
	{
#ifndef NDEBUG
		fprintf(stderr, "Attempted to draw with empty VAO\n");
#endif
		return;
	}

	BindArray();

	assert(m_ibo && m_ibo->IndexCount() > 0 && "Attempted to draw with empty or invalid index buffer");
	glDrawElements(GetActiveFillMode(), static_cast<GLsizei>(m_ibo->IndexCount()), GL_UNSIGNED_INT, NULL);

	CheckGLError();
}

void OpenGL_VertexArray::DrawIndexedOverride(size_t count, const IndexType* offset)
{
	assert(m_varr && "Attempted to draw an inexistent vertex array");
	assert(count <= m_ibo->IndexCount() && "Attempted to draw more indices than exist");

	// No reason to do any rendering if nothing will get rendered
	if (m_ibo->IndexCount() == 0)
	{
#ifndef NDEBUG
		fprintf(stderr, "Attempted to draw with empty VAO\n");
#endif
		return;
	}

	BindArray();

	assert(m_ibo && m_ibo->IndexCount() > 0 && "Attempted to draw with empty or invalid index buffer");
	glDrawElements(GetActiveFillMode(), static_cast<GLsizei>(count), GL_UNSIGNED_INT, offset);

	CheckGLError();
}

void OpenGL_VertexArray::DrawIndexedInstanced(uint32_t instanceCount)
{
	assert(m_varr);

	// No reason to do any rendering if nothing will get rendered
	if (m_ibo->IndexCount() == 0)
	{
#ifndef NDEBUG
		fprintf(stderr, "Attempted to draw with empty VAO\n");
#endif
		return;
	}

	BindArray();
	
	assert(m_ibo && m_ibo->IndexCount() > 0 && "Attempted to draw with empty or invalid index buffer");
	glDrawElementsInstanced(GetActiveFillMode(), static_cast<GLsizei>(m_ibo->IndexCount()), GL_UNSIGNED_INT, NULL, instanceCount);
	CheckGLError();
}

void OpenGL_VertexArray::AttachBuffers(GraphicsHandle<IndexBuffer> ibo)
{
	BindArray();
	m_ibo = ibo;
	CheckGLError();
}

void OpenGL_VertexArray::SetVertexDecl(const VertexDecl & decl)
{
	m_decl = decl;
	if (m_decl.HasIndices())
		m_ibo = m_decl.GetIndices();

	const std::vector<VertexData>& data = m_decl.GetData();

	vertex_count = data[0].vbo->GetVertexCount();
	for (auto it = data.begin() + 1; it != data.end(); ++it)
	{
		assert((it->vbo->GetVertexCount() == vertex_count) && "Attempted to attach vertex buffers of varying sizes to vertex array");
	}

	BindArray();
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		glEnableVertexAttribArray(it->index);
		GraphicsHandle<VertexBuffer> handle = it->vbo;

		if (m_vertex_buffers.count(it->vbo) == 0)
			m_vertex_buffers.insert(it->vbo);

		OpenGL_VertexBuffer& buf = dynamic_cast<OpenGL_VertexBuffer&>(*handle);
		buf.BindBuffer();
		glVertexAttribPointer(it->index, static_cast<GLsizei>(it->elem_count), enum_conversion::VertexDeclDataType(it->data_type), it->normalized, static_cast<GLsizei>(it->stride), it->offset);
	}
	OpenGLBuffer<GL_ELEMENT_ARRAY_BUFFER>::Unbind();
	OpenGLBuffer<GL_ARRAY_BUFFER>::Unbind();
	bound_array = 0;
	glBindVertexArray(0);
	CheckGLError();
}

bool OpenGL_VertexArray::HasVertexDecl()
{
	return false;
}

void OpenGL_VertexArray::BindArray()
{
	if (bound_array != m_varr)
	{
		glBindVertexArray(m_varr);
		bound_array = m_varr;

		if (m_ibo)
		{
			OpenGL_IndexBuffer& ibuf = dynamic_cast<OpenGL_IndexBuffer&>(*m_ibo);
			OpenGLBuffer<GL_ELEMENT_ARRAY_BUFFER>::Unbind();
			ibuf.BindBuffer();
		}
	}
	CheckGLError();
}
