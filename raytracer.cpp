#include<iostream>
#include<string>
#include"geometry.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
using namespace std;
int main()
{
	cout<<"ray tracer program"<<endl;
	string inPng = "touxiang.png";
	int w,h,n;
	unsigned char* data=stbi_load(inPng.c_str(),&w,&h,&n,0);
	cout<<" w:"<<w<<" h:"<<h<<" n:"<<n<<endl;
	string outPng="out.png";
	stbi_write_png(outPng.c_str(),w,h,n,data,0);
	stbi_image_free(data);
	return 0;
}	

