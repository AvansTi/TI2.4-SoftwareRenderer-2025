#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include <iostream>
#include "stb_image_write.h"
#include "Vec3.h"
#include <math.h>
#include "Vec2.h"
#include "ObjModel.h"
#include "Matrix4.h"

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

Vec3 baryCentric(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& P)
{
	Vec3 X(C.x - A.x, B.x - A.x, A.x - P.x);
	Vec3 Y(C.y - A.y, B.y - A.y, A.y - P.y);
	Vec3 U = X.cross(Y);
	return Vec3(1 - (U.x + U.y) / U.z, U.y / U.z, U.x / U.z);
}

void drawTriangle(float* depthBuffer, unsigned char* data, 
	const Vec3& a, const Vec3& b, const Vec3& c,
	const Vec3& normala, const Vec3& normalb, const Vec3& normalc,
	const Vec3 &color)
{
	float minX = std::min(a.x, std::min(b.x, c.x));
	float minY = std::min(a.y, std::min(b.y, c.y));
	float maxY = std::max(a.y, std::max(b.y, c.y));
	float maxX = std::max(a.x, std::max(b.x, c.x));

	for (int x = (int)minX; x <= (int)maxX; x++)
	{
		for (int y = (int)minY; y <= (int)maxY; y++)
		{
			if (x < 0 || x >= 512 || y < 0 || y >= 512)
				continue;

			auto barry = baryCentric(a, b, c, Vec2(x,y));
			if (barry.x < 0 || barry.y < 0 || barry.z < 0)
				continue;
			if (barry.x > 1 || barry.y > 1 || barry.z > 1)
				continue;

			float z = a.z * barry.x + b.z * barry.y + c.z * barry.z;
			if (depthBuffer[y * imageWidth + x] > z)
				continue;
			Vec3 normal = normala * barry.x + normalb * barry.y + normalc * barry.z;
			
			float intensity = normal.normalized().dot(Vec3(1, 1, 1).normalized());
			intensity = std::max(0.0f, intensity);

			intensity = 0.2f + 0.8f * intensity;

			depthBuffer[y * imageWidth + x] = z;
			setPixel(data, x, y, color * intensity);
		}
	}

}


int main()
{
	unsigned char* data = new unsigned char[imageWidth * imageHeight * 3];
	float* depthBuffer = new float[imageWidth * imageHeight];
	ObjModel model("gecko.obj");

	for (int frame = 0; frame < 36; frame++)
	{
		for (int x = 0; x < imageWidth; x++)
			for (int y = 0; y < imageHeight; y++)
				setPixel(data, x, y, Vec3(.2f, .1f, .2f));
		for (int x = 0; x < imageWidth; x++)
			for (int y = 0; y < imageHeight; y++)
				depthBuffer[y * imageWidth + x] = -10000000;

		Matrix4 matrix = Matrix4::rotate(3.1415f * 2 * (frame / 36.0f), Vec3(0, 1, 0));


		for (const auto& face : model.faces)
		{
			auto a = matrix * model.vertices[face.vertex[0]];
			auto b = matrix * model.vertices[face.vertex[1]];
			auto c = matrix * model.vertices[face.vertex[2]];




			drawTriangle(depthBuffer, data,
				Vec3(256, 256, 0) + a * 256 * Vec3(1, -1, 1),
				Vec3(256, 256, 0) + b * 256 * Vec3(1, -1, 1),
				Vec3(256, 256, 0) + c * 256 * Vec3(1, -1, 1),
				model.normals[face.normal[0]],
				model.normals[face.normal[1]],
				model.normals[face.normal[2]],

				Vec3(1, 1, 1));
		}
		stbi_write_png(("out"+std::to_string(frame) + ".png").c_str(), imageWidth, imageHeight, 3, data, 0);
	}

#ifdef _WIN32
    ShellExecuteA(nullptr, nullptr, "out.png", nullptr, nullptr, SW_SHOW);
#endif

}