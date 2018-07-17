#ifdef MINGINE
#pragma once
#include "../glad.h"
#include <unordered_map>
#include "../graphics_enums.hpp"
#include "opengl_rendertexture.hpp"
#include "opengl_shader_storage_buffer.hpp"

#ifdef MINGINE_SDL
struct SDL_Window;
#endif

namespace mingine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenGL_GraphicsInterface
			{
			public:
				OpenGL_GraphicsInterface();
				~OpenGL_GraphicsInterface();

			public:
				void SetActiveRenderTexture(RenderTextureHandle handle);
				RenderTextureHandle GetActiveRenderTexture();

			public:
				// TODO: THIS SYSTEM IS TEMPORARY!  TEXTURES WILL CURRENTLY NOT BE FREED UNTIL SHUTDOWN!!!
				TextureHandle GetTextureByID(size_t texid);

				inline void KillTextureHandle(size_t texid)
				{
					if (m_textures.count(texid) > 0)
					{
						m_textures.erase(texid);
						//m_freed_texture_ids.push_back(texid);
					}
				}

				ShaderProgramHandle GetErrorProgram();

			public:
				GraphicsHandle<IndexBuffer> AllocateIndexBuffer();
				GraphicsHandle<VertexBuffer> AllocateVertexBuffer();
				GraphicsHandle<VertexArray> AllocateVertexArray();

				GraphicsHandle<Shader> AllocateShader();
				GraphicsHandle<ShaderProgram> AllocateShaderProgram();
				OpenGL_ShaderStorageBufferHandle AllocateShaderStorageBuffer();
				
				TextureHandle AllocateTexture();
				ConstantBufferHandle AllocateConstantBuffer();
				RenderTextureHandle AllocateRenderTexture();

				void SetPrimitiveTopology(PrimitiveTopology topology);
				void EnableConservativeRasterization(bool enabled);

				PrimitiveTopology GetPrimitiveTopology();

				rendering::ShaderLib& GetShaderLibInstance();

			public:
				GLenum GetGLPrimitiveMode();

			private:
				rendering::ShaderLib m_shaderlib;
				std::unordered_map<size_t, TextureHandle> m_textures;
				//std::vector<size_t> m_freed_texture_ids;
				ShaderProgramHandle m_error_program;
				platform::Platform* m_platform;
				PrimitiveTopology m_prim_topo;
				GLenum m_gl_prim_topo;
				size_t m_next_texture_id;
				void* m_context;
#ifdef MINGINE_SDL
				SDL_Window* m_window;

				// TODO: Implement
				SDL_Window* m_asyncwindow;
				void* m_asynccontext;
#endif
			};
		}
	}
}
#endif