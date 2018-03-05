#include "stdafx.h"
#include "Game.h"
#include "RenderEngine.h"

Game::Game(int height, int width)
{

	height_ = height;
	width_ = width;
	render_ = new RenderEngine(height_, width_);
}

Game::~Game()
{
	delete render_;
}



