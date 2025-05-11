#include "olcConsoleGameEngine.h"

// std
#include <stdexcept>
#include <iostream>

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

class srEngine : public olcConsoleGameEngine
{
private:
	mesh mesh_cube;
	mat4x4 mat_proj;
	mat4x4 rotZ, rotY;
	float rotAngle = 0.0f;

	void MultiplyMatrixVector(const vec3& in, vec3& out, const mat4x4& mat)const
	{
		out.x	= in.x * mat.m[0][0] + in.y * mat.m[1][0] + in.z * mat.m[2][0] + mat.m[3][0];
		out.y	= in.x * mat.m[0][1] + in.y * mat.m[1][1] + in.z * mat.m[2][1] + mat.m[3][1];
		out.z	= in.x * mat.m[0][2] + in.y * mat.m[1][2] + in.z * mat.m[2][2] + mat.m[3][2];
		float w = in.x * mat.m[0][3] + in.y * mat.m[1][3] + in.z * mat.m[2][3] + mat.m[3][3];

		if (w != 0)
		{
			out.x /= w;
			out.y /= w;
			out.z /= w;
		}
	}

public:
	srEngine()
	{
		m_sAppName = L"test";
	}

	bool OnUserCreate() override
	{
		mesh_cube.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		// Projection Matrix

		float f_far = 1000;
		float f_near = 1;
		float f_FOV = 90;
		float f_aspectRatio = static_cast<float>(ScreenHeight()) / static_cast<float>(ScreenWidth());
		float f_FOV_rad = 1.0f / tanf(PI * f_FOV / 360.0f);

		mat_proj.m[0][0] = f_aspectRatio * f_FOV_rad;
		mat_proj.m[1][1] = f_FOV_rad;
		mat_proj.m[2][2] = f_far * (f_far - f_near);
		mat_proj.m[3][2] = -(f_far * f_near) / (f_far - f_near);
		mat_proj.m[2][3] = 1.0f;
		mat_proj.m[3][3] = 0.0f;

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		rotAngle += 0.5f * deltaTime;

		rotZ.m[0][0] = cosf(rotAngle);
		rotZ.m[0][1] = -sinf(rotAngle);
		rotZ.m[1][0] = sinf(rotAngle);
		rotZ.m[1][1] = cosf(rotAngle);
		rotZ.m[2][2] = 1;
		rotZ.m[3][3] = 1;

		rotY.m[0][0] = cosf(rotAngle);
		rotY.m[0][2] = sinf(rotAngle);
		rotY.m[1][1] = 1;
		rotY.m[2][0] = -sinf(rotAngle);
		rotY.m[2][2] = cosf(rotAngle);
		rotY.m[3][3] = 1;

		// Drawing triangles
		for (const triangle& tri : mesh_cube.tris)
		{
			triangle triProjected, triTranslated, triRotZ, triRotZY;

			MultiplyMatrixVector(tri.p[0], triRotZ.p[0], rotZ);
			MultiplyMatrixVector(tri.p[1], triRotZ.p[1], rotZ);
			MultiplyMatrixVector(tri.p[2], triRotZ.p[2], rotZ);

			MultiplyMatrixVector(triRotZ.p[0], triRotZY.p[0], rotY);
			MultiplyMatrixVector(triRotZ.p[1], triRotZY.p[1], rotY);
			MultiplyMatrixVector(triRotZ.p[2], triRotZY.p[2], rotY);

			triTranslated = triRotZY;

			triTranslated.p[0].z += 3;
			triTranslated.p[1].z += 3;
			triTranslated.p[2].z += 3;

			MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], mat_proj);
			MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], mat_proj);
			MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], mat_proj);

			// Scaling to the view
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * static_cast<float>(ScreenWidth());
			triProjected.p[0].y *= 0.5f * static_cast<float>(ScreenHeight());
			triProjected.p[1].x *= 0.5f * static_cast<float>(ScreenWidth());
			triProjected.p[1].y *= 0.5f * static_cast<float>(ScreenHeight());
			triProjected.p[2].x *= 0.5f * static_cast<float>(ScreenWidth());
			triProjected.p[2].y *= 0.5f * static_cast<float>(ScreenHeight());

			FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				PIXEL_SOLID, FG_CYAN);

		}

		return true;
	}
};

int main()
{
	srEngine test;

	if (test.ConstructConsole(512, 480, 2, 2))
		test.Start();
	else
		throw std::runtime_error("Unable to construct the console engine");
}