#pragma once
#include "geometry.h"
#include "Material.h"
#include <cmath>
struct Sphere{
	Vec3f center;
	float radius;
	Material mat;
	Sphere(const Vec3f& c, const float& r,const Material& m):center(c),radius(r),mat(m){}
	bool isRayIntersect(const Vec3f& orig,const Vec3f& dir)
	{
		Vec3f l=center-orig;
		float proj=l*dir;
		float s=l*l-proj*proj;
		float d=sqrt(s);
		if(d>radius) return false;
		else
		{
			float t=sqrt(radius*radius-s);
			float t0=proj-t;
			if(t0<0) t0=proj+t;
			if(t0<0) return false;
		}
		return true;
	}
};
