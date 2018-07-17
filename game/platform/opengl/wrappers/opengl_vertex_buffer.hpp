#pragma once
#include "opengl_buffer_base.hpp"

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_VertexBuffer
			{
			public:
				OpenGL_VertexBuffer();
				~OpenGL_VertexBuffer();

			public:
				void Create(enums::BufferUsage::Enum);
				void Destroy();
				bool IsValid() const;

				void SetData(size_t vCount, size_t size, const void* data);
				void SetSubData(size_t vCount, size_t start, size_t length, const void* data);

				size_t GetVertexCount() const;

			public:
				void BindBuffer();
				inline OpenGLBuffer<GL_ARRAY_BUFFER>& GetBuffer()
				{
					return m_buffer;
				}

			private:
				OpenGLBuffer<GL_ARRAY_BUFFER> m_buffer;
				size_t m_vert_count;
			};
		}
		typedef opengl::OpenGL_VertexBuffer VertexBuffer;
	}
}