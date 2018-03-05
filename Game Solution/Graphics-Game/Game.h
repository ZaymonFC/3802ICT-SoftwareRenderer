#pragma once
#include "RenderEngine.h"

class Game
{
private:
	int height_;
	int width_;
	RenderEngine * render_;
public:
	Game(int height, int width);
	~Game();
};

