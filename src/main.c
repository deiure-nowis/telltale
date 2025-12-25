#include <SDL2/SDL.h>
#include "isoWindow.h"
#include "isoGraphic.h"
#include "control.h"
#include "player.h"
#include "world.h"

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Isometric Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	initIsoOffsets();

	Player player;
	initPlayer(&player);

	World world;
	initWorld(&world);

	int running = 1;
	int hoverIsoX = -1, hoverIsoY = -1;

	while (running) {
		handleInput(&running, &player, &world);

		// Get current mouse state
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// Update hover tile
		if(getTileUnderMouse(mouseX, mouseY, &hoverIsoX, &hoverIsoY)){
			// Valid tile
		}else{
			hoverIsoX = hoverIsoY = -1;
		}

		updatePlayer(&player, &world);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
		SDL_RenderClear(renderer);

		drawScene(renderer, hoverIsoX, hoverIsoY, &player, &world);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
