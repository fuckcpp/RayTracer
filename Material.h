#pragma once 
#include "geometry.h"
#include "texture.h"
#include <string>
using namespace std;
struct Material
{
	Vec3f color;
	Vec4f albedo;
	float specular;
	float refract;

	texture* tex;

	Material(const Vec3f& c,const Vec4f& a,const float& s,const float& r):albedo(a),specular(s),refract(r)
	{
		color=c*255;
	}

	Material(const Vec3f& c)
	{
		new(this) Material(c,Vec4f(0.,0.,0.,0.),0.,1.);
	}
	Material(const string& url)
	{
		tex=new texture(url);
	}
	Vec3f uv(float u,float v) const
	{
		return tex?tex->uv(u,v):Vec3f(0,0,0);
	}
};
