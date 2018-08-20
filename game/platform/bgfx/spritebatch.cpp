#include <algorithm>
#include <graphics/spritebatch.hpp>

tiny::graphics::Spritebatch::Spritebatch()
{
}

tiny::graphics::Spritebatch::~Spritebatch()
{
}

void tiny::graphics::Spritebatch::submit(const Sprite & sprite, const glm::vec2 & pos, float rot, int level)
{
	SpriteRenderCommand cmd;
	cmd.sprite = sprite;
	cmd.pos = pos;
	cmd.rot = rot;

	if (levels.count(level) == 0)
		levels.insert({level, std::vector<SpriteRenderCommand>()});

	levels[level].push_back(cmd);
}

void tiny::graphics::Spritebatch::render()
{
	scratch_levels.clear();
	for (auto& it : levels) {
		scratch_levels.push_back(it.first);
	}

	std::sort(scratch_levels.begin(), scratch_levels.end());
	for (auto& it : scratch_levels) {
		render_level(it);
	}
}

void tiny::graphics::Spritebatch::render_level(int level) {

}
