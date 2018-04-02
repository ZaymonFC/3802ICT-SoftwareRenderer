#include "Face.h"
#include <utility>


Face::Face(Point a, Point b, Point c) : A{std::move(a)}, B{std::move(b)}, C{std::move(c)}
{
	
}



