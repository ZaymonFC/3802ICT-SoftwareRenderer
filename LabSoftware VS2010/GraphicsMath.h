#pragma once
#include <vector>

class Point;

class GraphicsMath
{
public:
	static Point RotatePoint(const Point& point, int rotationX, int rotationY, int rotationZ);

	static double Clamp(double value,  double minimum,  double maximum);
	static double LinearLerp( double start,  double end,  double gradient);
	static double CrossProduct( double x1,  double y1,  double x2,  double y2);
	static int ClipTest(double p, double q, double* u1, double* u2);
	static bool PointInTriangle(Point p, Point a, Point b, Point c);
	static double CrossProduct( Point v1,  Point v2);
	static double LineSide2D( Point p,  Point lineFrom,  Point lineTo);
	static double Convex2D( Point origin,  Point pNext,  Point pPrev);
	static int SameSide(Point a, Point b, Point l1, Point l2);
};

