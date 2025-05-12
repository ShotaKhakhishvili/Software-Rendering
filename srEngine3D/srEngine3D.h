#pragma once

#include "olcConsoleGameEngine.h"
#include "Structures.h"

class srEngine : public olcConsoleGameEngine
{
private:
	mesh mesh;
	mat4x4 mat_proj;
	mat4x4 rotZ, rotY;

	vec3 vCamera;

	float rotAngle = 0.0f;

public:
	srEngine(std::string objSrc);

	bool OnUserCreate()override;

	bool OnUserUpdate(float deltaTime) override;
};