#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "geometry.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;
struct texture
{
	int w,h,n;
	unsigned char* rgba;
	texture(const string& url)
	{
		cout<<url<<endl;
    		rgba = stbi_load(url.c_str(), &w, &h, &n, 0);
		cout<<"w: "<<w<<" h: "<<h<<" n: "<<n<<endl;
	}
	Vec3f uv(float u,float v)const
	{
		Vec3f color(0,0,0);
		assert(u>0&&u<1);
		assert(v>0&&v<1);
		//cout<<u<<","<<v<<endl;
		{
			u=fmod(u,1.);
			v=fmod(v,1.);
			u=fabs(u);
			v=fabs(v);
			int width=w*u;
			int height=h*v;
			color.x=rgba[n*(height*w+width)+0];
			color.y=rgba[n*(height*w+width)+1];
			color.z=rgba[n*(height*w+width)+2];
		}
		return color;
	}
};

struct cubemap
{
	texture* posx;
	texture* posy;
	texture* posz;
	texture* negx;
	texture* negy;
	texture* negz;
	cubemap(texture* px,texture* py,texture* pz,texture* nx,texture* ny,texture* nz):
		posx(px),posy(py),posz(pz),negx(nx),negy(ny),negz(nz)
	{
		assert(posx&&posy&&posz&&negx&&negy&&negz);
	}
	Vec3f intersect(const Vec3f& dir)const
	{
		texture* sampleTex;
		float u,v;
		float m=max(max(fabs(dir.x),fabs(dir.y)),fabs(dir.z));
		if(m==fabs(dir.x))
		{
			if(dir.x>0)
			{
				sampleTex=posx;
				u=(dir.z/m+1.f)/2;
				v=(-dir.y/m+1.f)/2;
			}
			else
			{
				sampleTex=negx;
				u=(-dir.z/m+1.f)/2;
				v=(-dir.y/m+1.f)/2;
			}
		}
		else if(m==fabs(dir.y))
		{
			if(dir.y>0)
			{
				sampleTex=posy;
				u=(dir.x/m+1.f)/2;
				v=(-dir.z/m+1.f)/2;
			}
			else
			{
				sampleTex=negy;
				u=(dir.x/m+1.f)/2;
				v=(dir.z/m+1.f)/2;
			}
		}
		else if(m==fabs(dir.z))
		{
			if(dir.z>0)
			{
				sampleTex=posz;
				u=(-dir.x/m+1.f)/2;
				v=(-dir.y/m+1.f)/2;
			}
			else
			{
				sampleTex=negz;
				u=(dir.x/m+1.f)/2;
				v=(-dir.y/m+1.f)/2;
			}
		}
		return sampleTex->uv(u,v);
	}

};
