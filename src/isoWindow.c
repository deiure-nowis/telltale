#include "isoWindow.h"

int offsetX = 0;
int offsetY = 0;
float zoom = 1.0f; // Default zoom level
int rotation = 0;

void getIsoScreenPos(int isoX, int isoY, int* screenX, int* screenY){
	// Apply rotation to get rotated isometric coordinates
	int rotX = 0, rotY = 0;
	getRotatedIsoPos(isoX, isoY, &rotX, &rotY);

	// Standard isometric projection, scaled by zoom
	*screenX = (rotX - rotY) * (BASE_HALF_TILE_WIDTH * zoom);
	*screenY = (rotX + rotY) * (BASE_HALF_TILE_HEIGHT * zoom);
}

void getRotatedIsoPos(int isoX, int isoY, int* rotX, int* rotY){
	// Center the grid around (7,7) for rotation
	int centerX = GRID_SIZE / 2;
	int centerY = GRID_SIZE / 2;
	int relX = isoX - centerX;
	int relY = isoY - centerY;

	// Rotate based on current rotation (0, 90, 180, 270)
	switch(rotation){
		case 0:   // No rotation
			*rotX = isoX;
			*rotY = isoY;
			break;
		case 90:  // 90° clockwise
			*rotX = centerX + relY;
			*rotY = centerY - relX;
			break;
		case 180: // 180°
			*rotX = centerX - relX;
			*rotY = centerY - relY;
			break;
		case 270: // 270° clockwise
			*rotX = centerX - relY;
			*rotY = centerY + relX;
			break;
	}
}

void initIsoOffsets(void){
	updateIsoOffsets();
}

void updateIsoOffsets(void){
	int centerX, centerY;
	getIsoScreenPos(GRID_SIZE / 2, GRID_SIZE / 2, &centerX, &centerY);
	offsetX = WINDOW_WIDTH / 2 - centerX;
	offsetY = WINDOW_HEIGHT / 2 - centerY;
}
