#include "Render.h"
#include "Colour.h"
#include "Point.h"
#include "DecompositionService.h"
#include "Mesh.h"

#include <cstdlib>
#include <vector>

Render::Render(const int width, const int height, BYTE * screen) : FRAME_WIDE{width}, FRAME_HIGH{height}, _screen{screen}
{
	for (auto i = 0; i < width * height; i++)
	{
		_zBuffer.emplace_back(0);
	}
}

void Render::SetPixel(Point point, Colour colour)
{
	// Calculate the position in the 1D Array
	const auto position = (point.x + point.y * FRAME_WIDE) * 3;
	const auto zPosition = (point.x + point.y * FRAME_WIDE);
	
	// Query ZBuffer
//	if (_zBuffer[zPosition] > point.z) return;

	_screen[position] = colour.r;
	_screen[position + 1] = colour.g;
	_screen[position + 2] = colour.b;

//	_zBuffer[zPosition] = point.z;
}

void Render::ClearZBuffer()
{
	std::fill(_zBuffer.begin(), _zBuffer.end(), 0);
}


void Render::DrawLine_Dda(const Point p0, const Point p1)
{
	const auto x0 = p0.x;
	const auto y0 = p0.y;
	const auto x1 = p1.x;
	const auto y1 = p1.y;

	const auto dx = abs(x1 - x0);
	const auto dy = abs(y1 - y0);

	int steps;
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}

	const auto x_inc = dx / static_cast<double>(steps);
	const auto y_inc = dy / static_cast<double>(steps);

	double x = x0;
	double y = y0;

	SetPixel(Point(lround(x), lround(y)), p0.colour);

	for (auto i = 0; i < steps; i++) {
		const auto colourLerp = p0.colour.Interpolate(p1.colour, steps, i);
		SetPixel(Point(lround(x), lround(y)), colourLerp);
		x += x_inc;
		y += y_inc;
	}
}


void Render::DrawClipLine(Point p1, Point p2)
{
	auto u1 = 0.0;
	auto u2 = 1.0;
	const double dx = p2.x - p1.x;

	if (GraphicsMath::ClipTest(-dx, p1.x - 0, &u1, &u2))
	{
		if (GraphicsMath::ClipTest(dx, static_cast<double>(FRAME_WIDE - p1.x), &u1, &u2))
		{
			const double dy = p2.y - p1.y;
			if (GraphicsMath::ClipTest(-dy, p1.y - 0, &u1, &u2))
			{
				if (GraphicsMath::ClipTest(dy, static_cast<double>(FRAME_HIGH - p1.y), &u1, &u2))
				{
					if (u2 < 1.0)
					{
						p2.x = p1.x + u2 * dx;
						p2.y = p1.y + u2 * dy;
					}
					if (u1 > 0.0)
					{
						p1.x += u1 * dx;
						p1.y += u1 * dy;
					}
					DrawLine_Dda(p1, p2);
				}
			}
		}
	}
}


void Render::DrawScanLine(const int y, const Point pa, const Point pb, const Point pc, const Point pd, const Colour&
	leftColour, const Colour& rightColour)
{
	// CLIP FOR Y
	if (y < 0 || y > FRAME_HIGH - 1) { return; }

	// Calculate the gradients of the two lines (pa-pb and pc-pd)
	const auto gradAB = pa.y != pb.y ? static_cast<double>(y - pa.y) / (pb.y - pa.y) : 1;
	const auto gradCD = pc.y != pd.y ? static_cast<double>(y - pc.y) / (pd.y - pc.y) : 1;

	// Calculate the left x
	const auto sx = static_cast<int>(GraphicsMath::LinearLerp(pa.x, pb.x, gradAB));
	// Calculate the right x
	const auto ex = static_cast<int>(GraphicsMath::LinearLerp(pc.x, pd.x, gradCD));

	const auto xWidth = ex - sx;
	for (auto x = sx; x <= ex; x++)
	{
		// CLIP FOR X
		if (x < 0) { continue; }
		if (x > FRAME_WIDE - 1) { return; }

		SetPixel(Point(x, y), leftColour.Interpolate(rightColour, xWidth, x - sx));
	}
}


