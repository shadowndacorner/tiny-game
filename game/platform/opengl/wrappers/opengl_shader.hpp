#pragma once
#include "../glad.h"
#include "../graphics_enums.hpp"
#include <containers/string.hpp>
#include <stdio.h>

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_Shader
			{
			public:
				OpenGL_Shader();
				OpenGL_Shader(const OpenGL_Shader& rhs) = delete;
				~OpenGL_Shader();

			public:
				bool IsValid();

				bool Ready();
				void CreateFromSource(enums::ShaderType::Enum, const tiny::string& shaderSource);
				void Destroy();
				enums::ShaderType::Enum GetType();

			public:
				inline void SetShaderTypeGL(enums::ShaderType::Enum type)
				{
					using namespace enums::ShaderType;
					switch (type)
					{
					case Vertex:
						m_gl_shadertype = GL_VERTEX_SHADER;
						break;
					case Fragment:
						m_gl_shadertype = GL_FRAGMENT_SHADER;
						break;
					case Geometry:
						m_gl_shadertype = GL_GEOMETRY_SHADER;
						break;
					case Compute:
#ifdef GL_COMPUTE_SHADER
						m_gl_shadertype = GL_COMPUTE_SHADER;
#else
						fprintf(stderr, "Compute shaders not currently supported\n");
						assert(false && "Compute shaders not currently supported");
#endif
						break;
					default:
						fprintf(stderr, "Attempted to use unsupported shader of type %i\n", type);
						assert(false && "Attempted to use unsupported shader type");
						break;
					}
				}

				inline bool GetShaderError()
				{
					int success;
					char infoLog[512];
					glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
					if (!success)
					{
						glGetShaderInfoLog(m_shader, 512, NULL, infoLog);
						fprintf(stderr, "ERROR::SHADER::");
						switch (m_shader_type)
						{
						case enums::ShaderType::Vertex:
							fprintf(stderr, "VERTEX");
							break;
						case enums::ShaderType::Fragment:
							fprintf(stderr, "FRAGMENT");
							break;
						case enums::ShaderType::Geometry:
							fprintf(stderr, "GEOMETRY");
							break;
						case enums::ShaderType::Compute:
							fprintf(stderr, "COMPUTE");
							break;
						}
						fprintf(stderr, "::COMPILATION_FAILED%s%s\n", infoLog, "\n");
						return true;
					}
					return false;
				}

				inline GLuint GetShaderID()
				{
					return m_shader;
				}

			private:
				enums::ShaderType::Enum m_shader_type;
				GLenum m_gl_shadertype;
				GLuint m_shader;
				bool m_initialized;
			};
		}
		typedef opengl::OpenGL_Shader Shader;
	}
}