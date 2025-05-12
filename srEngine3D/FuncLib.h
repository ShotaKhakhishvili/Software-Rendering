#pragma once

#include "Structures.h"
#include <fstream>
#include <string>
#include <strstream>

void MultiplyMatrixVector(const vec3& in, vec3& out, const mat4x4& mat)
{
	out.x = in.x * mat.m[0][0] + in.y * mat.m[1][0] + in.z * mat.m[2][0] + mat.m[3][0];
	out.y = in.x * mat.m[0][1] + in.y * mat.m[1][1] + in.z * mat.m[2][1] + mat.m[3][1];
	out.z = in.x * mat.m[0][2] + in.y * mat.m[1][2] + in.z * mat.m[2][2] + mat.m[3][2];
	float w = in.x * mat.m[0][3] + in.y * mat.m[1][3] + in.z * mat.m[2][3] + mat.m[3][3];

	if (w != 0)
	{
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}
}

std::string GetPrefixBeforeSlash(const std::string& s) {
	size_t pos = s.find('/');
	if (pos == std::string::npos)
		return s; // No '/' found, return the whole string
	return s.substr(0, pos);
}

void ReadFromObj(std::string filename, mesh& targetMesh)
{
	std::ifstream f{ filename };

	if (!f.is_open())
	{
		throw std::runtime_error("Could not read an obj file");
	}

	std::vector<vec3> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream lineString;

		lineString << line;

		std::string cmd;

		lineString >> cmd;

		if (cmd == "v")
		{
			vec3 vert;

			lineString >> vert.x >> vert.y >> vert.z;
			verts.push_back(vert);
		}

		if (cmd == "f")
		{
			std::string v1, v2, v3;

			lineString >> v1 >> v2 >> v3;

			v1 = GetPrefixBeforeSlash(v1);
			v2 = GetPrefixBeforeSlash(v2);
			v3 = GetPrefixBeforeSlash(v3);

			targetMesh.tris.push_back({ verts[stoi(v1) - 1] , verts[stoi(v2) - 1], verts[stoi(v3) - 1] });

			if (!lineString.eof()) {
				std::string v4;
				lineString >> v4;

				v4 = GetPrefixBeforeSlash(v4);
				targetMesh.tris.push_back({ verts[stoi(v1) - 1] , verts[stoi(v3) - 1], verts[stoi(v4) - 1] });
			}
		}
	}
}

// Taken From Command Line Webcam Video
CHAR_INFO GetColour(float lum)
{
	short bg_col, fg_col;
	wchar_t sym;
	int pixel_bw = (int)(13.0f * lum);
	switch (pixel_bw)
	{
	case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

	case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
	case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
	case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

	case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
	case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
	case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

	case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
	case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
	case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
	case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
	default:
		bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
	}

	CHAR_INFO c;
	c.Attributes = bg_col | fg_col;
	c.Char.UnicodeChar = sym;
	return c;
}

