#pragma once
#include <vector>

class Point;

class GraphicsMath
{
public:
	static Point RotatePoint(const Point& point, float a, float b, float c);

	static float Clamp(float value,  float minimum,  float maximum);
	static float LinearLerp(float start, float end, float gradient);
	static float CrossProduct( float x1,  float y1,  float x2,  float y2);
	static int ClipTest(float p, float q, float* u1, float* u2);
	static bool PointInTriangle(Point p, Point a, Point b, Point c);
	static float CrossProduct( Point v1,  Point v2);
	static float LineSide2D( Point p,  Point lineFrom,  Point lineTo);
	static float Convex2D( Point origin,  Point pNext,  Point pPrev);
	static int SameSide(Point a, Point b, Point l1, Point l2);
};

