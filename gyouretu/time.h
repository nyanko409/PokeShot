#pragma once

struct Time
{
	float time;
	float current;
	float inc_per_frame;
	
	Time(float time, float inc_per_frame) : time(time), inc_per_frame(inc_per_frame), current(0) {};
	bool passed() { return (time - current) < 0; }
	void reset() { current = 0; }
	void update(float factor = 1.0F) { current += inc_per_frame * factor; }
};

void InitTime();
void UpdateTime();
void DrawTime();
