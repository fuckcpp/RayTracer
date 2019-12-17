#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "geometry.h"
#include "material.h"
#include "sphere.h"
#include "light.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

Material ivory(Vec3f(0.4, 0.4, 0.3));
Material red_rubber(Vec3f(0.3, 0.1, 0.1));
Material bg(Vec3f(0.1,0.1,0.5));

Vec3f trace(const Vec3f& orig,const Vec3f& dir,const vector<Sphere>& spheres,
	const vector<PointLight>& pointLights)
{
	Vec3f hit;
	Vec3f n;
	float intensitys=0;
	Material mat(Vec3f(1,0,0));
	for(auto& sphere:spheres)
	{	
		if(sphere.isRayIntersect(orig,dir,hit,n,mat))
		{
			for(auto& pointLight:pointLights)
			{
				Vec3f l=(pointLight.center-hit).normalize();
				intensitys+=pointLight.intensity*max(0.f,l*n);
			}
			Vec3f color=mat.color*intensitys;//rgb值超过255怎么解决？
			return color;
		}
	}
	return bg.color;
}

void render(const vector<Sphere>& spheres,const vector<PointLight>& pointLights)
{
	const int w = 1080;
	const int h = 720;
	const int n = 4;
	unsigned char* data=(unsigned char*)malloc(w*h*n);
	cout<<" w:"<<w<<" h:"<<h<<" n:"<<n<<endl;
	string outPng = "out.png";

	Vec3f Camera(0.,0.,0.);
	Vec3f Dir;
	float fov=M_PI/2;

	//rgba 4通道，a取值255
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			float x=((i+0.5)/w*2-1)*tan(fov/2);
			float y=-((j+0.5)/h*2-1)*tan(fov/2)*h/w;
			float z=-1;
			Dir=Vec3f(x,y,z);
			Dir.normalize();
			
			Vec3f color=trace(Camera,Dir,spheres,pointLights);
			
			data[n*(j*w+i)+0]=color.x;
			data[n*(j*w+i)+1]=color.y;
			data[n*(j*w+i)+2]=color.z;
			data[n*(j*w+i)+3]=255;
		}
	}
	stbi_write_png(outPng.c_str(),w,h,n,data,0);
	stbi_image_free(data);
}
int main()
{
	cout<<"ray tracer program"<<endl;
	vector<Sphere> spheres;
	vector<PointLight> pointLights;
	spheres.emplace_back(Sphere(Vec3f(-3,0,-16),2,ivory));
	spheres.emplace_back(Sphere(Vec3f(-1,-1.5,-12),2,red_rubber));
	spheres.emplace_back(Sphere(Vec3f(1.5,-0.5,-18),3,ivory));
	spheres.emplace_back(Sphere(Vec3f(7,5,-18),4,red_rubber));
	pointLights.emplace_back(PointLight(Vec3f(10,10,10),1));
	pointLights.emplace_back(PointLight(Vec3f(0,0,0),1));
	render(spheres,pointLights);
	return 0;
}	
