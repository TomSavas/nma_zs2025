#include "renderer.h"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <iostream>

void setColor(RenderTarget& rt, int x, int y, Color color) {
    if (0 < x && x < rt.width && 0 < y && y < rt.height) {
        y = rt.height - y;
        rt.pixels[y * rt.width + x] = color;
    }
}

void setColorDepth(RenderTarget& rt, int x, int y, float z, Color color) {
    if (0 < x && x < rt.width && 0 < y && y < rt.height) {
        y = rt.height - y;
        int index = y * rt.width + x;

        if (z > rt.zBuffer[index]) {
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

void drawLine(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
    int xDiff = std::abs(x0 - x1);
    int yDiff = std::abs(y0 - y1);

    const bool swapXY = xDiff < yDiff;
    if (swapXY) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(xDiff, yDiff);
    }

    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++)
    {
        float linePercentage = (float)(x - x0) / xDiff;
        int y = ((float)yDiff * linePercentage) + y0;
        
        if (swapXY) {
            setColor(rt, y, x, color);
        } else {
            setColor(rt, x, y, color);
        }
    }
}

void drawLineBresenham0(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
    int xDiff = std::abs(x0 - x1);
    int yDiff = std::abs(y0 - y1);

    const bool swapXY = xDiff < yDiff;
    if (swapXY) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(xDiff, yDiff);
    }

    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float slope = (float)yDiff / xDiff;
    for (int x = x0; x <= x1; x++)
    {
        int y = y0 + (float)(x) * slope;
        
        if (swapXY) {
            setColor(rt, y, x, color);
        } else {
            setColor(rt, x, y, color);
        }
    }
}

void drawLineBresenham1(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
    int xDiff = std::abs(x0 - x1);
    int yDiff = std::abs(y0 - y1);

    const bool swapXY = xDiff < yDiff;
    if (swapXY) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(xDiff, yDiff);
    }

    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float slope = (float)yDiff / xDiff;
    int y = y0;
    float yProgress = 0;
    for (int x = x0; x <= x1; x++)
    {
        if (swapXY) {
            setColor(rt, y, x, color);
        } else {
            setColor(rt, x, y, color);
        }

        yProgress += slope;
        if (yProgress > 0.5) {
            y += ((y0 < y1) ? 1 : -1);
            yProgress -= 1.0;
        }
    }
}

void drawLineBresenham2(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
    int xDiff = std::abs(x0 - x1);
    int yDiff = std::abs(y0 - y1);

    const bool swapXY = xDiff < yDiff;
    if (swapXY) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(xDiff, yDiff);
    }

    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int slope2 = yDiff * 2;
    int y = y0;
    int yProgress = 0;
    int xDiff2 = xDiff*2;
    for (int x = x0; x <= x1; x++)
    {
        if (swapXY) {
            setColor(rt, y, x, color);
        } else {
            setColor(rt, x, y, color);
        }

        yProgress += slope2;
        if (yProgress > xDiff) {
            y += ((y0 < y1) ? 1 : -1);
            yProgress -= xDiff2;
        }
    }
}

void drawLineBresenham3(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
    int xDiff = std::abs(x0 - x1);
    int yDiff = std::abs(y0 - y1);

    const bool swapXY = xDiff < yDiff;
    if (swapXY) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(xDiff, yDiff);
    }

    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int slope2 = yDiff * 2;
    int y = y0;
    int yProgress = 0;
    int xDiff2 = xDiff*2;
    int yInc = ((y0 < y1) ? 1 : -1);
    if (swapXY) {
        for (int x = x0; x <= x1; x++) {
            setColor(rt, y, x, color);
            yProgress += slope2;
            if (yProgress > xDiff) {
                y += yInc;
                yProgress -= xDiff2;
            }
        }
    } else {
        for (int x = x0; x <= x1; x++) {
            setColor(rt, x, y, color);
            yProgress += slope2;
            if (yProgress > xDiff) {
                y += yInc;
                yProgress -= xDiff2;
            }
        }
    }
}

void crossProduct(float x0, float y0, float z0, float x1, float y1, float z1, float* cross) {
    cross[0] = y0 * z1 - z0 * y1;
    cross[1] = z0 * x1 - x0 * z1;
    cross[2] = x0 * y1 - y0 * x1;
}

void calculateBarycentric(int x, int y, int a[2], int b[2], int c[2], float* barycentric) {
    float cross[3];
    crossProduct((float)c[0]-a[0], (float)b[0]-a[0], (float)a[0]-x, // vector 1
                 (float)c[1]-a[1], (float)b[1]-a[1], (float)a[1]-y, // vector 2
                 cross);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(cross[2])<1) {
        barycentric[0] = -1.f;
        barycentric[1] = 1.f;
        barycentric[2] = 1.f;
    } else {
        barycentric[0] = 1.f - (cross[0] + cross[1]) / cross[2]; 
        barycentric[1] = cross[1] / cross[2];
        barycentric[2] = cross[0] / cross[2];
    }
}

