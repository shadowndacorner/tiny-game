#pragma once
#include <cereal/cereal.hpp>
#include <serialization/helpers.hpp>

namespace tiny
{
	namespace game
	{
		namespace FullscreenMode
		{
			enum Enum : uint8_t
			{
				Windowed,
				Borderless,
				Fullscreen
			};
		}

		struct GameConfig
		{
			template <typename Archive>
			bool archive(Archive& ar)
			{
				try
				{
					ar(cereal::make_nvp("config", *this));
				}
				catch (cereal::Exception& exc)
				{
					std::cerr << "Failed to load config" << std::endl;
					return false;
				}
				return true;
			}

			struct Graphics
			{
				inline Graphics()
				{
					fullscreen_mode = (FullscreenMode::Enum)0;
					width = 1280;
					height = 720;
				}

				FullscreenMode::Enum fullscreen_mode;
				int width;
				int height;
			};

			Graphics graphics;
		};

		SERIALIZE_METHOD(GameConfig)
		{
			SERIALIZE_VAR(graphics);
		}

		SERIALIZE_METHOD(GameConfig::Graphics)
		{
			SERIALIZE_VAR(fullscreen_mode);
			SERIALIZE_VAR(width);
			SERIALIZE_VAR(height);
		}
	}
}