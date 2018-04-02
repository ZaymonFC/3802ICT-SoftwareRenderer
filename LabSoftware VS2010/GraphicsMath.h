#pragma once

class Point;

class GraphicsMath
{
public:
	GraphicsMath() = default;
	~GraphicsMath() = default;

	static double Clamp(const double value, const double minimum, const double maximum);
	static double LinearLerp(const double start, const double end, const double gradient);
	static double CrossProduct(const double x1, const double y1, const double x2, const double y2);
	static int ClipTest(double p, double q, double* u1, double* u2);
	static bool PointInTriangle(Point p, Point a, Point b, Point c);
	static double CrossProduct(const Point v1, const Point v2);
	static double LineSide2D(const Point p, const Point lineFrom, const Point lineTo);
	static double Convex2D(const Point origin, const Point pNext, const Point pPrev);
private:
	static int SameSide(Point a, Point b, Point l1, Point l2);
};

