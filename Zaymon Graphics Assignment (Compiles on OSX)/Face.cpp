#include "Face.h"
#include <utility>


Face::Face(Point a, Point b, Point c) : a{std::move(a)}, b{std::move(b)}, c{std::move(c)}
{
	
}