void Render::DrawTriangle(Point p1, Point p2, Point p3)
{
	// Order the points by height
	if (p1.y > p2.y)
	{
		std::swap(p1, p2);
	}
	if (p2.y > p3.y)
	{
		std::swap(p2, p3);
	}
	if (p1.y > p2.y)
	{
		std::swap(p1, p2);
	}

	// Draw Triangle - P2 on the right
	if (GraphicsMath::LineSide2D(p2, p1, p3) > 0)
	{
		for (auto y = p1.y; y <= p3.y; y++)
		{
			// Calculate the colour on the left edge p1-p3 (Common to top and bottom)
			const auto stepsP1P3 = abs(p1.y - p3.y);
			const auto leftColour = p1.colour.Interpolate(p3.colour, stepsP1P3, y - p1.y);

			// Draw the top half
			if (y < p2.y)
			{
				// Calculate the colour on the right edge p1-p2
				const auto stepsP1P2 = abs(p1.y - p2.y);
				const auto rightColour = p1.colour.Interpolate(p2.colour, stepsP1P2, y - p1.y);

				DrawScanLine(y, p1, p3, p1, p2, leftColour, rightColour);
			}
			// Draw the bottom half
			else
			{
				// Calculate the colour on the right edge p2-p3
				const auto stepsP2P3 = abs(p2.y - p3.y);
				const auto rightColour = p2.colour.Interpolate(p3.colour, stepsP2P3, y - p2.y);

				DrawScanLine(y, p1, p3, p2, p3, leftColour, rightColour);
			}
		}
	}
	// Draw Triangle - P2 on the left
	else
	{
		for (auto y = p1.y; y < p3.y; y++)
		{
			// Calculate the colour on the right edge p1-p3 (Common edge)
			const auto stepsP1P3 = abs(p1.y - p3.y);
			const auto rightColour = p1.colour.Interpolate(p3.colour, stepsP1P3, y - p1.y);

			// Draw the top half
			if (y < p2.y)
			{
				// Calculate the colour on the left edge p1-p2
				const auto stepsP1P2 = abs(p1.y - p2.y);
				const auto leftColour = p1.colour.Interpolate(p2.colour, stepsP1P2, y - p1.y);

				DrawScanLine(y, p1, p2, p1, p3, leftColour, rightColour);
			}
			else
			{
				// Calculate the colour on the left edge p1-p3
				const auto stepsP2P3 = abs(p2.y - p3.y);
				const auto leftColour = p2.colour.Interpolate(p3.colour, stepsP2P3, y - p2.y);

				DrawScanLine(y, p2, p3, p1, p3, leftColour, rightColour);
			}
		}
	}
}


void Render::DrawPolygon(const std::vector<Point>& points)
{
	auto faces = DecompositionService::DecomposePolygon(points);

	for (const auto& face : faces)
	{
		DrawTriangle(face.A, face.B, face.C);
	}
}

void Render::DrawTriangle(Face face)
{
	DrawTriangle(face.A, face.B, face.C);
}

Point Render::TransformPoint(std::vector<Point>::const_reference point, const int d)
{
	const auto x = (((point.x - (FRAME_WIDE / 2)) * d) / (point.z + d)) + (FRAME_WIDE / 2);
	const auto y = (((point.y - (FRAME_HIGH / 2)) * d) / (point.z + d)) + (FRAME_HIGH / 2);
	return {x, y, point.z, point.colour};
}

void Render::DrawMesh(Mesh mesh)
{
	auto faces = std::vector<Face>();
	for (const auto& polygon : mesh.polygons)
	{
		auto transformedPoints = std::vector<Point>();
		for (auto index : polygon)
		{
			transformedPoints.push_back(TransformPoint(mesh.vertices[index], 500));
		}

		for (const auto& face : DecompositionService::DecomposePolygon(transformedPoints))
		{
			faces.push_back(face);
		}
	}

	for (const auto& face : faces)
	{
		DrawTriangle(face);
	}
}
