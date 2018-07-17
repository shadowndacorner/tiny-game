#include "gl_util.h"
#include <graphics/texture.hpp>
#include <stb_image.h>
#include <stdio.h>
#include <algorithm>
#include "wrappers/opengl_texture.hpp"

namespace tiny
{
	namespace graphics
	{
		namespace internal
		{
			struct PlatformSpecificTextureData
			{
				opengl::OpenGL_Texture texture;
			};
		}

		Texture::image_data::image_data()
		{
			texture_data = NULL;
		}

		Texture::image_data::image_data(image_data && rhs)
		{
			std::swap(texture_data, rhs.texture_data);
		}

		Texture::image_data::~image_data()
		{
			unload();
		}

		void* Texture::image_data::load(FILE * file, int & w, int & h, int & channels, int desired_channels)
		{
			stbi_set_flip_vertically_on_load(true);
			return texture_data = stbi_load_from_file(file, &w, &h, &channels, desired_channels);
		}

		void Texture::image_data::unload()
		{
			if (texture_data)
			{
				stbi_image_free(texture_data);
				texture_data = NULL;
			}
		}
	}
}

struct OpenGL_Texture
{

};

using namespace tiny::graphics;
tiny::graphics::Texture::Texture()
{
	platform_specific_data = new graphics::internal::PlatformSpecificTextureData;
}

tiny::graphics::Texture::Texture(Texture&& rhs)
{
	std::swap(platform_specific_data, rhs.platform_specific_data);
}

tiny::graphics::Texture::~Texture()
{
	delete platform_specific_data;
}

bool tiny::graphics::Texture::is_valid() const
{
	return platform_specific_data->texture.is_valid();
}

bool tiny::graphics::Texture::load(const tiny::string & path)
{
	// TODO: Remove stbi, make it a build step
	auto file = fopen(path.c_str(), "r");
	if (!file)
		return false;

	int width, height, channels;
	m_image_data.load(file, width, height, channels);
	/*

	TextureFormat format;
	switch (channels)
	{
	case 1:
		format = TextureFormat::R8;
		break;
	case 2:
		format = TextureFormat::RG;
		break;
	case 3:
		format = TextureFormat::RGB;
		break;
	case 4:
		format = TextureFormat::RGBA;
		break;
	}

	auto& texture = platform_specific_data->texture;
	if (!texture.is_valid())
	{
		texture.create2d(width, height, tiny::graphics::TextureFilterMode::NearestNeighbor, format, tiny::graphics::TextureWrap::Clamp);
		texture.update((char*)m_image_data.texture_data, format, 0);
	}
	fclose(file);
	*/
	return false;
}

bool tiny::graphics::Texture::load_data(TextureFormat dataFormat, void * data, size_t size)
{
	assert(false && "Texture::load_data not yet implemented");
	return false;
}
