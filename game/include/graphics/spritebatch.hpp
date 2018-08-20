#pragma once
#include <unordered_map>
#include <graphics/sprite.hpp>
#include <containers/vector.hpp>

namespace tiny
{
	namespace graphics
	{
		class Spritebatch
		{
			struct SpriteRenderCommand {
				Sprite sprite;
				glm::vec2 pos;
				float rot;
			};

		public:
			Spritebatch();
			~Spritebatch();

		public:
			void submit(const Sprite& sprite, const glm::vec2& pos, float rot, int level=0);
			void render();
			void render_level(int level);

		public:
			std::unordered_map<int, tiny::vector<SpriteRenderCommand>> levels;
			std::vector<int> scratch_levels;
		};
	}
}