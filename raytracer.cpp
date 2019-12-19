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

const Material bg(Vec3f(0.1,0.5,0.1));
const Material white(Vec3f(1.,1.,1.));

Vec3f reflect(const Vec3f& in,const Vec3f& n)
{
	return n*-(in*n)*2+in;
}

void tonemapping(Vec3f& c)
{
        c.x=c.x>255?255:(c.x<0?0:c.x);
        c.y=c.y>255?255:(c.y<0?0:c.y);
        c.z=c.z>255?255:(c.z<0?0:c.z);
}

bool shadow(const vector<Sphere>& spheres,const Vec3f& hit,const Vec3f& l,const Vec3f& n)
{
	bool isOccluded=false;
	float distance=numeric_limits<float>::max();
	Vec3f shadow_orig=l*n>0?hit+n*1e-3:hit-n*1e-3;
	for(auto& sphere:spheres)
	{
		if(sphere.isRayIntersect(shadow_orig,l,distance))
			isOccluded=true;
	}
	return isOccluded;
}

void intersect(const Vec3f& orig,const Vec3f& dir,const vector<Sphere>& spheres,float& distance,Vec3f& hit,Vec3f& norm,Material& mat)
{
	float dis=numeric_limits<float>::max();
	for(auto& sphere:spheres)
	{
		if(sphere.isRayIntersect(orig,dir,dis)&&dis<distance)
		{
			distance=dis;
                        hit=dir*dis+orig;
			norm=(hit-sphere.center).normalize();
			mat=sphere.mat;
		}
	}
}

Vec3f trace(const Vec3f& orig,const Vec3f& dir,const vector<Sphere>& spheres,
	const vector<PointLight>& pointLights)
{
	Vec3f hit;
	Vec3f n;
	float diffIntensity=0;
	float specIntensity=0;
	Material mat(Vec3f(1,0,0));
	float distance=numeric_limits<float>::max();
	Vec3f color=bg.color;
	intersect(orig,dir,spheres,distance,hit,n,mat);
	if(distance<numeric_limits<float>::max())
	{
		for(auto& pointLight:pointLights)
		{
			Vec3f l=(pointLight.center-hit).normalize();
			if(shadow(spheres,hit,l,n))
				continue;
			diffIntensity+=pointLight.intensity*max(0.f,l*n);
			specIntensity+=pointLight.intensity*powf(max(0.f,reflect(-l,n)*-dir),mat.specular);
		}
	       Vec3f diffColor=mat.color*diffIntensity;
	       Vec3f specColor=white.color*specIntensity;
	       color=diffColor*mat.albedo[0]+specColor*mat.albedo[1];
	}
	tonemapping(color);
	return color;
	
}

void render(const vector<Sphere>& spheres,const vector<PointLight>& pointLights)
{
	const int w = 1080;
	const int h = 720;
	const int n = 4;
	unsigned char* data=(unsigned char*)malloc(w*h*n);
	cout<<" w:"<<w<<" h:"<<h<<" n:"<<n<<endl;
	string outPng = "out.png";

	Vec3f Camera(0.f,0.f,0.f);
	Vec3f Dir;
	float fov=M_PI/2;

	//rgba 4通道，a取值255
	//#pragma omp parallel for
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

	Material ivory(Vec3f(0.4, 0.4, 0.3),Vec2f(0.6,0.3),50);
	Material red_rubber(Vec3f(0.3, 0.1, 0.1),Vec2f(0.9,0.1),10);
	spheres.emplace_back(Sphere(Vec3f(-3,0,-16),2,ivory));
	spheres.emplace_back(Sphere(Vec3f(-1,-1.5,-12),2,red_rubber));
	spheres.emplace_back(Sphere(Vec3f(1.5,-0.5,-18),3,ivory));
	spheres.emplace_back(Sphere(Vec3f(7,5,-18),4,red_rubber));
	pointLights.emplace_back(PointLight(Vec3f(-10,10,10),2));
	pointLights.emplace_back(PointLight(Vec3f(30,50,-25),1.8));
	pointLights.emplace_back(PointLight(Vec3f(30,20,30),1.7));
	render(spheres,pointLights);
	return 0;
}	
