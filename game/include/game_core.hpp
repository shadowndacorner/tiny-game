#pragma once
#include <forward_decl/platform.hpp>
namespace tiny
{
	// Heap allocated segment of memory which 
	// contains all game data.
	// Allocated on the heap to guarantee it
	// doesn't exist as static data.
	struct GameMemory
	{

	};

	class Game
	{
	public:
		static Game* get_active_game();

	public:
		Game(Platform*);
		~Game();

	public:
		void update(double gt, double dt);
		void render(double gt, double dt);

	public:
		GameMemory* active_memory;
	};
}