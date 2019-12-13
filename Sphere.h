#pragma once
#include "geometry.h"
#include <cmath>
struct Sphere{
	Vec3f center;
	float radius;
	Sphere(const Vec3f& c, const float& r):center(c),radius(r){}
	bool isRayIntersect(const Vec3f& orig,const Vec3f& dir)
	{
		Vec3f l=center-orig;
		float proj=l*dir;
		float d=sqrt(l*l-proj*proj);
		if(d>radius) return false;
		else return true;
	}
};
