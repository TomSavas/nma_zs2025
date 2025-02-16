#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "model.h"
#include "vec.h"

MModel::MModel(const char *filename) : verts_(), faces_() {
    std::cout << "wtf is going on!!!!!!!!!!!!!! " << filename << std::endl;
    
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
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    std::cout << "!!!!!!!!# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

int MModel::nverts() {
    return (int)verts_.size();
}

int MModel::nfaces() {
    return (int)faces_.size();
}

std::vector<int> MModel::face(int idx) {
    return faces_[idx];
}

Vec3f MModel::vert(int i) {
    return verts_[i];
}


