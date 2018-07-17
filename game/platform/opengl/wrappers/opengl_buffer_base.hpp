#pragma once
#include "../glad.h"
#include "../graphics_enums.hpp"
#include "opengl_enum_conversion.hpp"
#include "opengl_util.hpp"
#include <vector>

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			struct Binding
			{
				template <GLenum BufferType>
				static GLuint& bound_buffer()
				{
					static GLuint buf = 0;
					return buf;
				}
			};

			template <GLenum BufferType>
			class OpenGLBuffer
			{
			public:
				// Creates a null OpenGL buffer
				inline OpenGLBuffer() :
					buf_id(0),
					buf_usage(0)
				{}

				// Creates an OpenGL buffer with the given mode
				inline OpenGLBuffer(enums::BufferUsage::Enum use,
					enums::BufferMode::Enum mode = enums::BufferMode::Draw) :
					buf_id(0),
					buf_usage(0)
				{
					CreateBuffer(use, mode);
				}

				// Move constructor for opengl buffer.  Hands the buffer
				// name to the new object.
				inline OpenGLBuffer(OpenGLBuffer&& rhs) :
					buf_id(rhs.buf_id), buf_usage(rhs.buf_usage)
				{
					rhs.buf_id = 0;
					rhs.buf_usage = 0;
				}

				inline ~OpenGLBuffer()
				{
					DestroyBuffer();
				}

			public:
				inline GLuint CreateBuffer(
					enums::BufferUsage::Enum use, 
					enums::BufferMode::Enum mode = enums::BufferMode::Draw)
				{
					assert(buf_id == 0);
					SetGLBufferUsage(use, mode);
					glGenBuffers(1, &buf_id);
					CheckGLError();
					return buf_id;
				}

				inline void DestroyBuffer()
				{
					if (buf_id == 0)
						return;

					if (Binding::bound_buffer<BufferType>() == buf_id)
					{
						glBindBuffer(BufferType, 0);
						Binding::bound_buffer<BufferType>() = 0;
					}

					CheckGLError();
					if (buf_id != 0)
					{
						glDeleteBuffers(1, &buf_id);
						buf_id = 0;
					}
					CheckGLError();
				}

				inline void SetData(GLsizeiptr size, const void* data)
				{
					BindBuffer();
					glBufferData(BufferType, size, data, buf_usage);
					CheckGLError();
					buffer_size = size;
				}

				inline void SetDataPartial(GLsizeiptr size, GLintptr offset, const void* data)
				{
					assert((offset + size) <= buffer_size && "Attempted to set partial buffer data > buffer size");
					BindBuffer();
					glBufferSubData(BufferType, offset, size, data);
					CheckGLError();
				}

				inline void* MapBuffer(GLenum access)
				{
					BindBuffer();
					auto ret = glMapBuffer(BufferType, access);
					CheckGLError();
					return ret;
				}

				inline void* MapBufferRange(GLintptr offset, GLsizeiptr length, GLenum access)
				{
					BindBuffer();
					auto ret = glMapBufferRange(BufferType, offset, length, access);
					CheckGLError();
					return ret;
				}

				inline void UnmapBuffer()
				{
					glUnmapBuffer(BufferType);
					CheckGLError();
				}

				template <typename T>
				inline void SetData(const std::vector<T>& data)
				{
					BindBuffer();
					glBufferData(BufferType, data.size() * sizeof(T), data.data(), buf_usage);
					CheckGLError();
				}

				inline void BindBuffer()
				{
					if (Binding::bound_buffer<BufferType>() != buf_id)
					{
						Binding::bound_buffer<BufferType>() = buf_id;
						glBindBuffer(BufferType, buf_id);
					}
					CheckGLError();
				}

				static inline void Unbind()
				{
					Binding::bound_buffer<BufferType>() = 0;
					GL_CHECK(glBindBuffer(BufferType, 0));
				}

				inline void SetGLBufferUsage(enums::BufferUsage::Enum usage,
					enums::BufferMode::Enum mode = enums::BufferMode::Draw)
				{
					buf_usage = enum_conversion::BufferEnum(usage, mode);
				}

				inline GLuint GetHandle()
				{
					return buf_id;
				}

				inline bool IsValid() const
				{
					return buf_id != 0;
				}

			private:
				int64_t buffer_size;
				GLuint buf_id;
				GLenum buf_usage;
			};

			/*
			template <GLenum BufferType>
			class OpenGLBufferDSA
			{
			public:

				inline GLuint CreateBuffer(BufferUsage::BufferUsage use)
				{
					SetGLBufferUsage(use);
					glCreateBuffers(1, &buf_id);
					CheckGLError();
					return buf_id;
				}

				inline void DestroyBuffer()
				{
					if (Binding::bound_buffer<BufferType>() == buf_id)
					{
						glBindBuffer(BufferType, 0);
						Binding::bound_buffer<BufferType>() = 0;
					}

					glDeleteBuffers(1, &buf_id);
					buf_id = 0;
					CheckGLError();
				}

				inline void SetData(GLsizeiptr size, const void* data)
				{
					glNamedBufferData(buf_id, size, data, buf_usage);
					CheckGLError();
				}

				inline void SetDataPartial(GLsizeiptr size, GLintptr offset, const void* data)
				{
					glNamedBufferSubData(buf_id, offset, size, data);
					CheckGLError();
				}

				inline void* MapBuffer(GLenum access)
				{
					auto ret = glMapNamedBuffer(buf_id, access);
					CheckGLError();
					return ret;
				}

				inline void* MapBufferRange(GLintptr offset, GLsizeiptr length, GLenum access)
				{
					auto ret = glMapNamedBufferRange(buf_id, offset, length, access);
					CheckGLError();
					return ret;
				}

				inline void UnmapBuffer()
				{
					glUnmapNamedBuffer(buf_id);
					CheckGLError();
				}

				template <typename T>
				inline void SetData(const std::vector<T>& data)
				{
					glNamedBufferData(buf_id, data.size() * sizeof(T), data.data(), buf_usage);
					CheckGLError();
				}

				inline void BindBuffer()
				{
					if (Binding::bound_buffer<BufferType>() != buf_id)
					{
						Binding::bound_buffer<BufferType>() = buf_id;
						glBindBuffer(BufferType, buf_id);
					}
					CheckGLError();
				}

				static inline void Unbind()
				{
					Binding::bound_buffer<BufferType>() = 0;
					glBindBuffer(BufferType, 0);
				}

				inline void SetGLBufferUsage(BufferUsage::BufferUsage usage)
				{
					using namespace BufferUsage;
					switch (usage)
					{
					case Static:
						buf_usage = GL_STATIC_DRAW;
						break;
					case Dynamic:
						buf_usage = GL_DYNAMIC_DRAW;
						break;
					case Stream:
						buf_usage = GL_STREAM_DRAW;
						break;
					}
				}

				inline GLuint GetHandle()
				{
					return buf_id;
				}

			private:
				GLuint buf_id;
				GLenum buf_usage;
			};
			*/
		}
	}
}