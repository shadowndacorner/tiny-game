#include "vertex_decl.hpp"

using namespace tiny::graphics;
void tiny::graphics::VertexDecl::SetIndices(GraphicsHandle<IndexBuffer> ibo)
{
	m_ibo = ibo;
}

bool tiny::graphics::VertexDecl::HasIndices() const
{
	return m_ibo.is_valid();
}

GraphicsHandle<IndexBuffer> tiny::graphics::VertexDecl::GetIndices() const
{
	return m_ibo;
}

int tiny::graphics::VertexDecl::AddElement(GraphicsHandle<VertexBuffer> vbo, const void * offset, size_t elem_size, size_t elem_count, size_t stride, enums::VertexDeclDataType::Enum type, bool norm)
{
	int id = static_cast<int>(m_data.size());
	VertexData data;
	data.normalized = norm;
	data.vbo = vbo;
	data.offset = offset;
	data.elem_size = elem_size;
	data.elem_count = elem_count;
	data.data_type = type;
	data.index = id;
	data.stride = stride;
	m_data.push_back(data);

	return id;
}
