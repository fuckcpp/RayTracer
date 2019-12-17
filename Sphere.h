#pragma once
#include "geometry.h"
#include "material.h"
#include <cmath>
struct Sphere{
	Vec3f center;
	float radius;
	Material mat;
	Sphere(const Vec3f& c, const float& r,const Material& m):center(c),radius(r),mat(m){}
	bool isRayIntersect(const Vec3f& orig,const Vec3f& dir, Vec3f& hit,Vec3f& norm,
			Material& m) const
	{
		Vec3f l=center-orig;
		float proj=l*dir;
		float s=l*l-proj*proj;
		float d=sqrt(s);
		float t0;
		if(d>radius) return false;
		else
		{
			float t=sqrt(radius*radius-s);
			t0=proj-t;
			if(t0<0) t0=proj+t;
			if(t0<0) return false;
		}
		hit=dir*t0+orig;
		norm=hit-center;
		norm.normalize();
		m=mat;
		return true;
	}
};
