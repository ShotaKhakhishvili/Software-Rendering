#pragma once

// std

#define PI 3.141592653

struct vec3
{
	float x, y, z;
};

struct triangle
{
	vec3 p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};


