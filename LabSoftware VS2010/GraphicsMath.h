#pragma once
#include <vector>

class Point;

class GraphicsMath
{
public:
	// Point transformations
	static auto RotatePoint(const Point& point, float a, float b, float c) -> Point;
	static auto ProjectionTransformPoint(std::vector<Point>::const_reference point, int d, int frame_wide_, int frame_high_) -> Point;

	// Interpolation
	static float LinearLerp(float start, float end, float gradient);
	static float Clamp(float value,  float minimum,  float maximum);

	// Normaliztion
	static auto Normalize(float value, float min, float max) -> float;

	// Liang Barski Helpers
	static int   ClipTest(float p, float q, float* u1, float* u2);

	// General Maths
	static float CrossProduct( float x1,  float y1,  float x2,  float y2);
	static float CrossProduct( Point v1,  Point v2);

	// Line Side
	static float LineSide2D( Point p,  Point lineFrom,  Point lineTo);

	// Polygon Decomposition / Back Face Culling Helpers
	static bool  PointInTriangle(Point p, Point a, Point b, Point c);
	static float Convex2D( Point origin,  Point pNext,  Point pPrev);
	static int   SameSide(Point a, Point b, Point l1, Point l2);

	// Conversions
	static constexpr float Degrees(const float f)
	{
		return  f * (3.141592653589793238462643383279502884f / 180.0f);
	}
};

