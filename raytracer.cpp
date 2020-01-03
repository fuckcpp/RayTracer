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
using namespace std;

const Material bg(Vec3f(0.2,0.7,0.8));
const Material white(Vec3f(1.,1.,1.));
const Material bg_fj("texture/bg/fj.jpg");
const int w = 1080;
const int h = 720;
const int n = 4;
float fov=M_PI/2;

texture* posx=new texture("texture/cubemap/posx.jpg");
texture* posy=new texture("texture/cubemap/posy.jpg");
texture* posz=new texture("texture/cubemap/posz.jpg");
texture* negx=new texture("texture/cubemap/negx.jpg");
texture* negy=new texture("texture/cubemap/negy.jpg");
texture* negz=new texture("texture/cubemap/negz.jpg");
cubemap* envir=new cubemap(posx,posy,posz,negx,negy,negz);

Vec3f reflect(const Vec3f& in,const Vec3f& n)
{
	return n*-(in*n)*2+in;
}

Vec3f refract(const Vec3f& in,const Vec3f& N,float refract)
{
	float cosi=-in*N;
	float ni=1;
	float nj=refract;
	Vec3f n=N;
	if(cosi<0)
	{
		cosi=-cosi;
		swap(ni,nj);
		n=-N;
	}
	float eta=ni/nj;
	float cosj=sqrtf(1-eta*eta*(1-cosi*cosi));
	return cosj<0?Vec3f(0,0,0):in*eta+n*(eta*cosi-cosj);
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

bool intersect(const Vec3f& orig,const Vec3f& dir,const vector<Sphere>& spheres,Vec3f& hit,Vec3f& norm,Material& mat)
{
	float distance=numeric_limits<float>::max();
	for(auto& sphere:spheres)
	{
		float dis;
		if(sphere.isRayIntersect(orig,dir,dis)&&dis<distance)
		{
			distance=dis;
                        hit=dir*dis+orig;
			norm=(hit-sphere.center).normalize();
			mat=sphere.mat;
		}
	}
	return distance<numeric_limits<float>::max();
}

Vec3f trace(const Vec3f& orig,const Vec3f& dir,const vector<Sphere>& spheres,
	const vector<PointLight>& pointLights,int depth)
{
	Vec3f hit;
	Vec3f n;
	Material mat(Vec3f(1,0,0));
	Vec3f color=bg.color;
	if(depth>0&& intersect(orig,dir,spheres,hit,n,mat))
	{
		float diffIntensity=0;
		float specIntensity=0;
		Vec3f reflectRay=reflect(dir,n).normalize();
		Vec3f refractRay=refract(dir,n,mat.refract).normalize();
		Vec3f reflectPoint=reflectRay*n>0?hit+n*1e-3:hit-n*1e-3;
		Vec3f refractPoint=refractRay*n>0?hit+n*1e-3:hit-n*1e-3;
		Vec3f reflectColor=trace(reflectPoint,reflectRay,spheres,pointLights,depth-1);
		Vec3f refractColor=trace(refractPoint,refractRay,spheres,pointLights,depth-1);
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
	       color=diffColor*mat.albedo[0]+specColor*mat.albedo[1]+reflectColor*mat.albedo[2]+refractColor*mat.albedo[3];
	}
	else
	{
		color=envir->intersect(dir);
	}
	tonemapping(color);
	return color;
}

void render(const vector<Sphere>& spheres,const vector<PointLight>& pointLights)
{
	unsigned char* data=(unsigned char*)malloc(w*h*n);
	cout<<" w:"<<w<<" h:"<<h<<" n:"<<n<<endl;
	string outPng = "out.png";

	Vec3f Camera(0.f,0.f,0.f);
	Vec3f Dir;

	//rgba 4通道，a取值255
	//#pragma omp parallel for
	int recurDepth=4;
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			float x=((i+0.5)/w*2-1)*tan(fov/2);
			float y=-((j+0.5)/h*2-1)*tan(fov/2)*h/w;
			float z=-1;
			Dir=Vec3f(x,y,z);
			Dir.normalize();
			
			Vec3f color=trace(Camera,Dir,spheres,pointLights,recurDepth);
			
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

	Material ivory(Vec3f(0.4, 0.4, 0.3),Vec4f(0.6,0.3,0.1,0.0),50,1.0);
	Material glass(Vec3f(0.6,0.7,0.8),Vec4f(0.0,0.5,0.1,0.8),125.,1.5);
	Material red_rubber(Vec3f(0.3, 0.1, 0.1),Vec4f(0.9,0.1,0.0,0.0),10,1.0);
	Material mirror(Vec3f(1.0,1.0,1.0),Vec4f(0.,10.0,0.8,0.0),1425,1.0);
	spheres.emplace_back(Sphere(Vec3f(-3,0,-16),2,ivory));
	spheres.emplace_back(Sphere(Vec3f(-1,-1.5,-12),2,glass));
	spheres.emplace_back(Sphere(Vec3f(1.5,-0.5,-18),3,red_rubber));
	spheres.emplace_back(Sphere(Vec3f(7,5,-18),4,mirror));
	pointLights.emplace_back(PointLight(Vec3f(-10,10,10),2));
	pointLights.emplace_back(PointLight(Vec3f(30,50,-25),1.8));
	pointLights.emplace_back(PointLight(Vec3f(30,20,30),1.7));
	render(spheres,pointLights);
	return 0;
}	
