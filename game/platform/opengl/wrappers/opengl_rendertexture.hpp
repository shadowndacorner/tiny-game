#pragma once
#include <containers/vector.hpp>
#include "opengl_graphics_interface.hpp"
#include "../glad.h"
#include "opengl_texture.hpp"
#include "../graphics_enums.hpp"

namespace tiny
{
	namespace graphics
	{
		class OpenGL_RenderTexture
		{
		public:
			OpenGL_RenderTexture();
			~OpenGL_RenderTexture();

		public:
			void Create();
			void Create(int width, int height, int depth);
			void Resize(int width, int height);
			void Resize(int width, int height, int depth);
			void Destroy();

			int CreateColorTexture(TextureFormat);
			int CreateDepthTexture();
			void SetDrawTargets(const tiny::vector<uint32_t>& targetIndices);

			TextureHandle GetColorAttachment(int id);
			TextureHandle GetDepthAttachment(int id);

			void* GetDepthAttachment();
			void SetDepthAttachment(void*);

		public:
			void Validate();

			inline GLuint GetFBOId()
			{
				return m_fbo;
			}

			inline int GetWidth()
			{
				return m_width;
			}

			inline int GetHeight()
			{
				return m_height;
			}

			inline const std::vector<GLenum>& GetDrawbuffers()
			{
				return m_drawbuffers;
			}

		private:
			std::vector<TextureHandle> m_color_textures;
			std::vector<TextureHandle> m_depth_textures;
			std::vector<GLenum> m_drawbuffers;
			GLuint m_fbo;
			GLuint m_depthrb;
			int m_width;
			int m_height;
			int m_depth;
			bool m_validated;
		};
	}
}