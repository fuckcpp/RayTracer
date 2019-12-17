#pragma once 
#include "geometry.h"
struct DirectionalLight
{
	Vec3f dir;
	float intensity;
};

struct PointLight
{
	Vec3f center;
	float intensity;
	PointLight(const Vec3f& c,float i):center(c),intensity(i){}
};
