#include "enemy.h"
#include "sprite.h"
#include <time.h>
#include <math.h>
#include <string>
#include <queue>
#include "transform.h"
#include "sound.h"



Enemy::Enemy(TextureIndex tex, v2i pos, float scale) : tex(tex), position(pos), isActive(true), scale(scale)
{
	flip = false;
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::UpdateScale()
{
	if (isActive)
	{
		if (!flip)
		{
			scale += 0.1F;
			if (scale > 1.2F)
			{
				scale = 1.2F;
				flip = true;
			}
		}
		else if (scale > 1.0F)
		{
			scale -= 0.05F;
			if (scale < 1.0F) scale = 1.0F;
		}
	}
	else
	{
		if (flip)
		{
			scale += 0.1F;
			if (scale > 1.2F)
			{
				scale = 1.2F;
				flip = false;
			}
		}
		else if (scale > 0.0F)
		{
			scale -= 0.1F;
			if (scale < 0.0F) scale = 0.0F;
		}
	}
}

using std::queue;


queue<Enemy> enemies;
static int drawCount;

Ball* ball;
int score;
static int stringAlpha;
Enemy enemyCache;


void InitEnemy()
{
	// init
	enemies = {};
	enemyCache = {};
	ball = GetBall();
	score = 0;
	srand(time(NULL)); 
	
	SetEnemy();
	SetEnemy();
}

void UninitEnemy()
{

}

void UpdateEnemy()
{
	enemies.front().UpdateScale();
	enemies.back().UpdateScale();

	enemyCache.UpdateScale();

	// enemy collided with player
	if(CheckCollision(enemies.front()))
	{
		// play sound
		PlaySound(SOUND_LABEL_ENEMY_HIT);

		// remove first in queue
		enemyCache = enemies.front();
		enemyCache.isActive = false;
		enemies.pop();

		// add enemy to queue
		SetEnemy();

		// add score and reset alpha
		score++;
		stringAlpha = 255;

		// simulate hit stop
		Sleep(50);
	}
}

void SetEnemy()
{
	// get random pos and add offset
	TextureIndex tex = TEXTURE_INDEX_ENEMY;
	v2i random = GetRandomPos(SCREEN_WIDTH - Texture_GetWidth(tex) - 40, SCREEN_HEIGHT - Texture_GetHeight(tex) - 20);

	// create new enemy at position
	enemies.push(Enemy(tex, random));
}

void DrawEnemy()
{
	// init draw count and temp queue
	queue<Enemy> temp = enemies;
	drawCount = enemies.size();
	if (enemies.size() < 2) drawCount = 2;

	// draw enemies
	Ato::ScaleFromCenter(true);
	for (int i = 0; i < drawCount; i++)
	{

		Enemy en = temp.front();
		TextureIndex tex = en.getTexture();

		Ato::SetScale(en.scale);
		if (i == 0) Ato::SetColor(D3DCOLOR_RGBA(0, 255, 0, 255));
		else Ato::SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));

		Ato::DrawSprite(tex, en.position.x, en.position.y, Texture_GetWidth(tex), Texture_GetHeight(tex), 
						0.0F, 0.0F, 1.0F, 1.0F);

		temp.pop();
	}


	// draw enemy hit animation
	Ato::SetScale(enemyCache.scale);
	Ato::DrawSprite(enemyCache.tex, enemyCache.position.x, enemyCache.position.y, Texture_GetWidth(enemyCache.tex), Texture_GetHeight(enemyCache.tex),
		0.0F, 0.0F, 1.0F, 1.0F);

	// draw score
	if (score > 0)
	{
		int xOffset = score < 10 ? 30 : 20;

		RECT rect;
		SetRect(&rect, enemyCache.position.x + xOffset, enemyCache.position.y + 20, 30, 30);

		stringAlpha -= 5;
		if (stringAlpha < 0) stringAlpha = 0;

		auto str = std::to_string(score);
		g_pFont->DrawTextA(NULL, str.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(stringAlpha, 0, 0, 0));
	}

}

v2i GetRandomPos(int width, int height)
{	
	v2i temp = { rand() / ((RAND_MAX + 1u) / width),
				 rand() / ((RAND_MAX + 1u) / height) };
	
	return temp;
}

bool CheckCollision(Enemy enemy)
{
	v2i ballPos = ball->getPosition();
	v2i ballSize = v2i{ Texture_GetWidth(ball->getTexture()), Texture_GetHeight(ball->getTexture()) };
	v2i enemySize = v2i{ Texture_GetWidth(enemy.getTexture()), Texture_GetHeight(enemy.getTexture()) };


	if (ballPos.x + ballSize.x > enemy.position.x && ballPos.x < enemy.position.x + enemySize.x)
	{
		if (ballPos.y + ballSize.y > enemy.position.y && ballPos.y < enemy.position.y + enemySize.y)
		{
			return true;
		}
	}

	return false;
}
