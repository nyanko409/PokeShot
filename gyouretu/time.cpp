#include "time.h"
#include <string>
#include "sprite.h"
#include "enemy.h"
#include "scene_management.h"
#include "ball.h"


static Time timer(30, 1.0F);
static int remainingTime;
static int stringAlpha;

void InitTime()
{
	timer.reset();

	remainingTime = 20;
	stringAlpha = 0;
}

void UpdateTime()
{
	// update timer
	timer.update(slowmoFactor);

	if (timer.passed())
	{
		// reset timer and decrease remaining time
		timer.reset();

		stringAlpha = 255;
		remainingTime--;
	}

	// remaining time reaches 0 - game finish
	if (remainingTime <= 0)
	{
		currentScene = ResultScreen;
		return;
	}

	// decrease string alpha
	stringAlpha -= 20;
	if (stringAlpha < 0) stringAlpha = 0;
}

void DrawTime()
{
	if (remainingTime <= 0) return;

	// draw remaining time
	RECT rect;

	if (remainingTime >= 10)
	{
		SetRect(&rect, SCREEN_WIDTH / 2 - 20, 10, 30, 30);
		auto str = std::to_string(remainingTime);
		g_pFont->DrawTextA(NULL, str.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(120, 0, 0, 255));
	}
	else if(remainingTime < 10)
	{
		SetRect(&rect, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150, 150, 150);
		auto str = std::to_string(remainingTime);
		g_pBigFont->DrawTextA(NULL, str.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(stringAlpha, 128, 50, 255));
	}
}