Vec3f calculateBarycentric(int x, int y, Vec3f a, Vec3f b, Vec3f c) {
    float cross[3];
    crossProduct((float)c[0]-a[0], (float)b[0]-a[0], (float)a[0]-x, // vector 1
                 (float)c[1]-a[1], (float)b[1]-a[1], (float)a[1]-y, // vector 2
                 cross);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(cross[2])<1) {
        return Vec3f(-1.f, 1.f, 1.f);
    } else {
        return Vec3f(
            1.f - (cross[0] + cross[1]) / cross[2],
            cross[1] / cross[2],
            cross[0] / cross[2]
        );
    }
}


void drawTriangleLineSweep_tiny(RenderTarget& rt, int a[2], int b[2], int c[2], Color color) {
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
    if (a[1]>b[1]) std::swap(a, b); 
    if (a[1]>c[1]) std::swap(a, c); 
    if (b[1]>c[1]) std::swap(b, c); 
    int total_height = c[1]-a[1]; 
    for (int y=a[1]; y<=b[1]; y++) { 
        int segment_height = b[1]-a[1]+1; 
        float alpha = (float)(y-a[1])/total_height; 
        float beta  = (float)(y-a[1])/segment_height; // be careful with divisions by zero 

        int A[2] = {(int)(a[0] + (c[0] - a[0]) * alpha), (int)(a[1] + (c[1] - a[1]) * alpha)};
        int B[2] = {(int)(a[0] + (b[0] - a[0]) * beta), (int)(a[1] + (b[1] - a[1]) * beta)};
        // Vec2i A = a + (c-a)*alpha; 
        // Vec2i B = a + (b-a)*beta; 

        if (A[0]>B[0]) std::swap(A, B); 
        for (int j=A[0]; j<=B[0]; j++) { 
            // image.set(j, y, color); // attention, due to int casts t0[1]+i != A.y 
            setColor(rt, j, y, color);
        } 
    } 
    for (int y=b[1]; y<=c[1]; y++) { 
        int segment_height =  c[1]-b[1]+1; 
        float alpha = (float)(y-a[1])/total_height; 
        float beta  = (float)(y-b[1])/segment_height; // be careful with divisions by zero 
        // Vec2i A = a + (c-a)*alpha; 
        // Vec2i B = b + (c-b)*beta; 
        int A[2] = {(int)(a[0] + (c[0] - a[0]) * alpha), (int)(a[1] + (c[1] - a[1]) * alpha)};
        int B[2] = {(int)(b[0] + (c[0] - b[0]) * beta), (int)(b[1] + (c[1] - b[1]) * beta)};
        if (A[0]>B[0]) std::swap(A, B); 
        for (int j=A[0]; j<=B[0]; j++) { 
            // image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
            setColor(rt, j, y, color);
        } 
    } 
}

void drawTriangleLineSweep(RenderTarget& rt, int a[2], int b[2], int c[2], Color color) {
    // Sort a-to-b
    if (a[1] > b[1]) std::swap(a, b); 
    if (a[1] > c[1]) std::swap(a, c); 
    if (b[1] > c[1]) std::swap(b, c); 

    int acLength = c[1] - a[1];
    if (acLength <= 0) return;

    int abLength = b[1] - a[1];
    if (abLength > 0) {
        for (int y = std::max(a[1], 0); y <= std::min(b[1], rt.height); y++) {
            float abProgress = (float)(y - a[1]) / abLength;
            float acProgress = (float)(y - a[1]) / acLength;

            int abX = a[0] + abProgress * (b[0] - a[0]);
            int acX = a[0] + acProgress * (c[0] - a[0]);
            if (abX < acX) {
                std::swap(abX, acX);
            }
            for (int x = std::max(acX, 0); x <= std::min(abX, rt.width); x++) {
                setColor(rt, x, y, color);
            }
        }
    }
   
    int bcLength = c[1] - b[1];
    if (bcLength > 0) {
        for (int y = std::max(b[1], 0); y <= std::min(c[1], rt.height); y++) {
            float bcProgress = (float)(y - b[1]) / bcLength;
            float acProgress = (float)(y - a[1]) / acLength;

            int bcX = b[0] + bcProgress * (c[0] - b[0]);
            int acX = a[0] + acProgress * (c[0] - a[0]);
            if (acX < bcX) {
                std::swap(acX, bcX);
            }
            for (int x = std::max(bcX, 0); x <= std::min(acX, rt.width); x++) {
                setColor(rt, x, y, color);
            }
        }
    }
}

