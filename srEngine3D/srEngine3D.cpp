#include "srEngine3D.h"
#include "FuncLib.h"

srEngine::srEngine(std::string objSrc)
{
	ReadFromObj(objSrc, mesh);
	m_sAppName = L"test";
}

bool srEngine::OnUserCreate()
{
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

bool srEngine::OnUserUpdate(float deltaTime)
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
	for (const triangle& tri : mesh.tris)
	{
		triangle triProjected, triTranslated, triRotZ, triRotZY;

		// Rotation

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

		// Applying Normal

		vec3 normal, line1, line2;

		line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
		line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
		line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

		line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
		line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
		line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

		float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z + normal.z);
		
		normal.x /= l;
		normal.y /= l;
		normal.z /= l;

		vec3 lookDirection;
		
		lookDirection.x = triTranslated.p[0].x - vCamera.x;
		lookDirection.y = triTranslated.p[0].y - vCamera.y;
		lookDirection.z = triTranslated.p[0].z - vCamera.z;

		float dotProduct = normal.x * lookDirection.x + normal.y * lookDirection.y + normal.z * lookDirection.z;

		if (dotProduct > 0.0f)
		{
			continue;
		}

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
			PIXEL_SOLID, FG_WHITE);

		DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
			triProjected.p[1].x, triProjected.p[1].y,
			triProjected.p[2].x, triProjected.p[2].y,
			PIXEL_SOLID, FG_CYAN);

	}

	return true;
}

int main()
{
	srEngine test("object.txt");

	if (test.ConstructConsole(512, 480, 2, 2))
		test.Start();
	else
		throw std::runtime_error("Unable to construct the console engine");
}