#pragma once

#include <string>
#include <vector>
#include "Vec3.h"

class Face
{
public:
	int vertex[3];
	int normal[3];
};

class ObjModel
{
public:
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Face> faces;
	


	ObjModel(const std::string& fileName);

};

