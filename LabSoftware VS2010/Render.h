#pragma once
#include <bitset>
#include "Colour.h"
#include "Point.h"
#include "GraphicsMath.h"
#include <vector>

class Mesh;
class Face;

class Render
{
	using BYTE = unsigned char;
	const int frame_wide_;
	const int frame_high_;
	const int vanishingPointOffset_;
	BYTE * _screen;
	std::vector<int> _zBuffer;
public:
	Render(int width, int height, int vanishingPointOffset, BYTE * screen);

	void SetPixel(Point point, Colour colour);

	void ClearZBuffer();

	void DrawLine_Dda(const Point p0, const Point p1);

	void DrawClipLine(Point p1, Point p2);

	void DrawScanLine(const int y, const Point pa, const Point pb, const Point pc, const Point pd, const Colour& leftColour,
	                  const Colour& rightColour);

	void DrawTriangle(Point p1, Point p2, Point p3);

	void DrawPolygon(const std::vector<Point>& points);

	void DrawTriangle(Face face);

	Point ProjectionTransformPoint(std::vector<Point>::const_reference point, int d) const;

	void DrawMesh(Mesh mesh);

	~Render() = default;
};

