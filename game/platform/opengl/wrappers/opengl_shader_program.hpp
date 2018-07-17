#pragma once
#include <vector>
#include "../graphics_handle.hpp"
#include "opengl_shader.hpp"
#include <unordered_map>
#include <containers/string.hpp>
#include "opengl_texture.hpp"

#define SHADER_PROGRAM_MAX_BOUND_TEXTURES 32
#define TEXTURE_BINDING_MASK (SHADER_PROGRAM_MAX_BOUND_TEXTURES - 1)

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
#ifdef MINGINE
			struct OpenGL_ConstBufferBinding
			{
				ConstantBufferHandle buffer;
				uint32_t slot;
				GLuint uniform;
			};
#endif

			class ShaderData
			{
			public:
				enum class DataType
				{
					Error,
					Unknown,
					Uint,
					Int,
					Float,

					Vec2,
					Vec3,
					Vec4,

					Mat3,
					Mat4,

					Texture,
					Image,

					ConstBuf
				};

			public:
				inline ShaderData() :
					m_data(DataType::Error),
					m_data_ptr(NULL) {}

				template <typename T>
				inline ShaderData(const T& data, DataType overrideType)
					: m_data(get_type<T>())
				{
					m_data = overrideType;
					m_data_ptr = new T(data);	// assume a copy constructor exists
					m_owns = true;
				}

				inline ShaderData(ShaderData&& rhs) :
					m_data(rhs.m_data),
					m_data_ptr(rhs.m_data_ptr),
					m_owns(true)
				{
					rhs.m_owns = false;
					rhs.m_data_ptr = 0;
				}

				inline ShaderData(const ShaderData& rhs) :
					m_data(rhs.m_data), m_data_ptr(rhs.m_data_ptr), m_owns(false)
				{
				}

				template <typename T>
				ShaderData& operator=(const T& rhs)
				{
					assert(get_type<T>() != DataType::Error);
					if (m_data_ptr && m_owns)
						delete m_data_ptr;
					m_data = get_type<T>();
					auto cdata = new T(rhs);	// assume a copy constructor exists
					m_data_ptr = cdata;
					m_owns = true;
					return *this;
				}

				inline ~ShaderData()
				{
					// TODO: Save the destructor
					if (m_owns)
						delete m_data_ptr;
				};

			public:

#pragma region Types
				template <typename T>
				inline DataType get_type()
				{
					return DataType::Error;
				}
#pragma endregion

				inline DataType data_type()
				{
					return m_data;
				}

				template <typename T>
				inline const bool is()
				{
					return m_data == get_type<T>();
				}

				template<typename T>
				inline const T& get()
				{
					return *static_cast<T*>(m_data_ptr);
				}

			private:
				DataType m_data;
				void* m_data_ptr;
				bool m_owns;
			};

			class OpenGL_ShaderProgram
			{
			public:
				OpenGL_ShaderProgram();
				~OpenGL_ShaderProgram();

			public:
				void Create();
				void Destroy();
				void Complete();
				bool IsValid();

				void Activate() ;
				void Attach(GraphicsHandle<Shader> shader) ;
				void Detach(enums::ShaderType::Enum type);

				bool IsCompute();
				void DispatchCompute(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ);

				bool HasLocation(const tiny::string& loc);
				void SetInt(const tiny::string& loc, int32_t val);
				void SetFloat(const tiny::string& loc, float val);

				void SetVector(const tiny::string & loc, const glm::vec2& val);
				void SetVector(const tiny::string & loc, const glm::vec3& val);
				void SetVector(const tiny::string& loc, const glm::vec4& val);

				void SetMatrix(const tiny::string& loc, const glm::mat3& val);
				void SetMatrix(const tiny::string& loc, const glm::mat4& val);

				void SetTexture(const tiny::string& loc, opengl::TextureHandle tex);

				//void SetConstantBuffer(const tiny::string& loc, ConstantBufferHandle buf);

				void SetOpaque(bool opaque);
				bool IsOpaque();

				std::unordered_map<tiny::string, ShaderData::DataType> GetParams();

				// ...

				//void SetConstantBuffer(const tiny::string& loc, ConstantBufferHandle buf);

			public:
				inline GLuint GetProgramID()
				{
					return shader_id;
				}

			private:
				inline GLint GetUniformLocation(const tiny::string& loc)
				{
					assert(shader_id);
					if (m_ids.count(loc) == 0)
					{
						m_ids[loc] = GL_CHECK(glGetUniformLocation(shader_id, loc.c_str()));
					}

					return m_ids[loc];
				}

				inline GLuint GetUniformBlockIndex(const tiny::string& loc, bool* created)
				{
					assert(shader_id);
					if (m_ids.count(loc) == 0)
					{
						m_ids[loc] = GL_CHECK(glGetUniformBlockIndex(shader_id, loc.c_str()));
						*created = true;
					}
					else
					{
						*created = false;
					}
					return m_ids[loc];
				}

				inline GLenum GetTextureSlot(GLint uniform)
				{
					if (m_color_textures.count(uniform) == 0)
					{
						auto slot = next_texture_slot++;
						GLint sslot = slot - GL_TEXTURE0;
						m_color_textures[uniform] = slot;
						glUniform1iv(uniform, 1, &sslot);
						CheckGLError();
						return slot;
					}

					return m_color_textures[uniform];
				}

				inline GLenum GetImageSlot(GLint uniform)
				{
					if (true)
						return 0;
					if (m_images.count(uniform) == 0)
					{
						auto slot = next_image_slot++;
						GLint sslot = slot - GL_TEXTURE0;
						m_images[uniform] = slot;
						glUniform1iv(uniform, 1, &sslot);
						return slot;
					}

					return m_color_textures[uniform];
				}

			private:
				GraphicsHandle<Shader> m_shaders[enums::ShaderType::Count];
				std::unordered_map<tiny::string, GLint> m_ids;
				std::unordered_map<GLint, GLenum> m_color_textures;
				std::unordered_map<GLint, GLenum> m_images;
				std::unordered_map<tiny::string, ShaderData::DataType> m_param_types;
				//std::vector<OpenGL_ConstBufferBinding> m_const_buffers;
				GLuint bound_textures[SHADER_PROGRAM_MAX_BOUND_TEXTURES];
				GLuint bound_images[SHADER_PROGRAM_MAX_BOUND_TEXTURES];

				GLenum shader_id;
				GLenum next_texture_slot;
				GLenum next_image_slot;
				bool m_linked;
				bool is_opaque;
			};
		}
	}
}