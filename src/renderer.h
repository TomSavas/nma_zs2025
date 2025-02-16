#pragma once

#include "model.h"

#include "raylib.h"

struct RenderTarget {
    Color* pixels;
    int width;
    int height;

    float* zBuffer;
};

struct Renderer {
    RenderTarget rt;

    Vec3f lightDir;
    Color lightColor;

    MModel model;
    // If you need any data to persist between frames, put it here 
};

void render(Renderer& renderer);
