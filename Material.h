#pragma once 
#include "geometry.h"
struct Material
{
	Vec3f color;
	Material(const Vec3f& c)
	{
		color=c*255;
	}
};
