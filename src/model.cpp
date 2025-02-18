#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "model.h"
#include "vec.h"

TexturedModel::TexturedModel(const char* filename, const char* diffuseTextureFilename) {// : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            vertices.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash;
            iss >> trash;
            Vec2f v;
            for (int i=0;i<2;i++) iss >> v.raw[i];
            float trashf;
            iss >> trashf;
            uvs.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash;
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            normals.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            std::vector<FaceIndices> fIndices;
            int vertIndex;
            int uvIndex;
            int normalIndex;
            iss >> trash;
            while (iss >> vertIndex >> trash >> uvIndex >> trash >> normalIndex) {
                f.push_back(vertIndex - 1);
                fIndices.push_back(FaceIndices{vertIndex - 1, uvIndex - 1, normalIndex - 1});
            }
            faceIndices.push_back(fIndices);
        }
    }

    diffuse = LoadImage(diffuseTextureFilename);
}

Face faceData(TexturedModel& model, int i) {
    Face f;
    f.diffuseTexture = model.diffuse;
    for (int j = 0; j < 3; j++) {
        FaceIndices& indices = model.faceIndices[i][j];
        f.points[j].worldPos = model.vertices[indices.vertIndex];
        f.points[j].uv = model.uvs[indices.uvIndex];
        f.points[j].normal = model.normals[indices.normalIndex];
    }

    return f;
}

