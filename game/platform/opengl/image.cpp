#include "gl_util.h"
#include <graphics/image.hpp>
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

		Image::image_data::image_data()
		{
			texture_data = NULL;
		}

		Image::image_data::image_data(image_data && rhs)
		{
			std::swap(texture_data, rhs.texture_data);
		}

		Image::image_data::~image_data()
		{
			unload();
		}

		void* Image::image_data::load(const char* file, int & w, int & h, int & channels, int desired_channels)
		{
			stbi_set_flip_vertically_on_load(true);
			texture_data = stbi_load(file, &w, &h, &channels, desired_channels);
			if (!texture_data)
			{
				fprintf(stderr, "stbi: Failed to load image with reason: %s\n", stbi_failure_reason());
			}
			return texture_data;
		}

		void* Image::image_data::load(FILE * file, int & w, int & h, int & channels, int desired_channels)
		{
			stbi_set_flip_vertically_on_load(true);
			texture_data = stbi_load_from_file(file, &w, &h, &channels, desired_channels);
			if (!texture_data)
			{
				fprintf(stderr, "stbi: Failed to load image with reason: %s\n", stbi_failure_reason());
			}
			return texture_data;
		}

		void Image::image_data::unload()
		{
			if (texture_data)
			{
				stbi_image_free(texture_data);
				texture_data = NULL;
			}
		}
	}
}

using namespace tiny::graphics;
tiny::graphics::Image::Image()
{
	platform_specific_data = new graphics::internal::PlatformSpecificTextureData;
}

tiny::graphics::Image::Image(Image&& rhs)
{
	std::swap(platform_specific_data, rhs.platform_specific_data);
}

tiny::graphics::Image::~Image()
{
	delete platform_specific_data;
}

bool tiny::graphics::Image::is_valid() const
{
	return platform_specific_data->texture.is_valid();
}

bool tiny::graphics::Image::load(const tiny::string & path)
{
	// TODO: Remove stbi, make it a build step
	int width, height, channels;
	if (!m_image_data.load(path.c_str(), width, height, channels))
	{
		return false;
	}

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
	return false;
}

bool tiny::graphics::Image::load_data(TextureFormat dataFormat, void * data, size_t size)
{
	assert(false && "Texture::load_data not yet implemented");
	return false;
}
