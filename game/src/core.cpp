#include <game_core.hpp>
#include <graphics/image.hpp>
#include <Box2D/Box2D.h>

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

tiny::Game::Game(tiny::Platform* plat)
{
	active_memory = new GameMemory;
	
	// textures
	graphics::Image m_tex;
	m_tex.load("data/x.jpg");
}

tiny::Game::~Game()
{
	delete active_memory;
}

void tiny::Game::update(double gt, double dt)
{
	// This is where the core game loop goes
	while (active_memory->physicsTime < gt)
	{
		const double timestep = 1.0 / 60.0;
		active_memory->world.Step(float(timestep), 1, 1);
		active_memory->physicsTime += 1.0 / 60.0;
	}
}

void tiny::Game::render(double gt, double dt)
{

}