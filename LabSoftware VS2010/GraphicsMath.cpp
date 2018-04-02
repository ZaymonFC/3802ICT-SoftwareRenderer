#include "GraphicsMath.h"
#include "Point.h"

double GraphicsMath::Clamp(const double value, const double minimum = 0, const double maximum = 1)
{
	return value > maximum ? 1 :
		value < minimum ? 0 : value;
}

double GraphicsMath::LinearLerp(const double start, const double end, const double gradient)
{
	return start + (end - start) * Clamp(gradient);
}

double GraphicsMath::CrossProduct(const double x1, const double y1, const double x2, const double y2)
{
	return x1 * y2 - x2 * y1;
}

double GraphicsMath::CrossProduct(const Point v1, const Point v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

double GraphicsMath::LineSide2D(const Point p, const Point lineFrom, const Point lineTo)
{
	return CrossProduct(p.x - lineFrom.x, p.y - lineFrom.y, lineTo.x - lineFrom.x, lineTo.y - lineFrom.y);
}

double GraphicsMath::Convex2D(const Point origin, const Point pNext, const Point pPrev)
{
	const auto vectorA = Point(pNext.x - origin.x, pNext.y - origin.y);
	const auto vectorB = Point(pPrev.x - origin.x, pPrev.y - origin.y);
	
	return CrossProduct(vectorA, vectorB);
}

int GraphicsMath::ClipTest(double p, double q, double * u1, double * u2)
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
	const double abt = (a.x - l1.x) * (l2.y - l1.y) - (l2.x - l1.x) * (a.y - l1.y);
	const double bpt = (b.x - l1.x) * (l2.y - l1.y) - (l2.x - l1.x) * (b.y - l1.y);

	return abt * bpt > 0;
}

bool GraphicsMath::PointInTriangle(Point p, Point a, Point b, Point c)
{
	return SameSide(p, a, b, c) && 
		   SameSide(p, b, a, c) && 
		   SameSide(p, c, a, b);
}

