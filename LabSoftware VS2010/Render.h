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

	Render(int width, int height);

	void SetPixel(BYTE* screen, Point point, Colour colour) const;

	void DrawLine_Dda(const Point p0, const Point p1, BYTE* screen) const;

	void DrawClipLine(Point p1, Point p2, BYTE* screen) const;

	void DrawScanLine(const int y, const Point pa, const Point pb, const Point pc, const Point pd, const Colour& leftColour,
	                  const Colour& rightColour, BYTE* screen) const;

	void DrawTriangle(Point p1, Point p2, Point p3, BYTE* screen) const;

	void DrawPolygon(const std::vector<Point>& points, BYTE* screen) const;

	void DrawTriangle(Face face, BYTE * screen) const;

	~Render() = default;
};

