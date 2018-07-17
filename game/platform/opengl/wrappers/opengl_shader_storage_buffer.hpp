#ifdef MINGINE
#pragma once
#include "../graphics_handle.hpp"
#include "opengl_buffer_base.hpp"
#include "../shader_program.hpp"

// Note: This is sloppy as hell, I'll improve it later
namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_ShaderStorageBuffer
			{
			public:
				OpenGL_ShaderStorageBuffer();
				~OpenGL_ShaderStorageBuffer();

			public:
				void Create(enums::BufferUsage::Enum);
				void Destroy();

				void SetData(const void* data, size_t size);
				void SetSubData(size_t offset, const void* data, size_t size);
				void GetData(void* dest, size_t size);
				size_t GetDataSize();

				void SetBindPoint(uint32_t bind_point);
				void BindToShader(ShaderProgramHandle program, const std::string& name);

				//GLuint GetBlockIndex(GLuint program, const std::string& name);
				//void BindToProgram();

			private:
				OpenGLBuffer<GL_SHADER_STORAGE_BUFFER> m_buffer;
				size_t m_size;
				uint32_t m_binding_point;
			};

			CREATE_HANDLE_TYPE(OpenGL_ShaderStorageBuffer);
		}
	}
}
#endif