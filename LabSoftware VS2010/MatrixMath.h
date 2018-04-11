#pragma once
class Point;

class MatrixMath
{
public:
	struct Mat4
	{
		float values[16];
		Mat4 Mat4Multiply(const Mat4& b)
		{
			auto out = Mat4();

			for (auto row = 0; row < 4; ++row) {
				for (auto column = 0; column < 4; ++column) {
					float r = 0;
					for (auto i = 0; i < 4; ++i) {
						r = (values[row * 4 + i] * b.values[i * 4 + column]) + r;
					}
					out.values[row * 4 + column] = r;
				}
			}
			return out;
		}
	};

//	static auto Mat4Multiply(const Mat4& a, const Mat4& b) -> Mat4;

	static auto TransformPoint(const Mat4& a, Point& point, float w) -> void;

};

