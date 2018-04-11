#include "Render.h"
#include "Colour.h"
#include "Point.h"
#include "DecompositionService.h"
#include "Mesh.h"

#include <cstdlib>
#include <vector>
#include "libs/glut.h"

Render::Render(const int width, const int height, const int vanishingPointOffset, BYTE * screen) :
	frame_wide_{ width },
	frame_high_{ height },
	vanishingPointOffset_(vanishingPointOffset),
	_screen { screen }
{
	// Initialise the Z-Buffer
	for (auto i = 0; i < frame_wide_ * frame_high_; i++)
	{
		_zBuffer.push_back(0.0f);
	}

	renderZ_ = false;
	renderWire_ = false;
	renderZBuff_ = true;
}

void Render::SetPixel(const Point point, const Colour colour)
{
	const auto x = static_cast<int>(point.x);
	const auto y = static_cast<int>(point.y);

	// Calculate the position in the 1D Array
	const auto position = (x + y * frame_wide_) * 3;
	const auto zPosition = x + y * frame_wide_;
	
//	// Query ZBuffer
	if (renderZBuff_)
	{
		if (point.z > _zBuffer.at(zPosition)) return;
	}

	_zBuffer.at(zPosition) = point.z;

	// Set the screen position
	if (!renderZ_)
	{
		_screen[position] = colour.r;
		_screen[position + 1] = colour.g;
		_screen[position + 2] = colour.b;
	}
	else
	{
		const auto zColour = GraphicsMath::Normalize(point.z, 0, 1200);
		const auto zColourConverted = 255 - (zColour >= 1.0 ? 255 : (zColour <= 0.0 ? 0 : static_cast<int>(floor(zColour * 256.0))));
		
		_screen[position] = zColourConverted;
		_screen[position + 1] = zColourConverted;
		_screen[position + 2] = zColourConverted;
	}
}

void Render::ClearZBuffer()
{
	std::fill(_zBuffer.begin(), _zBuffer.end(), 100000);
}


void Render::DrawLine_Dda(const Point p0, const Point p1)
{
	const auto x0 = p0.x;
	const auto y0 = p0.y;
	const auto x1 = p1.x;
	const auto y1 = p1.y;

	const auto dx = x1 - x0;
	const auto dy = y1 - y0;

	int steps;
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}

	const auto x_inc = dx / static_cast<float>(steps);
	const auto y_inc = dy / static_cast<float>(steps);

	auto x = x0;
	auto y = y0;

	SetPixel(Point(x, y), p0.colour);

	for (auto i = 0; i < steps; i++) {
		const auto colourLerp = p0.colour.Interpolate(p1.colour, steps, i);
		SetPixel(Point(x, y), colourLerp);
		x += x_inc;
		y += y_inc;
	}
}


