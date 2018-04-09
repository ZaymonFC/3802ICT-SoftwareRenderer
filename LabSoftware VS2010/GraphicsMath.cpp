#include "GraphicsMath.h"
#include "Point.h"
#include <iostream>

/**
 * \param point Point to rotate
 * \param a Rotation around the x Axis
 * \param b Rotation around the y Axis
 * \param c Rotation around the z Axis
 * \return 
 */
Point GraphicsMath::RotatePoint(const Point& point, const float a, const float b, const float c)
{
	const auto x3 = point.x * (cos(b) * cos(c)) + point.z * sin(b) - point.y * (cos(b) * sin(c));
	const auto y3 = -point.z * (cos(b) * sin(a)) + point.x * (cos(c) * sin(a) * sin(b) + cos(a) * sin(c)) + point.y * (cos(a) * cos(c) - sin(a) * sin(b) * sin(c));
	const auto z3 = point.z * (cos(a) * cos(b)) + point.x * (sin(a) * sin(c) - cos(a) * cos(c) * sin(b)) + point.y * (cos(c) * sin(a) + cos(a) * sin(b) * sin(c));

//	std::cout << a << b << c;
	return {x3, y3, z3, point.colour};
}

Point GraphicsMath::ProjectionTransformPoint(std::vector<Point>::const_reference point, const int d, int frame_wide_, int frame_high_)
{
	const auto x = (((point.x - (frame_wide_ / 2.0f)) * d) / (point.z + d)) + (frame_wide_ / 2.0f);
	const auto y = (((point.y - (frame_high_ / 2.0f)) * d) / (point.z + d)) + (frame_high_ / 2.0f);
	return { x, y, point.z, point.colour };
}


float GraphicsMath::Clamp(const float value, const float minimum = 0, const float maximum = 1)
{
	return value > maximum ? 1 :
		value < minimum ? 0 : value;
}

auto GraphicsMath::Normalize(float value, float min, float max) -> float
{
	return (value - min) / (max - min);
}

float GraphicsMath::LinearLerp(const float start, const float end, float gradient)
{
	return start + (end - start) * Clamp(gradient);
}

float GraphicsMath::CrossProduct(const float x1, const float y1, const float x2, const float y2)
{
	return x1 * y2 - x2 * y1;
}

float GraphicsMath::CrossProduct(const Point v1, const Point v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

float GraphicsMath::LineSide2D(const Point p, const Point lineFrom, const Point lineTo)
{
	return CrossProduct(p.x - lineFrom.x, p.y - lineFrom.y, lineTo.x - lineFrom.x, lineTo.y - lineFrom.y);
}

float GraphicsMath::Convex2D(const Point origin, const Point pNext, const Point pPrev)
{
	const auto vectorA = Point(pNext.x - origin.x, pNext.y - origin.y);
	const auto vectorB = Point(pPrev.x - origin.x, pPrev.y - origin.y);
	
	return CrossProduct(vectorA, vectorB);
}

int GraphicsMath::ClipTest(float p, float q, float * u1, float * u2)
{
	const auto r = q / p;

	if (p < 0.0)
	{
		if (r > *u2) { return false; }
		if (r > * u1) {
			*u1 = r;
		}
	}
	else if (p > 0.0)
	{
		if (r < *u1) { return false; }
		if (r < *u2) {
			*u2 = r;
		}
	}
	// p == 0 so line is parallel to clip boundary
	else if (q < 0.0) {
		return false;
	}

	return true;
}

int GraphicsMath::SameSide(const Point a, const Point b, const Point l1, const Point l2)
{
	const auto abt = (a.x - l1.x) * (l2.y - l1.y) - (l2.x - l1.x) * (a.y - l1.y);
	const auto bpt = (b.x - l1.x) * (l2.y - l1.y) - (l2.x - l1.x) * (b.y - l1.y);

	return abt * bpt > 0;
}

bool GraphicsMath::PointInTriangle(Point p, Point a, Point b, Point c)
{
	return SameSide(p, a, b, c) && 
		   SameSide(p, b, a, c) && 
		   SameSide(p, c, a, b);
}

