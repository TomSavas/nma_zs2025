#pragma once

#include "model.h"

#include "raylib.h"

#include <functional>

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

    std::function<void(RenderTarget&, Vec2f, Vec2f, Color)> drawLine;
    std::function<void(RenderTarget&, Vec3f, Vec3f, Vec3f, Color)> drawTriangle;

    std::function<Color(RenderTarget&, Vec3f, Vec3f, Vec3f, Color)> shade;
    std::function<void(Renderer&)> scene;

    // If you need any data to persist between frames, put it here 
};

void render(Renderer& renderer);

// Part 1. Lines
void drawLine(RenderTarget& rt, Vec2i a, Vec2i b, Color color);
void drawLineBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color);
void drawLineOptimisedBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color);
// Left as an exercise to the interested reader: Wu's line drawing algorithm.
// Builds upon Bresenham's algorithm and adds anti-aliasing.
void drawLineWu(RenderTarget& rt, Vec2i a, Vec2i b, Color color);

// Part 2. Triangles
void drawTriangle(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color);
void drawTriangleLineSweep(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color);
void drawTriangleBarycentric(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color);

// Part 3. Shading
void drawTriangleShaded(RenderTarget& rt, Vec3f a, Vec3f b, Vec3f c);

void flatShading();
void goroudShading();
void phongShading();
void toonShading();

// Scenes
void drawTestLines(Renderer& renderer);
void drawTestTriangles(Renderer& renderer);
void drawWireframeModel(Renderer& renderer);
void drawRandomColorModel(Renderer& renderer);
void drawModel(Renderer& renderer);
