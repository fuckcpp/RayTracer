#pragma once 
#include "geometry.h"
struct Material
{
	Vec3f color;
	Vec3f albedo;
	float specular;

	Material(const Vec3f& c,const Vec3f& a,const float& s):albedo(a),specular(s)
	{
		color=c*255;
	}

	Material(const Vec3f& c)
	{
		new(this) Material(c,Vec3f(0.,0.,0.),0.);
	}
};
