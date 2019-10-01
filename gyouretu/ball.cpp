#include "sprite.h"
#include "ball.h"
#include "common.h"
#include "input.h"
#include "sound.h"


Ball::Ball() : tex(TEXTURE_INDEX_BALL), position(v2i{ 200, 300 }), xVel(0), yVel(0)
{
	
}

Ball::~Ball()
{

}



Ball ball;
float slowmoFactor;
bool isDragging;
v2i startPos, releasePos, currentPos;
bool isPressed, startPosSaved, releaseDone;
bool sfxPlaying;


Ball* GetBall()
{
	return &ball;
}

void InitBall()
{
	ball = Ball();
	releaseDone = true;
	isDragging = false;
	slowmoFactor = 1.0F;

	isPressed = false;
	startPosSaved = false;
	sfxPlaying = false;

	startPos = {}, releasePos = {}, currentPos = {};
}

void UninitBall()
{

}

void UpdateBall()
{
	// update slow-mo factor
	if (isPressed)
	{
		slowmoFactor -= .15F;
		if (slowmoFactor < .05F) slowmoFactor = .05F;
	}
	else
	{
		slowmoFactor += .3F;
		if (slowmoFactor > 1.0F) slowmoFactor = 1.0F;
	}

	// left mouse is pressed
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		isPressed = true;
		isDragging = true;
		releaseDone = false;

		// play sfx
		if (!sfxPlaying)
		{
			StopSound(SOUND_LABEL_SLOWMO_END);
			PlaySound(SOUND_LABEL_SLOWMO_START);
			sfxPlaying = true;
		}

		POINT p = POINT();
		GetCursorPos(&p);

		// init click position
		if (!startPosSaved)
		{
			startPos.x = p.x;
			startPos.y = p.y;
			startPosSaved = true;
		}

		// update current pos
		currentPos.x = p.x;
		currentPos.y = p.y;
	}
	else isPressed = false;

	// player released mouse button, get release pos and add to velocity
	if (!isPressed && !releaseDone)
	{
		releaseDone = true;
		isDragging = false;
		startPosSaved = false;

		// stop start sfx and start end sfx
		sfxPlaying = false;
		StopSound(SOUND_LABEL_SLOWMO_START);
		PlaySound(SOUND_LABEL_SLOWMO_END);

		// get cursor pos at release location
		POINT p = POINT();
		if (GetCursorPos(&p))
		{
			releasePos.x = p.x;
			releasePos.y = p.y;
		}

		// direction to shoot
		v2i diff = startPos - releasePos;

		// multiply by speed factor
		diff *= .15F;

		// set x and y speed
		ball.setXVel(diff.x);
		ball.setYVel(diff.y);
	}

	// add velocity to ball
	ball.addPos(ball.getXVel() * slowmoFactor, ball.getYVel() * slowmoFactor);

	// keep ball in screen, reverses direction
	if (ball.getPosition().x < 0)
	{
		PlaySound(SOUND_LABEL_BALL_BOUNCE);
		ball.reverseXDir();
		ball.setPos(0, ball.getPosition().y);
	}
	else if (ball.getPosition().x + Texture_GetWidth(ball.getTexture()) > SCREEN_WIDTH)
	{
		PlaySound(SOUND_LABEL_BALL_BOUNCE);
		ball.reverseXDir();
		ball.setPos(SCREEN_WIDTH - Texture_GetWidth(ball.getTexture()), ball.getPosition().y);
	}
	if (ball.getPosition().y < 0)
	{
		PlaySound(SOUND_LABEL_BALL_BOUNCE);
		ball.reverseYDir();
		ball.setPos(ball.getPosition().x, 0);
	}
	else if (ball.getPosition().y + Texture_GetHeight(ball.getTexture()) > SCREEN_HEIGHT)
	{
		PlaySound(SOUND_LABEL_BALL_BOUNCE);
		ball.reverseYDir();
		ball.setPos(ball.getPosition().x, SCREEN_HEIGHT - Texture_GetHeight(ball.getTexture()));
	}
}

void DrawBall()
{
	TextureIndex tex = ball.getTexture();

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(tex, ball.getPosition().x, ball.getPosition().y, 0, 0, Texture_GetWidth(tex), Texture_GetHeight(tex));
}
