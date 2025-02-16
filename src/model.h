#pragma once

#include <vector>

#include "vec.h"

class MModel {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
public:
	MModel() = default;
	MModel(const char *filename);
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);
};
