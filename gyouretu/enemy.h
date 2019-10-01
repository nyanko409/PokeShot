#pragma once

#include "texture.h"
#include "ball.h"
#include "common.h"

#define MAX_ENEMIES 2


class Enemy
{
private:
	TextureIndex tex;
	v2i position;
	float scale;
	bool flip;
	bool isActive;

public:
	friend void InitEnemy();
	friend void UpdateEnemy();
	friend void DrawEnemy();
	friend void SetEnemy();
	friend bool CheckCollision(Enemy enemy);

	Enemy();
	Enemy(TextureIndex tex, v2i pos = {0, 0}, float scale = 0);
	~Enemy();

	TextureIndex getTexture() { return tex; }

	void UpdateScale();
};

extern int score;

void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
void DrawEnemy();
void SetEnemy();
v2i GetRandomPos(int width, int height);
bool CheckCollision(Enemy enemy);
