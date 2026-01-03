#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	GameAssets assets;
	initAssets(renderer, &assets);

	Player player;
	initPlayer(&player);

	World world;
	initWorld(&world);

	centerCameraOn((float)player.isoX, (float)player.isoY);

	int running = 1;
	int hoverIsoX = -1, hoverIsoY = -1;
	Uint64 prevTime = SDL_GetTicks64();

	while (running) {
		Uint64 currTime = SDL_GetTicks64();
		float deltaTime = (float)((currTime - prevTime) / 1000.0);
		if(deltaTime > 0.1f) deltaTime = 0.1f; // Cap max dt
		prevTime = currTime;

		handleInput(&running, &player, &world);
		updateContinuousInput(deltaTime);

		// Mouse hover update
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		if(screenToIsoGrid(mouseX, mouseY, &hoverIsoX, &hoverIsoY)){
			// Valid tile
		}else{
			hoverIsoX = hoverIsoY = -1;
		}

		updatePlayer(&player, &world);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
		SDL_RenderClear(renderer);

		updateTileAnimations(deltaTime, &world);

		drawScene(renderer, hoverIsoX, hoverIsoY, &player, &world, &assets);

		SDL_RenderPresent(renderer);
		SDL_Delay(8);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	destroyAssets(&assets);
	SDL_Quit();
	return 0;
}
