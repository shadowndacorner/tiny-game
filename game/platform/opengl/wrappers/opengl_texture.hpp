#pragma once
#include <containers/refcounted_ptr.hpp>
#include <glm/glm.hpp>
#include "../glad.h"
#include "../graphics_enums.hpp"

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_Texture
			{
			public:
				OpenGL_Texture();

				~OpenGL_Texture();

			public:
				bool is_valid();
				void destroy();
				void create1d(size_t w, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap);
				void create2d(size_t w, size_t h, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap);
				void create2dArray(size_t w, size_t h, size_t layerCount, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap);
				void create3d(size_t w, size_t h, size_t d, size_t mipLevelCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap);

				inline void create1d(size_t w, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
				{
					create1d(w, 1, filter, format, wrap);
				}

				inline void create2d(size_t w, size_t h, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
				{
					create2d(w, h, 1, filter, format, wrap);
				}

				inline void create2dArray(size_t w, size_t h, size_t layerCount, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
				{
					create2dArray(w, h, layerCount, 1, filter, format, wrap);
				}

				inline void create3d(size_t w, size_t h, size_t d, TextureFilterMode filter, TextureFormat format, TextureWrap wrap)
				{
					create3d(w, h, d, 1, filter, format, wrap);
				}

				void update(const char* imgdata, TextureFormat dataFormat, size_t level = 0);
				void update_partial_1d(const char* imgdata, size_t x, size_t width, TextureFormat, size_t level = 0);
				void update_partial_2d(const char* imgdata, size_t x, size_t y, size_t width, size_t height, TextureFormat, size_t level = 0);
				void update_partial_3d(const char* imgdata, size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth, TextureFormat, size_t level = 0);

				void resize(size_t w, size_t h = 0, size_t d = 0);
				size_t width();
				size_t height();
				size_t depth();

				glm::ivec2 get_size();
				glm::ivec3 get_size_3();

				void generate_mipmaps();
				uint8_t get_dimension();
				bool is_array();

				void set_filter_mode(TextureFilterMode mode);
				void set_wrap_mode(TextureWrap wrap);
				void set_format(TextureFormat format);
				TextureFormat get_format();
				TextureFormat get_internal_format();

				size_t get_texture_id();
				inline void internal_set_texture_id(size_t texid)
				{
					m_texid = texid;
				}

			public:
				// Binded GL exclusive
				void BindTexture();
				inline GLuint GetTextureID()
				{
					return m_tex;
				}

				inline GLuint GetTextureTypeGL()
				{
					return tex_type;
				}

				inline GLuint GetTextureFormat()
				{
					return tex_format;
				}

				inline GLuint GetInternalFormat()
				{
					return tex_internal_format;
				}

				inline void SetTextureID(GLuint id)
				{
					assert(m_tex == 0);
					m_tex = id;
				}

				inline void internal_set_size(int w, int h)
				{
					m_width = static_cast<GLuint>(w);
					m_height = static_cast<GLuint>(h);
				}

			private:
				void SetTexFilterGL(TextureFilterMode filter);
				void SetTexFormatGL(TextureFormat format);
				void SetTexFormatInternalGL(TextureFormat format);
				void SetTexWrapGL(TextureWrap format);

			public:
				size_t m_texid;
				TextureFilterMode m_filter;
				TextureFormat m_format;
				TextureFormat m_data_format;
				size_t m_mipcount;

				GLuint m_tex;
				GLuint m_width;
				GLuint m_height;
				GLuint m_depth;

				GLuint tex_type;
				GLint tex_filter;
				GLint tex_internal_format;
				GLint tex_wrap;
				GLenum tex_format;
				uint8_t m_dimension;
				bool m_is_array;
			};

			typedef memory::refcounted_ptr_t<OpenGL_Texture> TextureHandle;
		}
	}
}