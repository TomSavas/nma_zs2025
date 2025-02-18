#pragma once

#include "model.h"

#include "raylib.h"

#include <functional>

struct RenderTarget;
struct Renderer;

void render(Renderer& renderer);

// Part 1. Lines
void drawLine(RenderTarget& rt, Vec2i a, Vec2i b, Color color);
void drawLineBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color);
void drawLineOptimisedBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color);
// Left as an exercise to the interested reader: Wu's line drawing algorithm.
// Builds upon Bresenham's algorithm and adds anti-aliasing.
void drawLineWu(RenderTarget& rt, Vec2i a, Vec2i b, Color color);

// Part 2. Triangles
void drawTriangleLineSweep(Renderer& renderer, Vec2i a, Vec2i b, Vec2i c, Face& face);
void drawTriangleBarycentric(Renderer& renderer, Vec2i a, Vec2i b, Vec2i c, Face& face);
void drawTriangle(Renderer& renderer, Vec3f a, Vec3f b, Vec3f c, Face& face);

// Part 3. Shading
Color noShading(Renderer& renderer, Face& face, Vec3f bary);
Color flatShading(Renderer& renderer, Face& face, Vec3f bary);
Color goroudShading(Renderer& renderer, Face& face, Vec3f bary);
Color phongShading(Renderer& renderer, Face& face, Vec3f bary);

// Scenes
void drawTest(Renderer& renderer);
void drawWireframeModel(Renderer& renderer);
void drawRandomColorModel(Renderer& renderer);
void drawModel(Renderer& renderer);

struct RenderTarget {
    Color* pixels;
    int width;
    int height;

    float* zBuffer;
};

struct RendererOptions {
    bool moveLight = false;
    bool useZbuffer = false;
    bool doTexturing = false;

    // int line;
    // int triangle;
    // int shading;
    // int scene;

    // std::function<void(RenderTarget&, Vec2f, Vec2f, Color)> lineAlgs[] = {
    //     drawLine,
    //     drawLineBresenham,
    //     drawLineOptimisedBresenham,
    //     drawLineWu,
    // };
};

struct Renderer {
    RenderTarget rt;

    Vec3f lightDir;
    Color lightColor;

    TexturedModel model;

    std::function<void(RenderTarget&, Vec2f, Vec2f, Color)> drawLine;
    std::function<void(Renderer&, Vec3f, Vec3f, Vec3f, Face&)> drawTriangle;
    std::function<Color(Renderer&, Face&, Vec3f)> shade;
    std::function<void(Renderer&)> scene;

    // If you need any data to persist between frames, put it here 
};

