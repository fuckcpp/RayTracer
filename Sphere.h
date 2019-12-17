#pragma once
#include "geometry.h"
#include "material.h"
#include <cmath>
struct Sphere{
	Vec3f center;
	float radius;
	Material mat;
	Sphere(const Vec3f& c, const float& r,const Material& m):center(c),radius(r),mat(m){}
	bool isRayIntersect(const Vec3f& orig,const Vec3f& dir,float& dis) const
	{
		Vec3f l=center-orig;
		float proj=l*dir;
		float s=l*l-proj*proj;
		float d=sqrt(s);
		if(d>radius) return false;
		else
		{
			float t=sqrt(radius*radius-s);
			dis=proj-t;
			if(dis<0) dis=proj+t;
			if(dis<0) return false;
		}
		return true;
	}
};
