#pragma once
#include <bitset>
#include "Colour.h"
#include "Point.h"
#include "GraphicsMath.h"
#include <vector>

class Face;

class Render
{
public:
	int FRAME_WIDE, FRAME_HIGH;
	using BYTE = unsigned char;
	BYTE * _screen;

	Render(int width, int height, BYTE * screen);

	void SetPixel(Point point, Colour colour) const;

	void DrawLine_Dda(const Point p0, const Point p1) const;

	void DrawClipLine(Point p1, Point p2) const;

	void DrawScanLine(const int y, const Point pa, const Point pb, const Point pc, const Point pd, const Colour& leftColour,
	                  const Colour& rightColour) const;

	void DrawTriangle(Point p1, Point p2, Point p3) const;

	void DrawPolygon(const std::vector<Point>& points) const;

	void DrawTriangle(Face face) const;

	~Render() = default;
};

