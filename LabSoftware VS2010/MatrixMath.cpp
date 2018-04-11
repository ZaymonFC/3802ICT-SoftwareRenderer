#include "MatrixMath.h"
#include "Point.h"

//auto MatrixMath::Mat4Multiply(const Mat4& a, const Mat4& b) -> Mat4
//{
//	Mat4 out;
//
//	for (auto row = 0; row < 4; ++row) {
//		for (auto column = 0; column < 4; ++column) {
//			float r = 0;
//			for (auto i = 0; i < 4; ++i) {
//				r = (a.values[row * 4 + i] * b.values[i * 4 + column]) + r;
//			}
//			out.values[row * 4 + column] = r;
//		}
//	}
//	return out;
//}

auto MatrixMath::TransformPoint(const Mat4& a, Point& point, float w) -> void
{
//	const auto row = 4;
//
//	point.x = a.values[0] * point.x + a.values[row] * point.y + a.values[2 * row] * point.z + a.values[3 * row] * w;
//	point.y = a.values[1] * point.y + a.values[row + 1] * point.y + a.values[2 * row + 1] * point.z + a.values[3 * row + 1] * w;
//	point.z = a.values[2] * point.z + a.values[row + 2] * point.y + a.values[2 * row + 2] * point.z + a.values[3 * row + 2] * w;

	{
		const auto r0 = a.values[0 * 4 + 0] * point.x;
		const auto r1 = a.values[0 * 4 + 1] * point.y;
		const auto r2 = a.values[0 * 4 + 2] * point.z;
		const auto r3 = a.values[0 * 4 + 3] * w;

		point.x = r0 + r1 + r2 + r3;
	}
	{
		const auto r0 = a.values[1 * 4 + 0] * point.x;
		const auto r1 = a.values[1 * 4 + 1] * point.y;
		const auto r2 = a.values[1 * 4 + 2] * point.z;
		const auto r3 = a.values[1 * 4 + 3] * w;

		point.y = r0 + r1 + r2 + r3;
	}
	{
		const auto r0 = a.values[2 * 4 + 0] * point.x;
		const auto r1 = a.values[2 * 4 + 1] * point.y;
		const auto r2 = a.values[2 * 4 + 2] * point.z;
		const auto r3 = a.values[2 * 4 + 3] * w;

		point.z = r0 + r1 + r2 + r3;
	}
}
