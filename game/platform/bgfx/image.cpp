#include <graphics/image.hpp>
#include <stb_image.h>
#include <stdio.h>
#include <algorithm>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace tiny
{
	namespace graphics
	{
		namespace internal
		{
			struct PlatformSpecificTextureData
			{
				PlatformSpecificTextureData() { texture.idx = bgfx::kInvalidHandle; }
				bgfx::TextureHandle texture;
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
	return bgfx::isValid(platform_specific_data->texture);
}

bool tiny::graphics::Image::load(const tiny::string & path)
{
	// TODO: Remove stbi, make it a build step
	int width, height, channels;
	if (!m_image_data.load(path.c_str(), width, height, channels))
	{
		return false;
	}

	bgfx::TextureFormat::Enum format;
	switch (channels)
	{
	case 1:
		format = bgfx::TextureFormat::R8;
		break;
	case 2:
		format = bgfx::TextureFormat::RG8;
		break;
	case 3:
		format = bgfx::TextureFormat::RGB8;
		break;
	case 4:
		format = bgfx::TextureFormat::RGBA8;
		break;
	}

	auto& texture = platform_specific_data->texture;
	if (!bgfx::isValid(texture))
	{
		// NOTE: If an image only lives for two frames, then this will break
		texture = bgfx::createTexture2D(width, height, false, 1, format, 0, bgfx::makeRef(m_image_data.texture_data, width * height * channels));
		return true;
	}
	return false;
}

bool tiny::graphics::Image::load_data(TextureFormat dataFormat, void * data, size_t size)
{
	throw std::runtime_error("not implemented"); //assert(false && "Texture::load_data not yet implemented");
	return false;
}
