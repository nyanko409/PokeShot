#pragma once

enum Scene
{
	MainMenuScreen,
	GameScreen,
	ResultScreen
};

extern Scene currentScene;

void UpdateMainScreen();
void DrawMainScreen();

void UpdateResultScreen();
void DrawResultScreen();
