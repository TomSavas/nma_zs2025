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

        if (z >= rt.zBuffer[index]) {
            rt.zBuffer[index] = z;
            rt.pixels[index] = color;
        }
    }
}

void clear(RenderTarget& rt, Color color) {
    for (int y = 0; y < rt.height; y++) {
        for (int x = 0; x < rt.width; x++) {
            setColor(rt, x, y, color);

            int index = y * rt.width + x;
            rt.zBuffer[index] = -std::numeric_limits<float>::max();
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

        bool backface = normal.z > 0;
        if (backface) continue;

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

        bool backface = normal.z > 0;
        if (backface) continue;

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
    bool swapXY = std::abs(a.x - b.x) < std::abs(a.y - b.y);
    if (swapXY) {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }

    if (b.x < a.x) {
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }

    float xDiff = a.x - b.x;
    float yDiff = a.y - b.y;
    for (int x = a.x; x <= b.x; x++) {
        float linePercentage = (float)(x - a.x) / xDiff;
        int y = (yDiff * linePercentage) + a.y;
        
        if (swapXY) {
            setColor(rt, y, x, color);
        } else {
            setColor(rt, x, y, color);
        }
    }
}

void drawLineBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {
    bool swapXY = std::abs(a.x - b.x) < std::abs(a.y - b.y);
    if (swapXY) {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }

    if (b.x < a.x) {
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }

    float xDiff = a.x - b.x;
    float yDiff = a.y - b.y;
    float slope = std::abs(yDiff / xDiff);

    int y = a.y;
    float yProgress = 0;
    for (int x = a.x; x <= b.x; x++) {
        if (swapXY) {
            setColor(rt, y, x, color);
        } else {
            setColor(rt, x, y, color);
        }

        yProgress += slope;
        if (yProgress > 0.5) {
            y += ((a.y < b.y) ? 1 : -1);
            yProgress -= 1.0;
        }
    }
}

void drawLineOptimisedBresenham(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {
    int xDiff = std::abs(a.x - b.x);
    int yDiff = std::abs(a.y - b.y);

    bool swapXY = xDiff < yDiff;
    if (swapXY) {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
        std::swap(xDiff, yDiff);
    }

    if (b.x < a.x) {
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }

    int slope2 = yDiff * 2;
    int y = a.y;
    int yProgress = 0;
    int xDiff2 = xDiff*2;
    int yInc = ((a.y < b.y) ? 1 : -1);

    if (swapXY) {
        for (int x = a.x; x <= b.x; x++) {
            setColor(rt, y, x, color);
            yProgress += slope2;
            if (yProgress > xDiff) {
                y += yInc;
                yProgress -= xDiff2;
            }
        }
    } else {
        for (int x = a.x; x <= b.x; x++) {
            setColor(rt, x, y, color);
            yProgress += slope2;
            if (yProgress > xDiff) {
                y += yInc;
                yProgress -= xDiff2;
            }
        }
    }
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
    // Sort a-to-b
    if (a[1] > b[1]) std::swap(a, b); 
    if (a[1] > c[1]) std::swap(a, c); 
    if (b[1] > c[1]) std::swap(b, c); 

    int acLength = c[1] - a[1];
    if (acLength <= 0) return;

    int abLength = b[1] - a[1];
    if (abLength > 0) {
        for (int y = std::max(a[1], 0); y <= std::min(b[1], renderer.rt.height); y++) {
            float abProgress = (float)(y - a[1]) / abLength;
            float acProgress = (float)(y - a[1]) / acLength;

            int abX = a[0] + abProgress * (b[0] - a[0]);
            int acX = a[0] + acProgress * (c[0] - a[0]);
            if (abX < acX) {
                std::swap(abX, acX);
            }
            for (int x = std::max(acX, 0); x <= std::min(abX, renderer.rt.width); x++) {
                setColor(renderer.rt, x, y, face.unshadedColor);
            }
        }
    }
   
    int bcLength = c[1] - b[1];
    if (bcLength > 0) {
        for (int y = std::max(b[1], 0); y <= std::min(c[1], renderer.rt.height); y++) {
            float bcProgress = (float)(y - b[1]) / bcLength;
            float acProgress = (float)(y - a[1]) / acLength;

            int bcX = b[0] + bcProgress * (c[0] - b[0]);
            int acX = a[0] + acProgress * (c[0] - a[0]);
            if (acX < bcX) {
                std::swap(acX, bcX);
            }
            for (int x = std::max(bcX, 0); x <= std::min(acX, renderer.rt.width); x++) {
                setColor(renderer.rt, x, y, face.unshadedColor);
            }
        }
    }
}

void drawTriangleBarycentric(Renderer& renderer, Vec2i a, Vec2i b, Vec2i c, Face& face) {
    float minX = std::min(a.x, std::min(b.x, c.x));
    float minY = std::min(a.y, std::min(b.y, c.y));
    float maxX = std::max(a.x, std::max(b.x, c.x));
    float maxY = std::max(a.y, std::max(b.y, c.y));

    int yBound = std::clamp((int)maxY, 0, renderer.rt.height);
    int xBound = std::clamp((int)maxX, 0, renderer.rt.width);

    for (int y = std::clamp((int)minY, 0, renderer.rt.height); y <= yBound; y++) {
        bool startedDrawingTriangle = false;

        for (int x = std::clamp((int)minX, 0, renderer.rt.width); x <= xBound; x++) {
            Vec3f bary = barycentric(x, y, a, b, c);
            bool insideTriangle = bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0;

            if (insideTriangle) {
                startedDrawingTriangle = true;
                setColor(renderer.rt, x, y, face.unshadedColor);
            } else if(startedDrawingTriangle) {
                break;
            }
        }
    }
}

void drawTriangle(Renderer& renderer, Vec3f a, Vec3f b, Vec3f c, Face& face) {
    float minX = std::min(a.x, std::min(b.x, c.x));
    float minY = std::min(a.y, std::min(b.y, c.y));
    float maxX = std::max(a.x, std::max(b.x, c.x));
    float maxY = std::max(a.y, std::max(b.y, c.y));

    
    int yBound = std::clamp((int)maxY, 0, renderer.rt.height);
    int xBound = std::clamp((int)maxX, 0, renderer.rt.width);

    for (int y = std::clamp((int)minY, 0, renderer.rt.height); y <= yBound; y++) {
        bool startedDrawingTriangle = false;

        for (int x = std::clamp((int)minX, 0, renderer.rt.width); x <= xBound; x++) {
            Vec3f bary = barycentric(x, y, a, b, c);
            bool insideTriangle = bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0;

            if (insideTriangle) {
                startedDrawingTriangle = true;

                float z = bary[0] * a.z + bary[1] * b.z + bary[2] * c.z;
                Color color = renderer.shade(renderer, face, bary);

                setColor(renderer.rt, x, y, color, z);
            } else if(startedDrawingTriangle) {
                break;
            }
        }
    }
}



// PART 3. Shading
// The following functions take in Face data and barycentric coordinates of a point
// inside the triangle. The function needs to return the color of this given point.

Color noShading(Renderer& renderer, Face& face, Vec3f bary) {
    return face.unshadedColor;
}

Color flatShading(Renderer& renderer, Face& face, Vec3f bary) {
    float lightIntensity = dot(-renderer.lightDir, face.normal);
    lightIntensity = std::clamp(lightIntensity, 0.f, 1.f);

    Color color = face.unshadedColor;
    // Color color = getDiffuse(face, bary);
    return Color {
        static_cast<unsigned char>(color.r * lightIntensity),
        static_cast<unsigned char>(color.g * lightIntensity),
        static_cast<unsigned char>(color.b * lightIntensity),
        static_cast<unsigned char>(255)
    };
}

Color goroudShading(Renderer& renderer, Face& face, Vec3f bary) {
    // Usually done once per triangle. So shouldn't live here, but rather outside the pixel drawing loop...
    // (it's here just for the sake of simplicity)
    float diffuse[3];
    float specular[3];
    for (int i = 0; i < 3; i++)
    {
        Vec3f normal = face.points[i].normal;

        normal = normalize(normal);
        diffuse[i] = dot(-renderer.lightDir, normal);

        Vec3f reflected = renderer.lightDir - (normal * 2.f * dot(normal, renderer.lightDir));
        specular[i] = std::pow(std::max(dot(Vec3f(0, 0, 1), reflected), 0.f), 32.f);
    }

    float interpDiffuse = diffuse[0] * bary.x + diffuse[1] * bary.y + diffuse[2] * bary.z;
    float interpSpecular = specular[0] * bary.x + specular[1] * bary.y + specular[2] * bary.z;
    float lighting = std::clamp(interpDiffuse + interpSpecular * 0.5f, 0.f, 1.f);

    Color color = face.unshadedColor;
    // Color color = getDiffuse(face, bary);
    return Color {
        static_cast<unsigned char>(color.r * lighting),
        static_cast<unsigned char>(color.g * lighting),
        static_cast<unsigned char>(color.b * lighting),
        static_cast<unsigned char>(255)
    };
}

// If you don't think there is a difference between Goroud and Phong, turn off the texture
// (just use face.unshadedColor), check the "Move light" checkbox and pay close attention
// to the specular hightlights (the brightest spot in the image that moves along with the light).
// Phong version looks smooth, meanwhile Goroud is not and you can clearly see the triangle boundaries.
Color phongShading(Renderer& renderer, Face& face, Vec3f bary) {
    Vec3f normal = face.points[0].normal * bary.x + 
               face.points[1].normal * bary.y +
               face.points[2].normal * bary.z;
    normal = normalize(normal);

    float diffuseLighting = dot(-renderer.lightDir, normal);

    Vec3f reflected = renderer.lightDir - (normal * 2.f * dot(normal, renderer.lightDir));
    float specularLighting = std::pow(std::max(dot(Vec3f(0, 0, 1), reflected), 0.f), 32.f);

    float lighting = std::clamp(diffuseLighting + specularLighting * 0.5f, 0.f, 1.f);

    Color color = face.unshadedColor;
    // Color color = getDiffuse(face, bary);
    return Color {
        static_cast<unsigned char>(color.r * lighting),
        static_cast<unsigned char>(color.g * lighting),
        static_cast<unsigned char>(color.b * lighting),
        static_cast<unsigned char>(255)
    };
}
