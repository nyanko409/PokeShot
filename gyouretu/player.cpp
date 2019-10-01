#include <iostream>
#include "player.h"
#include "texture.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include "Platform.h"
#include "camera.h"
#include "common.h"


Player::Player(int jumpHeight, int moveSpeed) : jumpHeight(jumpHeight), moveSpeed(moveSpeed), gravity(10), platform(GetPlatform())
{
	// cache width and height
	int width = Texture_GetWidth(TEXTURE_INDEX_SPRITESHEET_PLAYER_LEFT);
	int height = Texture_GetHeight(TEXTURE_INDEX_SPRITESHEET_PLAYER_LEFT);

	// set start pos
	oldX = posX = SCREEN_WIDTH / 2;
	oldY = posY = SCREEN_HEIGHT / 2 + 50;

	// init player width and height
	this->width = width / 8;
	this->height = height / 8;

	// init walk uv
	uvWalk[0] = std::make_pair((width / 8) * 4, (height / 8) * 1);
	uvWalk[1] = std::make_pair((width / 8) * 4, (height / 8) * 2);
	uvWalk[2] = std::make_pair((width / 8) * 4, (height / 8) * 3);
	uvWalk[3] = std::make_pair((width / 8) * 4, (height / 8) * 4);
	uvWalkFrame = 0;

	// init stand uv
	uvStand = std::make_pair((width / 8) * 4, (height / 8) * 5);
}

Player::~Player()
{
	// free memory
}

void Player::UpdateUV()
{
	uvWalkFrame++;
	if (uvWalkFrame > 7)
		uvWalkFrame = 0;
}

void Player::checkCollision()
{
	// check collision for each platform
	for (int i = 0; i < platform->capacity(); i++)
	{
		isGrounded = false;

		int block_width = (*(*platform)[i]).getWidth();
		int block_width_pixel = block_width * (*(*platform)[i]).getWidthPixel();
		int block_height_pixel = (*(*platform)[i]).getHeightPixel();
		int block_x = (*(*platform)[i]).getX();
		int block_y = (*(*platform)[i]).getY();

		if ((posY + height > block_y + 1 && posY + height < block_y + block_height_pixel - 1) ||
			(posY + 50 > block_y + 1 && posY + 50 < block_y + block_height_pixel - 1))
		{
			if (posX + width > block_x && posX + width < block_x + 20)
			{
				// left side
				posX = block_x - width;
				return;
			}
			if (posX > block_x + block_width_pixel - 20 && posX < block_x + block_width_pixel)
			{
				// right side
				posX = block_x + block_width_pixel;
				return;
			}
		}

		// from top / bot
		if (posX + width > block_x && posX < block_x + block_width_pixel)
		{
			if (posY + height > block_y && posY + 50 < block_y + block_height_pixel)
			{
				if (velocityY > 0)
				{
					// collision from top
					isJumping = false;
					isGrounded = true;
					curJumpHeight = 0;
					posY = block_y - height;
					return;
				}

				if (velocityY < 0)
				{
					// collision from bottom
					curJumpHeight = 0;
					posY = block_y + block_height_pixel - 50;
					return;
				}
			}
		}
	}
}

void Player::UpdatePhysics()
{
	// reset every frame
	isMoving = false;
	velocityX = 0;
	velocityY = 0;
	oldX = posX;
	oldY = posY;

	// move left and right
	if (Keyboard_IsPress(DIK_A))
	{
		velocityX -= moveSpeed;
		isMoving = true;
	}

	if (Keyboard_IsPress(DIK_D))
	{
		velocityX += moveSpeed;
		isMoving = true;
	}

	if (velocityX > 0) isFacingLeft = false;
	if (velocityX < 0) isFacingLeft = true;
	if (velocityX == 0) isMoving = false;

	// jump
	if (!isJumping && Keyboard_IsTrigger(DIK_SPACE) && isGrounded)
	{
		isJumping = true;
		curJumpHeight = jumpHeight;
	}

	if (isJumping || !isGrounded)
	{
		curJumpHeight -= 3;
		if (curJumpHeight < -10) curJumpHeight = -10;

		velocityY -= curJumpHeight;
	}

	// gravity
	velocityY += gravity;

	// update player pos
	posX += velocityX;
	posY += velocityY;

	// check platform collision
	checkCollision();
}
