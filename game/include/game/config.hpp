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
			void write(Archive& ar)
			{
				ar(cereal::make_nvp("config", *this));
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
			WRITE_VAR(graphics);
		}

		SERIALIZE_METHOD(GameConfig::Graphics)
		{
			WRITE_VAR(fullscreen_mode);
			WRITE_VAR(width);
			WRITE_VAR(height);
		}
	}
}