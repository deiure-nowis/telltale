#include "isoWindow.h"
#include "world.h"
#include <math.h>

float zoom = 1.0f; // Default zoom level
int rotation = 0;
float cameraIsoX = 0.0f;
float cameraIsoY = 0.0f;

void projectWorldToScreen(float worldX, float worldY, int* screenX, int* screenY){
	float relX = worldX - cameraIsoX;
	float relY = worldY - cameraIsoY;

	float rot_relX, rot_relY;
	switch(rotation){
		case 0:
			rot_relX = relX;
			rot_relY = relY;
			break;
		case 90:
			rot_relX = relY;
			rot_relY = -relX;
			break;
		case 180:
			rot_relX = -relX;
			rot_relY = -relY;
			break;
		case 270:
			rot_relX = -relY;
			rot_relY = relX;
			break;
		default:
			rot_relX = relX;
			rot_relY = relY;
			break;
	}

	float projX = (rot_relX - rot_relY) * BASE_HALF_TILE_WIDTH *zoom;
	float projY = (rot_relX + rot_relY) * BASE_HALF_TILE_HEIGHT *zoom;

	*screenX = (int)(projX + 0.5f + WINDOW_WIDTH / 2.0f);
	*screenY = (int)(projY + 0.5f + WINDOW_HEIGHT / 2.0f + 0.5f);
}

void unprojectScreenToWorldRel(float screenRelX, float screenRelY, float* relX, float* relY){
	float px = screenRelX / (BASE_HALF_TILE_WIDTH * zoom);
	float py = screenRelY / (BASE_HALF_TILE_HEIGHT * zoom);

	float isoX = (px + py) / 2.0f;
	float isoY = (py - px) / 2.0f;

	switch(rotation){
		case 0:
			*relX = isoX;
			*relY = isoY;
			break;
		case 90:
			*relX = -isoY;
			*relY = isoX;
			break;
		case 180:
			*relX = -isoX;
			*relY = -isoY;
			break;
		case 270:
			*relX = isoY;
			*relY = -isoX;
			break;
		default:
			*relX = isoX;
			*relY = isoY;
			break;
		
	}
}

void centerCameraOn(float worldX, float worldY){
	cameraIsoX = worldX;
	cameraIsoY = worldY;
	clampCamera();
}

void clampCamera(void){
	float half = VISIBLE_GRID_SIZE / 2.0f;
	cameraIsoX = fmaxf(half, fminf((float)MAP_SIZE - half, cameraIsoX));
	cameraIsoY = fmaxf(half, fminf((float)MAP_SIZE - half, cameraIsoY));
}
