#pragma once 
#include "geometry.h"
struct Material
{
	Vec3f color;
	Vec4f albedo;
	float specular;
	float refract;

	Material(const Vec3f& c,const Vec4f& a,const float& s,const float& r):albedo(a),specular(s),refract(r)
	{
		color=c*255;
	}

	Material(const Vec3f& c)
	{
		new(this) Material(c,Vec4f(0.,0.,0.,0.),0.,1.);
	}
};
