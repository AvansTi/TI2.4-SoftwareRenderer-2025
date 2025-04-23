#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include "stb_image_write.h"
#include "Vec3.h"

const int imageWidth = 512;
const int imageHeight = 512;



void setPixel(unsigned char* data, int x, int y, const Vec3 &color)
{
	if (x < 0 || x >= 512 || y < 0 || y >= 512)
		return;
	data[(y * imageWidth + x) * 3 + 0] = (unsigned char)(color.x * 255);
	data[(y * imageWidth + x) * 3 + 1] = (unsigned char)(color.y * 255);
	data[(y * imageWidth + x) * 3 + 2] = (unsigned char)(color.z * 255);
}

void drawTriangle(unsigned char* data, const Vec3& a, const Vec3& b, const Vec3& c, const Vec3 &color)
{
	
}


int main()
{
	unsigned char* data = new unsigned char[imageWidth * imageHeight * 3];

	for (int x = 0; x < imageWidth; x++)
		for (int y = 0; y < imageHeight; y++)
			setPixel(data, x, y, Vec3(.2f,.1f,.2f));


	drawTriangle(data, Vec3(10, 10, 0), Vec3(400, 10, 0), Vec3(200, 400, 0), Vec3(1, 1, 1));




	stbi_write_png("out.png", imageWidth, imageHeight, 3, data, 0);
#ifdef _WIN32
    ShellExecuteA(nullptr, nullptr, "out.png", nullptr, nullptr, SW_SHOW);
#endif

}