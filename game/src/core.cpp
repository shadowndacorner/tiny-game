#include <fstream>
#include <Box2D/Box2D.h>
#include <game_core.hpp>
#include <game/config.hpp>
#include <graphics/image.hpp>
#include <cereal/archives/json.hpp>
#include <graphics/spritebatch.hpp>

static tiny::Game* active_game = nullptr;
namespace tiny
{
	struct GameMemory
	{
		GameMemory() : world(b2Vec2(0, -9.8f)), physicsTime(0)
		{

		}

		~GameMemory()
		{

		}

		b2World world;
		double physicsTime;
	};
}

tiny::Game* tiny::Game::get_active_game()
{
	return active_game;
}

bool file_exist(const char* filename)
{
	struct stat	buffer;
	return (stat(filename, &buffer) == 0);
}

#include <miniz.h>
tiny::Game::Game(tiny::Platform* plat)
{
	active_memory = new GameMemory;
	
	tiny::game::GameConfig cfg;
	{
		if (!file_exist("config.json"))
		{
			std::fstream stream("config.json", std::ios::binary | std::ios::out);
			cereal::JSONOutputArchive ar(stream);
			cfg.archive(ar);
		}
		else
		{
			std::fstream stream("config.json", std::ios::binary | std::ios::in);
			cereal::JSONInputArchive ar(stream);
			cfg.archive(ar);
		}
	}

	// textures
	graphics::Image m_tex;
	m_tex.load("data/twitter avatar.png");
}

tiny::Game::~Game()
{
	delete active_memory;
}

void tiny::Game::update(double gt, double dt)
{
	while (active_memory->physicsTime < gt)
	{
		// call physics pre-tick code
		const double timestep = 1.0 / 60.0;
		active_memory->world.Step(float(timestep), 1, 1);
		active_memory->physicsTime += 1.0 / 60.0;
		
		// call physics tick code
	}
	
	// call system update code

}

void tiny::Game::render(double gt, double dt)
{
	// iterate through every object and send to sprite batch
}