#pragma once

#include <vector>
#include "Platform.h"

class Player;

// functions
void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
Player* GetPlayer();

// player class
class Player
{
private: 
	int jumpHeight;										// initial jump height when jump button is pressed
	int moveSpeed;										// move speed of player

	// internal use
	int width;											// player width in pixel
	int height;											// player height in pixel
	std::vector<Platform*> *platform;					// pointer to platforms

	bool isJumping;										// flag to check if player is jumping
	bool isGrounded;									// true if grounded
	int curJumpHeight;									// current jump height to add for realistic jump
	int gravity;										// gravity to add to velocity y

	int posX, posY;										// x and y position of player
	int oldX, oldY;										// position of player the frame before

	int velocityX, velocityY;							// current speed of player

	std::pair<int, int> uvWalk[5] = {};					// walk uv
	int uvWalkFrame;									// current uv frame to show

	std::pair<int, int> uvStand = {};					// stand uv

	bool isMoving;										// is true while moving
	bool isFacingLeft;									// is true when player is facing left

	void checkCollision();								// check if player is colliding with anything

public:			
	friend void DrawPlayer();							// allow private access only to drawPlayer

	Player(int jumpHeight = 40, int moveSpeed = 10);	// constructor
	~Player();											// destructor
	
	void UpdateUV();									// update uv for current frame
	void UpdatePhysics();								// player physics

	bool getGrounded() const { return isGrounded; }		// return true if grounded
	int getMoveSpeed() const { return moveSpeed; }		// return current movespeed
	int getPosX() const { return posX; }				// return x position
	int getPosY() const { return posY; }				// return y position
	int getOldX() const { return oldX; }				// return old x position
	int getOldY() const { return oldY; }				// return old y position
};
