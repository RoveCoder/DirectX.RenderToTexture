#pragma once

#include <vector>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) :
		x(x), y(y), z(z),
		nx(nx), ny(ny), nz(nz),
		u(u), v(v) {}

	float x;
	float y;
	float z;

	float u;
	float v;

	float nx;
	float ny;
	float nz;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};