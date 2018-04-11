#pragma once
#include "Colour.h"
#include "Point.h"
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
	std::vector<float> _zBuffer;
	
	bool renderZ_;
	bool renderWire_;
	bool renderZBuff_;
public:
	Render(int width, int height, int vanishingPointOffset, BYTE * screen);

	void SetPixel(Point point, Colour colour);

	void ClearZBuffer();

	void DrawLine_Dda(Point p0, Point p1);

	void DrawClipLine(Point p1, Point p2);

	void DrawScanLine(int y, Point pa, Point pb, Point pc, Point pd, const Colour& leftColour, const Colour& rightColour, float zLeft, float zRight);

	void DrawTriangle(Point p1, Point p2, Point p3);

	void DrawPolygon(const std::vector<Point>& points);

	void DrawTriangle(Face face);

	void DrawMesh(Mesh mesh);

	auto DrawWireFrame(const Mesh& mesh)-> void;

	auto SwitchRenderMode() -> void;

	auto ToggleZBuffer() -> void;
};

