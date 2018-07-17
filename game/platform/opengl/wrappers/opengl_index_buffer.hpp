#pragma once
#include "opengl_buffer_base.hpp"

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			typedef uint16_t IndexType;
			class OpenGL_IndexBuffer
			{
			public:
				OpenGL_IndexBuffer();
				OpenGL_IndexBuffer(OpenGL_IndexBuffer&& rhs);
				~OpenGL_IndexBuffer();

				void Create(enums::BufferUsage::Enum usage);
				void Destroy();
				bool IsValid();

				void SetIndices(const std::vector<IndexType>&);
				void SetIndices(std::vector<IndexType>&&);
				size_t IndexCount();

				void SetUsage(enums::BufferUsage::Enum usage);

			public:
				void BindBuffer();

			private:
				OpenGLBuffer<GL_ELEMENT_ARRAY_BUFFER> m_buffer;
				std::vector<IndexType> m_indices;
			};
		}
		typedef opengl::OpenGL_IndexBuffer IndexBuffer;
	}
}