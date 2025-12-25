#include "control.h"
#include "isoWindow.h"
#include <stdio.h>
#include <math.h>

int getTileUnderMouse(int mouseX, int mouseY, int* isoX, int* isoY){
	// Reverse the offset and zoom
	int relX = (mouseX - offsetX) / zoom;
	int relY = (mouseY - offsetY - BASE_HALF_TILE_HEIGHT) / zoom;

	// Convert screen coordinates back to isometric grid
	// Formula derived from: screenX = (isoX - isoY) * HALF_TILE_WIDTH
	//                     screenY = (isoX + isoY) * HALF_TILE_HEIGHT

	float px = relX / (float)BASE_HALF_TILE_WIDTH;
	float py = relY / (float)BASE_HALF_TILE_HEIGHT;

	float gridX = (px + py) / 2.0f;
	float gridY = (py - px) / 2.0f;

	// Reverse rotation
	int centerX = GRID_SIZE / 2;
	int centerY = GRID_SIZE / 2;
	int rotGridX = (int)(gridX + 0.5f);
	int rotGridY = (int)(gridY + 0.5f);
	int relRotX = rotGridX - centerX;
	int relRotY = rotGridY - centerY;

	switch(rotation){
		case 0:   // No rotation
			*isoX = rotGridX;
			*isoY = rotGridY;
			break;
		case 90:  // 90° clockwise
			*isoX = centerX - relRotY;
			*isoY = centerY + relRotX;
			break;
		case 180: // 180°
			*isoX = centerX - relRotX;
			*isoY = centerY - relRotY;
			break;
		case 270: // 270° clockwise
			*isoX = centerX + relRotY;
			*isoY = centerY - relRotX;
			break;
	}

	// Check bounds
	if (*isoX < 0 || *isoX >= GRID_SIZE || *isoY < 0 || *isoY >= GRID_SIZE)
		return 0;

	return 1;
}

void handleInput(int* running, Player* player, const World* world){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			*running = 0;
		}else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
			int mouseX = event.button.x;
			int mouseY = event.button.y;
			int destX, destY;
			if(getTileUnderMouse(mouseX, mouseY, &destX, &destY))
				findPath(player, destX, destY, world);
		}else if(event.type == SDL_MOUSEWHEEL){
			if(event.wheel.y > 0){ // Wheel up: zoom in
				zoom = zoom * 1.1f;
				if(zoom>2.0f) zoom = 2.0f; // Max zoom
			}else if(event.wheel.y < 0){ // Wheel down: zoom out
				zoom = zoom / 1.1f;
				if(zoom < 0.5f) zoom = 0.5f; // Min zoom
			}
			updateIsoOffsets(); // Recenter after zoom
		}else if(event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_LEFT:
					rotation = (rotation - 90 + 360) % 360; // Counter-clockwise
					updateIsoOffsets();
					break;
				case SDLK_RIGHT:
					rotation = (rotation + 90) % 360; // Clockwise
					updateIsoOffsets();
					break;
			}
		}
	}
}
