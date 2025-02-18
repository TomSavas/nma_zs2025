#pragma once

#include "vec.h"

#include "raylib.h"

#include <vector>

struct Vertex {
	Vec3f worldPos;
	Vec2f uv;
	Vec3f normal;
};

struct Face {
	Vertex points[3];
	Vec3f normal;
	Color unshadedColor = WHITE;
	Image diffuseTexture;
 };

struct FaceIndices {
	int vertIndex;
	int uvIndex;
	int normalIndex;
};

struct TexturedModel {
	std::vector<Vec2f> uvs;
	std::vector<Vec3f> normals;
	std::vector<Vec3f> vertices;
	std::vector<std::vector<FaceIndices>> faceIndices;
	Image diffuse;

	TexturedModel() = default;
	TexturedModel(const char* filename, const char* diffuseTextureFilename);
};
Face faceData(TexturedModel& model, int i);
