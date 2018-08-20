#pragma once
#include <glm/glm.hpp>
#include <graphics/image.hpp>

namespace tiny
{
	namespace graphics
	{
		class Sprite
		{
		public:
			Sprite();
			Sprite(const ImageHandle& image, const glm::vec4& bounds);
			Sprite(const Sprite& rhs);
			Sprite(Sprite&& rhs);

			Sprite& operator=(const Sprite& rhs);
			Sprite& operator=(Sprite&& rhs);

		public:
			glm::vec4 uv_bounds;
			ImageHandle image;
		};
	}
}