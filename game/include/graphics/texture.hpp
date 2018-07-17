#pragma once
#include <containers/smart_ptr.hpp>
#include <containers/string.hpp>
#include <graphics/graphics_enums.hpp>

namespace tiny
{
	namespace graphics
	{
		namespace internal
		{
			struct PlatformSpecificTextureData;
		}

		class Texture
		{
			struct image_data
			{
			public:
				image_data();
				image_data(image_data&& rhs);
				image_data(const image_data& rhs) = delete;
				~image_data();

			public:
				void* load(FILE* file, int& w, int& h, int& channels, int desired_channels = 0);
				void unload();

			public:
				void* texture_data;
			};

		public:
			Texture();
			Texture(Texture&&);
			Texture(const Texture&) = delete;
			~Texture();

		public:
			bool is_valid() const;

			// todo: this is temporary, it should be set up to just load texture data
			// the resource system should handle actually loading the data and then
			// call loaddata or whatever
			bool load(const tiny::string& path);
			bool load_data(TextureFormat dataFormat, void* data, size_t size);

		public:
			internal::PlatformSpecificTextureData* platform_specific_data;
			TextureFormat m_internal_format;
			image_data m_image_data;
		};

		// for now, handles are just shared ptrs
		typedef tiny::shared_ptr<Texture> TextureHandle;
	}
}