void drawTriangleBarycentric(RenderTarget& rt, int a[2], int b[2], int c[2], Color color) {
    float minX = std::min(a[0], std::min(b[0], c[0]));
    float minY = std::min(a[1], std::min(b[1], c[1]));

    float maxX = std::max(a[0], std::max(b[0], c[0]));
    float maxY = std::max(a[1], std::max(b[1], c[1]));

    int yBound = std::clamp((int)maxY, 0, rt.height);
    for (int y = std::clamp((int)minY, 0, rt.height); y <= yBound; y++) {
        // Ignore the point if it's out of viewport's bounds
        if (y < 0 || y >= rt.height)
            continue;

        bool started_drawing_triangle = false;
        int xBound = std::clamp((int)maxX, 0, rt.width);
        for (int x = std::clamp((int)minX, 0, rt.width); x <= xBound; x++) {
            // Ignore the point if it's out of viewport's bounds
            // if (x < 0 || x >= rt.width)
            //     continue;

            float barycentric[3];
            calculateBarycentric(x, y, a, b, c, barycentric);
            const bool insideTriangle = barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0;
            if (!insideTriangle) {
                // If we were drawing a triangle on this line but are no more, then we can
                // just jump to another line, as there definitely won't be any more of this
                // triangle on this line
                if (started_drawing_triangle)
                    break;

                continue;
            }
            started_drawing_triangle = true;

            // Point is already in ndc, so we have already "projected into 3d",
            // hence we don't need division by w.
            // float z = barycentric.X() * v0.Z() + barycentric.Y() * v1.Z() + barycentric.Z() * v2.Z();

            // int depth_buf_index = BufIndex(x, y);
            // if (z < depth_buf[depth_buf_index])
                // continue;

            // bool should_render;
            // Color color;
            // std::tie(should_render, color) = shader.Fragment(barycentric, face);

            // if (should_render) {
                // depth_buf[depth_buf_index] = z;
                // framebuf.Set(x, y, color);
            // }

            setColor(rt, x, y, color);
        }
    }
}

void drawWireframeModel(RenderTarget& rt, MModel& model) {
    for (int i = 0; i < model.nfaces(); i++) {
        std::vector<int> face = model.face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * rt.width / 2.;
            int y0 = (v0.y + 1.) * rt.height / 2.;
            int x1 = (v1.x + 1.) * rt.width / 2.;
            int y1 = (v1.y + 1.) * rt.height / 2.;

            drawLineBresenham3(rt, x0, y0, x1, y1, WHITE);
        }
    }
}

void drawColouredModel(RenderTarget& rt, MModel& model) {
    for (int i=0; i<model.nfaces(); i++) { 
        std::vector<int> face = model.face(i);
        int coords[3][2];
        for (int j = 0; j < 3; j++) {
            Vec3f world_coords = model.vert(face[j]);
            coords[j][0] = (int)((world_coords.x + 1.) * rt.width / 2.f);
            coords[j][1] = (int)((world_coords.y + 1.) * rt.height / 2.f);
        }

        // drawTriangleBarycentric(rt, coords[0], coords[1], coords[2], Color{rand() % 255, rand() % 255, rand() % 255, 255});
        drawTriangleLineSweep(rt, coords[0], coords[1], coords[2], Color{rand() % 255, rand() % 255, rand() % 255, 255});
        // drawTriangleLineSweep_tiny(rt, coords[0], coords[1], coords[2], Color{rand() % 255, rand() % 255, rand() % 255, 255});
        // drawTriangleBarycentric(rt, coords[0], coords[1], coords[2], RED);
    }
}

void drawFlatShadedModel(RenderTarget& rt, Vec3f lightDir, Color lightColor, MModel& model) {
    for (int i=0; i<model.nfaces(); i++) { 
        std::vector<int> face = model.face(i);
        int coords[3][2];
        Vec3f worldCoords[3];
        for (int j = 0; j < 3; j++) {
            worldCoords[j] = model.vert(face[j]);
            coords[j][0] = (int)((worldCoords[j].x + 1.) * rt.width / 2.f);
            coords[j][1] = (int)((worldCoords[j].y + 1.) * rt.height / 2.f);
        }

        lightDir = Vec3f(0, 0, -1);
        Vec3f normal = cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]);
        normal = normalize(normal);
        float lightIntensity = dot(lightDir, normal);
        Color color = Color{lightColor.r * lightIntensity, lightColor.g * lightIntensity, lightColor.b * lightIntensity, 255};

        // Extremely scuffed backface culling
        if (lightIntensity > 0) {
            drawTriangleLineSweep(rt, coords[0], coords[1], coords[2], color);
            // drawTriangleBarycentric(rt, coords[0], coords[1], coords[2], color);
        }
    }
}

