#pragma once
#include "geometry.h"
#include "Sphere.h"
#include <vector>
using namespace std;
struct Camera
{
	Vec3f center;
	const int w = 1080;
	const int h = 720;
	const int n = 4;
	vector<Sphere> spheres;
	unsigned char* data;
	Camera(const Vec3f& c):center(c)
	{
		data=(unsigned char*)malloc(w*h*n);
	}
	Vec3f trace(const Vec3f& orig,const Vec3f& dir)
	{
		for(auto& sphere:spheres)
		{

			if(sphere.isRayIntersect(orig,dir))
				return color1;
			else
				return color2;
		}
	}
}
