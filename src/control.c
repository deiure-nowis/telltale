#include "control.h"
#include "isoWindow.h"
#include "world.h"
#include <stdio.h>
#include <math.h>

int screenToIsoGrid(int mouseX, int mouseY, int* isoX, int* isoY){
	float relX, relY;
	float srelX = (float)mouseX - WINDOW_WIDTH / 2.0f;
	float srelY = (float)mouseY - WINDOW_HEIGHT / 2.0f - BASE_HALF_TILE_HEIGHT;
	unprojectScreenToWorldRel(srelX, srelY, &relX, &relY);

	float wx = cameraIsoX + relX;
	float wy = cameraIsoY + relY;

	int ix = (int)roundf(wx);
	int iy = (int)roundf(wy);

	if(ix>=0 && ix<MAP_SIZE && iy>=0 && iy<MAP_SIZE){
		*isoX = ix;
		*isoY = iy;
		return 1;
	}
	return 0;
}

static Tile* getTileUnderCursor(World* world){
	int mx, my, tx, ty;
	SDL_GetMouseState(&mx, &my);
	if(screenToIsoGrid(mx, my, &tx, &ty))
		return &world->grid[ty][tx];
	return NULL;
}

void handleInput(int* running, Player* player, World* world){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			*running = 0;
		}else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT){
			int mouseX = event.button.x;
			int mouseY = event.button.y;
			int destX, destY;
			if(screenToIsoGrid(mouseX, mouseY, &destX, &destY))
				findPath(player, destX, destY, world);
		}else if(event.type == SDL_MOUSEWHEEL){
			if(event.wheel.y > 0){ // Wheel up: zoom in
				zoom *= 1.1f;
				if(zoom>MAX_ZOOM) zoom = MAX_ZOOM;
			}else if(event.wheel.y < 0){ // Wheel down: zoom out
				zoom /= 1.1f;
				if(zoom < MIN_ZOOM) zoom = MIN_ZOOM;
			}
		}else if(event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_SPACE:
					centerCameraOn((float)player->isoX, (float)player->isoY);
					break;
				case SDLK_z:
					rotation = (rotation - 90 + 360) % 360; // Counter-clockwise
					break;
				case SDLK_c:
					rotation = (rotation + 90) % 360; // Clockwise
					break;
				case SDLK_e:{
					Tile* t = getTileUnderCursor(world);
					if(t) t->variantX = (t->variantX + 1) % X_VARIANT;
					break;
				}
				case SDLK_r:{
					Tile* t = getTileUnderCursor(world);
					if(t) t->variantY = (t->variantY + 1) % Y_VARIANT;
					break;
				}
			}
		}
	}
}

void updateContinuousInput(float deltaTime){
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	float panSpeed = CAMERA_PAN_SPEED * deltaTime; // pixels per second at screen space
	float dx = 0.0f, dy = 0.0f;

	if(keyState[SDL_SCANCODE_W]) dy -= panSpeed;
	if(keyState[SDL_SCANCODE_S]) dy += panSpeed;
	if(keyState[SDL_SCANCODE_A]) dx -= panSpeed;
	if(keyState[SDL_SCANCODE_D]) dx += panSpeed;

	if(dx != 0.0f && dy != 0.0f){
		float len = sqrtf(dx*dx + dy*dy);
		dx = (dx / len) * panSpeed;
		dy = (dy / len) * panSpeed;
	}

	if(dx != 0.0f || dy != 0.0f){
		float worldDx, worldDy;
		unprojectScreenToWorldRel(dx, dy, &worldDx, &worldDy);
		cameraIsoX += worldDx;
		cameraIsoY += worldDy;
		clampCamera();
	}
}
