#include <game_core.hpp>

static tiny::Game* active_game = nullptr;

tiny::Game* tiny::Game::get_active_game()
{
	return active_game;
}

tiny::Game::Game(tiny::Platform* plat)
{
	active_memory = new GameMemory;
}

tiny::Game::~Game()
{
	delete active_memory;
}

void tiny::Game::update(double gt, double dt)
{
	// This is where the core game loop goes
}

void tiny::Game::render(double gt, double dt)
{

}