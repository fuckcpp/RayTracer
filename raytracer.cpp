#include<iostream>
#include<vector>
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
	const int w = 1080;
	const int h = 720;
	const int n = 4;
	unsigned char* data=(unsigned char*)malloc(w*h*n);
	cout<<" w:"<<w<<" h:"<<h<<" n:"<<n<<endl;
	string outPng = "out.png";
	//rgba 4通道，a取值255
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			data[n*(j*w+i)+0]=j/float(h)*255;
			data[n*(j*w+i)+1]=i/float(w)*255;
			data[n*(j*w+i)+2]=0;
			data[n*(j*w+i)+3]=255;
		}
	}
	stbi_write_png(outPng.c_str(),w,h,n,data,0);
	stbi_image_free(data);
	return 0;
}	

