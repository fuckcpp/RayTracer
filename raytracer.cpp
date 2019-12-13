#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "geometry.h"
#include "Sphere.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

Vec3f color1(255,0,0);
Vec3f color2(0,255,0);
Sphere sphere(Vec3f(0,0,-10),5);

Vec3f trace(const Vec3f& orig,const Vec3f& dir)
{
	if(sphere.isRayIntersect(orig,dir))
		return color1;
	else
		return color2;
}

void render()
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
			float y=((j+0.5)/h*2-1)*tan(fov/2)*h/w;
			float z=-1;
			Dir=Vec3f(x,y,z);
			Dir.normalize();
			
			Vec3f color=trace(Camera,Dir);
			
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
	render();
	return 0;
}	

