#pragma once
#include "opengl_buffer_base.hpp"

#define OGL_MAX_BOUND_CBUFFERS 8
namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_ConstantBuffer
			{
			public:
				OpenGL_ConstantBuffer();
				~OpenGL_ConstantBuffer();

			public:
				void Create(enums::BufferUsage::Enum);
				void Destroy();

				uint32_t GetBindingPoint();
				void SetBindingPoint(uint32_t point);
				bool BindingPointValid();

				void SetData(const void* data, size_t size);
				void SetSubData(size_t offset, const void* data, size_t size);
				void GetData(void* dest, size_t size);
				size_t GetDataSize();

			public:
				inline GLuint GetHandle()
				{
					return m_buffer.GetHandle();
				}

			private:
				OpenGLBuffer<GL_UNIFORM_BUFFER> m_buffer;
				size_t m_datasize;
				GLuint m_bindpoint;
			};
		}
	}
}