void drawTriangleShaded(RenderTarget& rt, Vec3f a, Vec3f b, Vec3f c, Color color) {
    float minX = std::min(a[0], std::min(b[0], c[0]));
    float minY = std::min(a[1], std::min(b[1], c[1]));

    float maxX = std::max(a[0], std::max(b[0], c[0]));
    float maxY = std::max(a[1], std::max(b[1], c[1]));

    int yBound = std::clamp((int)maxY, 0, rt.height);
    for (int y = std::clamp((int)minY, 0, rt.height); y <= yBound; y++) {
        // Ignore the point if it's out of viewport's bounds
        if (y < 0 || y >= rt.height)
            continue;

        bool started_drawing_triangle = false;
        int xBound = std::clamp((int)maxX, 0, rt.width);
        for (int x = std::clamp((int)minX, 0, rt.width); x <= xBound; x++) {
            Vec3f barycentric = calculateBarycentric(x, y, a, b, c);
            const bool insideTriangle = barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0;
            if (!insideTriangle) {
                if (started_drawing_triangle)
                    break;

                continue;
            }
            started_drawing_triangle = true;

            // Point is already in ndc, so we have already "projected into 3d",
            // hence we don't need division by w.
            float z = barycentric[0] * a.z + barycentric[1] * b.z + barycentric[2] * c.z;
            // setColorDepth(rt, x, y, z, color);

            // float visZ = z * 0.5 + 0.5;
            // color = Color{255 * visZ, 255 * visZ, 255 * visZ, 255};
            setColorDepth(rt, x, y, z, color);
        }
    }
}

Vec3f worldToScreen(RenderTarget& rt, Vec3f world) {
    return Vec3f(
        (int)((world.x + 1.) * rt.width / 2.f),
        (int)((world.y + 1.) * rt.height / 2.f),
        world.z
    );
}

void drawShaded(RenderTarget& rt, Vec3f lightDir, Color lightColor, MModel& model) {
    for (int i=0; i<model.nfaces(); i++) { 
        std::vector<int> face = model.face(i);
        // int coords[3][2];
        Vec3f worldCoords[3];
        for (int j = 0; j < 3; j++) {
            worldCoords[j] = model.vert(face[j]);
        }

        Vec3f normal = cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]);
        normal = normalize(normal);
        Vec3f view = Vec3f(0, 0, -1);
        float backface = dot(view, normal);

        if (backface < 0) continue;
        
        float lightIntensity = dot(lightDir, normal);
        lightIntensity = std::clamp(lightIntensity, 0.f, 1.f);
        Color color = Color{lightColor.r * lightIntensity, lightColor.g * lightIntensity, lightColor.b * lightIntensity, 255};
        drawTriangleShaded(rt, worldToScreen(rt, worldCoords[0]), worldToScreen(rt, worldCoords[1]), worldToScreen(rt, worldCoords[2]), color);
    }
}

void render(Renderer& renderer) {
    RenderTarget& rt = renderer.rt;

    clear(rt, BLACK);

    drawLineBresenham3(rt, 0, 50, 1000, 100, BLUE);
    drawLineBresenham3(rt, 20, 100, 100, 1000, RED);

    int t0[3][2] = {{10, 70}, {50, 160}, {70, 80}}; 
    int t1[3][2] = {{180, 50},  {150, 1}, {70, 180}}; 
    int t2[3][2] = {{180, 150}, {120, 160}, {130, 180}}; 
    drawTriangleBarycentric(rt, t0[0], t0[1], t0[2], RED); 
    drawTriangleBarycentric(rt, t1[0], t1[1], t1[2], WHITE); 
    drawTriangleBarycentric(rt, t2[0], t2[1], t2[2], GREEN);

    // drawWireframeModel(rt, renderer.model);
    // drawColouredModel(rt, renderer.model);
    // drawFlatShadedModel(rt, renderer.lightDir, renderer.lightColor, renderer.model);
    drawShaded(rt, renderer.lightDir, renderer.lightColor, renderer.model);
}



void drawTestLines(Renderer& renderer) {}
void drawTriangles(Renderer& renderer) {}
void drawModel(Renderer& renderer) {}


void drawLine() {}
void drawLineBresenham() {}

void drawTriangle() {}
void drawTriangleLineSweep() {}
void drawTriangleBarycentric() {}

void flatShading() {}
void goroudShading() {}
void phongShading() {}
void toonShading() {}

void drawModel() {}
