#pragma once

#include <math.h>
#include "texture.h"


struct v2i
{
	int x;
	int y;
	
	int getMagnitude() { return sqrt(x * x + y * y); }

	v2i operator + (v2i ot) { return v2i{x + ot.x, y + ot.y}; }
	v2i operator - (v2i ot) { return v2i{ x - ot.x, y - ot.y }; }
	void operator += (v2i ot) { x += ot.x; y += ot.y; };
	void operator -= (v2i ot) { x -= ot.x; y -= ot.y; };
	void operator *= (float factor) { x *= factor; y *= factor; }
};

class Ball
{
public:
	Ball();
	~Ball();

	TextureIndex getTexture() const { return tex; }
	v2i getPosition() const { return position; }

	void reverseXDir() { xVel *= -1; }
	void reverseYDir() { yVel *= -1; }

	int getXVel() const { return xVel; }
	int getYVel() const { return yVel; }
	void setXVel(int x) { xVel = x; }
	void setYVel(int y) { yVel = y; }
	void addXVel(int x) { xVel += x; }
	void addYVel(int y) { yVel += y; }

	void setPos(int x, int y) { position.x = x; position.y = y; }
	void addPos(int x, int y) { position.x += x; position.y += y; }

private:
	TextureIndex tex;
	v2i position;
	int xVel, yVel;
};


extern float slowmoFactor;
extern bool isDragging;
extern v2i currentPos;
extern v2i startPos;
extern v2i releasePos;

void InitBall();
void UninitBall();
void UpdateBall();
void DrawBall();
Ball* GetBall();
