#pragma once

#include <vector>
#include <iostream>

class Platform;

// functions
void InitPlatform();
void UninitPlatform();
void UpdatePlatform();
void DrawPlatform();
void SetPlatform(int posX, int posY, int width = 1);
std::vector<Platform*>* GetPlatform();


// platform class
class Platform
{
private:
	int width;												// width of the platform
	int posX, posY;											// position of the platform
	int widthPixel, heightPixel;							// width and height in pixel of a single platform
	std::pair<int, int> *uvPos;								// uv pos of the platform based on given texture

	void initUV();											// init uvPos array

public:
	Platform(int posX, int posY, int width = 1);			// constructor
	~Platform();											// destructor

	int getX() { return posX; }								// return posX
	void setX(int x) { posX = x; }							// set posX
	int getY() { return posY; }								// return posY
	void setY(int y) { posY = y; }							// set posY
															
	int getWidthPixel() { return widthPixel; }				// return width in pixel per platform
	int getHeightPixel() { return heightPixel; }			// return height in pixel per platform
															
	int getWidth() { return width; }						// return the width of the platform
	auto* getUVPos() { return uvPos; };						// re
};
