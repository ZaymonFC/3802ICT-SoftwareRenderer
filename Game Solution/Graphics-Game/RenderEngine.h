#pragma once
#include <bitset>
#include "Point.h"
#include "Colour.h"


class RenderEngine
{
private:
	int height_;
	int width_;
	typedef std::bitset<8> byte;
	byte * _frame;

public:
	RenderEngine(int height, int width);
	~RenderEngine();
	void Render();

private:
	void InitGlut();
	void ClearScreen();
	void DrawFrame();
	void OnIdle();
	void OnDisplay();
	void BuildFrame();
	
	void OnMouse(int button, int state, int x, int y);
	void OnKeypress(unsigned char key, int x, int y);

	void SetPixel(Point p, Colour c);
	void DrawLine(Point p0, Point p1);



};

