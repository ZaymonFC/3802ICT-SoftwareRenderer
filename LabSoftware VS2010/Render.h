#pragma once
#include <bitset>
#include "Colour.h"
#include "Point.h"

class Render
{
public:
	int FRAME_WIDE, FRAME_HIGH;

	using BYTE = unsigned char;

	Render(int width, int height);

	void SetPixel(BYTE* screen, Point point, Colour colour) const;

	void DrawLine_Dda(const Point p0, const Point p1, BYTE * screen) const;

	static int ClipTest(double p, double q, double * u1, double * u2);

	void DrawClipLine(Point p1, Point p2, BYTE * screen) const;

	static double Clamp(const double value, const double minimum, const double maximum);

	static double LinearLerp(const double start, const double end, const double gradient);

	void DrawScanLine(const int y, const Point pa, const Point pb, const Point pc, const Point pd, const Colour& leftColour,
	                  const Colour& rightColour, BYTE
	                  * screen) const;

	static double CrossProduct(const double x1, const double y1, const double x2, const double y2);

	static double LineSide2D(const Point p, const Point lineFrom, const Point lineTo);

	void DrawTriangle(Point p1, Point p2, Point p3, BYTE* screen) const;

	~Render() = default;
};