void Render::DrawClipLine(Point p1, Point p2)
{
	auto u1 = 0.0f;
	auto u2 = 1.0f;
	const auto dx = p2.x - p1.x;

	if (GraphicsMath::ClipTest(-dx, p1.x - 0, &u1, &u2))
	{
		if (GraphicsMath::ClipTest(dx, static_cast<double>(frame_wide_ - 1 - p1.x), &u1, &u2))
		{
			const float dy = p2.y - p1.y;
			if (GraphicsMath::ClipTest(-dy, p1.y - 0, &u1, &u2))
			{
				if (GraphicsMath::ClipTest(dy, static_cast<float>(frame_high_ - 1 - p1.y), &u1, &u2))
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

void Render::DrawScanLine(const int y, const Point pa, const Point pb, const Point pc, const Point pd, const Colour& leftColour,
	const Colour& rightColour, float zLeft, float zRight)
{
	// CLIP FOR Y
	if (y < 0 || y > frame_high_ - 1) { return; }

	// Calculate the gradients of the two lines (pa-pb and pc-pd)
	const auto gradAB = pa.y != pb.y ? static_cast<double>(y - pa.y) / (pb.y - pa.y) : 1;
	const auto gradCD = pc.y != pd.y ? static_cast<double>(y - pc.y) / (pd.y - pc.y) : 1;

	// Calculate the left x
	const auto sx = static_cast<int>(GraphicsMath::LinearLerp(pa.x, pb.x, gradAB));
	// Calculate the right x
	const auto ex = static_cast<int>(GraphicsMath::LinearLerp(pc.x, pd.x, gradCD));

	const auto xWidth = ex - sx;

	const auto zIncrement = (zRight - zLeft) / xWidth;

	for (auto x = sx; x <= ex; x++)
	{
		// CLIP FOR X
		if (x < 0) { continue; }
		if (x > frame_wide_ - 1) { return; }

		SetPixel(Point(x, y, zLeft), leftColour.Interpolate(rightColour, xWidth, x - sx));
		zLeft += zIncrement;
	}
}

float InterpolateZValue(float initalValue, float increment, float index)
{
	return initalValue + (increment * index);
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

	const auto stepsP1P3 = abs(p1.y - p3.y);
	const auto stepsP1P2 = abs(p1.y - p2.y);
	const auto stepsP2P3 = abs(p2.y - p3.y);

	const auto zIncrementP1P3 = (p3.z - p1.z) / stepsP1P3;
	const auto zIncrementP1P2 = (p2.z - p1.z) / stepsP1P2;
	const auto zIncrementP2P3 = (p3.z - p2.z) / stepsP2P3;

	auto initialY = static_cast<int>(p1.y);
	
	if (initialY == static_cast<int>(p2.y))
	{
		DrawScanLine(initialY, p1, p3, p1, p2, p1.colour, p2.colour, p1.z, p2.z);
		initialY++;
	}

	// Draw Triangle - P2 on the right
	if (GraphicsMath::LineSide2D(p2, p1, p3) > 0)
	{
		for (auto y = initialY; y <= p3.y; y++)
		{
			const auto leftColour = p1.colour.Interpolate(p3.colour, stepsP1P3, y - p1.y);
			const auto currentStepLeft = stepsP1P3 - abs(p3.y - y);
			const auto zLeft = InterpolateZValue(p1.z, zIncrementP1P3, currentStepLeft);

			// Draw the top half
			if (y < p2.y)
			{
				const auto rightColour = p1.colour.Interpolate(p2.colour, stepsP1P2, y - p1.y);
				const auto currentStepRight = stepsP1P2 - abs(p2.y - y);
				const auto zRight = InterpolateZValue(p1.z, zIncrementP1P2, currentStepRight);

				DrawScanLine(y, p1, p3, p1, p2, leftColour, rightColour, zLeft, zRight);
			}
			// Draw the bottom half
			else
			{
				const auto currentStepRight = stepsP2P3 - abs(p3.y - y);
				const auto zRight = InterpolateZValue(p2.z, zIncrementP2P3, currentStepRight);
				const auto rightColour = p2.colour.Interpolate(p3.colour, stepsP2P3, y - p2.y);

				DrawScanLine(y, p1, p3, p2, p3, leftColour, rightColour, zLeft, zRight);
			}
		}
	}
	// Draw Triangle - P2 on the left
	else
	{
		for (auto y = initialY; y < p3.y; y++)
		{
			const auto rightColour = p1.colour.Interpolate(p3.colour, stepsP1P3, y - p1.y);
			const auto currentStepRight = stepsP1P3 - abs(p3.y - y);
			const auto zRight = InterpolateZValue(p1.z, zIncrementP1P3, currentStepRight);

			// Draw the top half
			if (y < p2.y)
			{
				const auto currentStepLeft = stepsP1P2 - abs(p2.y - y);
				const auto zLeft = InterpolateZValue(p1.z, zIncrementP1P2, currentStepLeft);
				const auto leftColour = p1.colour.Interpolate(p2.colour, stepsP1P2, y - p1.y);

				DrawScanLine(y, p1, p2, p1, p3, leftColour, rightColour, zLeft, zRight);
			}
			else
			{
				const auto currentStepLeft = stepsP2P3 - abs(p3.y - y);
				const auto zLeft = InterpolateZValue(p2.z, zIncrementP2P3, currentStepLeft);
				const auto leftColour = p2.colour.Interpolate(p3.colour, stepsP2P3, y - p2.y);

				DrawScanLine(y, p2, p3, p1, p3, leftColour, rightColour, zLeft, zRight);
			}
		}
	}
}

void Render::DrawPolygon(const std::vector<Point>& points)
{
	auto faces = DecompositionService::DecomposePolygon(points);

	for (const auto& face : faces)
	{
		DrawTriangle(face.a, face.b, face.c);
	}
}

void Render::DrawTriangle(Face face)
{
	DrawTriangle(face.a, face.b, face.c);
}

void Render::DrawMesh(Mesh mesh)
{
	if (renderWire_)
	{
		DrawWireFrame(mesh);
		return;
	}

	auto translatedPoints = mesh.TransformVertices();
	auto faces = std::vector<Face>();

	for (const auto& polygon : mesh.polygons)
	{
		auto transformedPoints = std::vector<Point>();

//		if (!GraphicsMath::BackFaceCull())

		for (auto index : polygon)
		{
			transformedPoints.push_back(GraphicsMath::ProjectionTransformPoint(translatedPoints[index], vanishingPointOffset_, frame_wide_, frame_high_));
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

auto Render::DrawWireFrame(const Mesh& mesh) -> void
{
	auto translatedPoints = mesh.TransformVertices();
	for (const auto& polygon : mesh.polygons)
	{
		auto transformedPoints = std::vector<Point>();

		for (auto index : polygon)
		{
			transformedPoints.push_back(GraphicsMath::ProjectionTransformPoint(translatedPoints[index], vanishingPointOffset_, frame_wide_, frame_high_));
		}

		for (unsigned int i = 0; i < transformedPoints.size() - 1; i++) {
			DrawClipLine(transformedPoints[i], transformedPoints[i + 1]);
		}
	}
}

auto Render::SwitchRenderMode() -> void
{
	if (renderZ_)
	{
		renderZ_ = false;
		renderWire_ = true;
		return;
	}
	
	if (renderWire_)
	{
		renderWire_ = false;
		return;
	}

	renderZ_ = true;
}

auto Render::ToggleZBuffer() -> void
{
	renderZBuff_ = !renderZBuff_;
}