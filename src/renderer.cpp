#include "renderer.h"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <iostream>

// Utility functions, feel free to ignore them (for now). 
// Stuff you need to work on starts with the label PART 1 :)

void setColor(RenderTarget& rt, int x, int y, Color color, float z = -std::numeric_limits<float>::max()) {
    if (0 < x && x < rt.width && 0 < y && y < rt.height) {
        // Generally y = 0 is at the top of the screen. This, however, might
        // be unusual for some. To avoid confusion let's flip y, s.t. it's at
        // the bottom of the screen.
        int flippedY = rt.height - y;
        int index = flippedY * rt.width + x;

        // TODO: Process z coordinate here when you work on the z buffer.
        rt.pixels[index] = color;
    }
}

void clear(RenderTarget& rt, Color color) {
    for (int y = 0; y < rt.height; y++) {
        for (int x = 0; x < rt.width; x++) {
            setColor(rt, x, y, color);

            // TODO: Surely you did not forget to also clear the zbuffer, right?
        }
    }
}

void render(Renderer& renderer) {
    clear(renderer.rt, BLACK);
    renderer.scene(renderer);
}

Vec3f worldToScreen(RenderTarget& rt, Vec3f world) {
    return Vec3f(
        (int)((world.x + 1.) * rt.width / 2.f),
        (int)((world.y + 1.) * rt.height / 2.f),
        world.z
    );
}

Color getDiffuse(Face& face, Vec3f bary) {
    Vec2f uv = face.points[0].uv * bary.x + face.points[1].uv * bary.y + face.points[2].uv * bary.z;

    float y = (int)(uv.y * (float)face.diffuseTexture.height);
    float x = (int)(uv.x * (float)face.diffuseTexture.width);
    int index = (face.diffuseTexture.height - y) * face.diffuseTexture.width + x;
    Color* data = (Color*)face.diffuseTexture.data;
    return data[index];
}

// Drawing of test scenes, feel free to ignore (for now).
// Stuff you need to work on starts with the label PART 1 :)

void drawTest(Renderer& renderer) {
    // Test lines
    renderer.drawLine(renderer.rt, Vec2i(50, 20), Vec2i(100, 400), RED);
    renderer.drawLine(renderer.rt, Vec2i(20, 80), Vec2i(400, 100), GREEN);
    renderer.drawLine(renderer.rt, Vec2i(200, 100), Vec2i(10, 200), BLUE);

    // Test triangles
    Face fakeFace = {};
    fakeFace.normal = renderer.lightDir;

    fakeFace.unshadedColor = RED;
    renderer.drawTriangle(renderer, Vec3f(210, 270), Vec3f(250, 360), Vec3f(270, 280), fakeFace);
    fakeFace.unshadedColor = GREEN;
    renderer.drawTriangle(renderer, Vec3f(380, 250), Vec3f(350, 200), Vec3f(270, 380), fakeFace);
    fakeFace.unshadedColor = BLUE;
    renderer.drawTriangle(renderer, Vec3f(380, 350), Vec3f(320, 360), Vec3f(330, 380), fakeFace);
}

void drawWireframeModel(Renderer& renderer) {
    for (int i = 0; i < renderer.model.faceIndices.size(); i++) {
        Face face = faceData(renderer.model, i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = face.points[j].worldPos;
            Vec3f v1 = face.points[(j + 1) % 3].worldPos;
            renderer.drawLine(renderer.rt, worldToScreen(renderer.rt, v0), worldToScreen(renderer.rt, v1), WHITE);
        }
    }
}

void drawRandomColorModel(Renderer& renderer) {
    // Re-seed every time so that the colors never change
    srand(0);

    for (int i = 0; i < renderer.model.faceIndices.size(); i++) { 
        Face face = faceData(renderer.model, i);

        Vec3f screenPoints[3];
        for (int j = 0; j < 3; j++) {
            screenPoints[j] = worldToScreen(renderer.rt, face.points[j].worldPos);
        }

        Vec3f normal = cross(face.points[2].worldPos - face.points[0].worldPos, face.points[1].worldPos - face.points[0].worldPos);
        normal = normalize(normal);

        Color randomColor = {
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(255)
        };

        // TODO: uncomment once you have something rendering
        // bool backface = normal.z > 0;
        // if (backface) continue;

        face.unshadedColor = randomColor;
        face.normal = -normal;
        renderer.drawTriangle(renderer, screenPoints[0], screenPoints[1], screenPoints[2], face);
    }
}

void drawModel(Renderer& renderer) {
    for (int i = 0; i < renderer.model.faceIndices.size(); i++) { 
        Face face = faceData(renderer.model, i);

        Vec3f screenPoints[3];
        for (int j = 0; j < 3; j++) {
            screenPoints[j] = worldToScreen(renderer.rt, face.points[j].worldPos);
        }

        Vec3f normal = cross(face.points[2].worldPos - face.points[0].worldPos, face.points[1].worldPos - face.points[0].worldPos);
        normal = normalize(normal);

        // TODO: uncomment once you have something rendering
        // bool backface = normal.z > 0;
        // if (backface) continue;

        face.unshadedColor = WHITE;
        face.normal = -normal;
        renderer.drawTriangle(renderer, screenPoints[0], screenPoints[1], screenPoints[2], face);
    }
}



// PART 1. Lines
// The following functions take in 2 points representing a line.
// The function needs draw the line using setColor function.
// Please ignore the z argument in setColor function for now.

void drawLine(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {
   // TODO: write any line drawing algorithm you can come up with .
}

void drawLineBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {
    // TODO: implement Bresenham's algorithm.
}

void drawLineOptimisedBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {
   // TODO: optimise the function above by eliminating divisions and floating point operations.
}

void drawLineWu(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {
    // TODO: outside the scope of the lecture, however feel free to look
    // into Wu's line drawing algorithm. It builds upon Bresenham's algorithm
    // and adds anti-aliasing.
}



// PART 2. Triangles
// The following functions take in 3 points representing 3 vertices of a triangle.
// The function needs to fill in the triangle.

void drawTriangleLineSweep(Renderer& renderer, Vec2i a, Vec2i b, Vec2i c, Face& face) {
    // TODO: implement triangle rendering using line sweep algorithm.
}

void drawTriangleBarycentric(Renderer& renderer, Vec2i a, Vec2i b, Vec2i c, Face& face) {
    // TODO: implement triangle rendering using barycentric coordinates.
}

void drawTriangle(Renderer& renderer, Vec3f a, Vec3f b, Vec3f c, Face& face) {
    // TODO: implement whichever algorithm again, however instead of using face.unshadedColor
    // use renderer.shade() this time around.

    // Additionally, use barycentric coordinates to calculate z coordinate and pass it
    // to the setColor function. For it to work properly you will need to add Z buffer
    // functionality to setColor and clear functions.
}



// PART 3. Shading
// The following functions take in Face data and barycentric coordinates of a point
// inside the triangle. The function needs to return the color of this given point.

Color noShading(Renderer& renderer, Face& face, Vec3f bary) {
    return face.unshadedColor;
}

Color flatShading(Renderer& renderer, Face& face, Vec3f bary) {
    // TODO: implement flat shading.
    return face.unshadedColor;
}

Color goroudShading(Renderer& renderer, Face& face, Vec3f bary) {
    // TODO: implement Goroud shading.
    return face.unshadedColor;
}

Color phongShading(Renderer& renderer, Face& face, Vec3f bary) {
    // TODO: implement Phong shading.
    return face.unshadedColor;
}
