#pragma once
#include "../glad.h"
#include <unordered_set>
#include "../vertex_decl.hpp"

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_VertexArray
			{
			public:
				OpenGL_VertexArray();
				~OpenGL_VertexArray();

			public:
				void Create();
				void Destroy();
				void Draw();
				void Draw(uint32_t count);
				void DrawInstanced(uint32_t);
				void DrawIndexed();
				void DrawIndexedOverride(size_t count, const IndexType* offset);
				void DrawIndexedInstanced(uint32_t instanceCount);
				void AttachBuffers(GraphicsHandle<IndexBuffer>);
				void SetVertexDecl(const VertexDecl& decl);
				bool HasVertexDecl();
			
			public:
				void BindArray();

			private:
				std::unordered_set<GraphicsHandle<VertexBuffer>> m_vertex_buffers;
				GraphicsHandle<IndexBuffer> m_ibo;
				GLuint m_varr;
				VertexDecl m_decl;
				size_t vertex_count;
			};
		}
	}
}