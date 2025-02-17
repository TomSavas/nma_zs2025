#include "renderer.h"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <iostream>

void setColor(RenderTarget& rt, int x, int y, Color color, float z = -std::numeric_limits<float>::max()) {
    if (0 < x && x < rt.width && 0 < y && y < rt.height) {
        // Generally y = 0 is at the top of the screen. This, however, might
        // be unusual for some. To avoid confusion let's flip y, s.t. it's at
        // the bottom of the screen.
        int flippedY = rt.height - y;
        int index = flippedY * rt.width + x;

        // TODO: Process z coordinate here when you work on the z buffer.
        if (z >= rt.zBuffer[index]) {
            rt.zBuffer[index] = z;
            rt.pixels[index] = color;
        }
        // rt.pixels[index] = color;
    }
}

void clear(RenderTarget& rt, Color color) {
    for (int y = 0; y < rt.height; y++) {
        for (int x = 0; x < rt.width; x++) {
            setColor(rt, x, y, color);

            // TODO: Surely you did not forget to also clear the zbuffer, right?
            int index = y * rt.width + x;
            rt.zBuffer[index] = -std::numeric_limits<float>::max();
        }
    }
}

// void drawLine(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
//     int xDiff = std::abs(x0 - x1);
//     int yDiff = std::abs(y0 - y1);

//     const bool swapXY = xDiff < yDiff;
//     if (swapXY) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         std::swap(xDiff, yDiff);
//     }

//     if (x1 < x0) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     for (int x = x0; x <= x1; x++)
//     {
//         float linePercentage = (float)(x - x0) / xDiff;
//         int y = ((float)yDiff * linePercentage) + y0;
        
//         if (swapXY) {
//             setColor(rt, y, x, color);
//         } else {
//             setColor(rt, x, y, color);
//         }
//     }
// }

// void drawLineBresenham0(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
//     int xDiff = std::abs(x0 - x1);
//     int yDiff = std::abs(y0 - y1);

//     const bool swapXY = xDiff < yDiff;
//     if (swapXY) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         std::swap(xDiff, yDiff);
//     }

//     if (x1 < x0) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     float slope = (float)yDiff / xDiff;
//     for (int x = x0; x <= x1; x++)
//     {
//         int y = y0 + (float)(x) * slope;
        
//         if (swapXY) {
//             setColor(rt, y, x, color);
//         } else {
//             setColor(rt, x, y, color);
//         }
//     }
// }

// void drawLineBresenham1(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
//     int xDiff = std::abs(x0 - x1);
//     int yDiff = std::abs(y0 - y1);

//     const bool swapXY = xDiff < yDiff;
//     if (swapXY) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         std::swap(xDiff, yDiff);
//     }

//     if (x1 < x0) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     float slope = (float)yDiff / xDiff;
//     int y = y0;
//     float yProgress = 0;
//     for (int x = x0; x <= x1; x++)
//     {
//         if (swapXY) {
//             setColor(rt, y, x, color);
//         } else {
//             setColor(rt, x, y, color);
//         }

//         yProgress += slope;
//         if (yProgress > 0.5) {
//             y += ((y0 < y1) ? 1 : -1);
//             yProgress -= 1.0;
//         }
//     }
// }

// void drawLineBresenham2(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
//     int xDiff = std::abs(x0 - x1);
//     int yDiff = std::abs(y0 - y1);

//     const bool swapXY = xDiff < yDiff;
//     if (swapXY) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         std::swap(xDiff, yDiff);
//     }

//     if (x1 < x0) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     int slope2 = yDiff * 2;
//     int y = y0;
//     int yProgress = 0;
//     int xDiff2 = xDiff*2;
//     for (int x = x0; x <= x1; x++)
//     {
//         if (swapXY) {
//             setColor(rt, y, x, color);
//         } else {
//             setColor(rt, x, y, color);
//         }

//         yProgress += slope2;
//         if (yProgress > xDiff) {
//             y += ((y0 < y1) ? 1 : -1);
//             yProgress -= xDiff2;
//         }
//     }
// }

// void drawLineBresenham3(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {
//     int xDiff = std::abs(x0 - x1);
//     int yDiff = std::abs(y0 - y1);

//     const bool swapXY = xDiff < yDiff;
//     if (swapXY) {
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//         std::swap(xDiff, yDiff);
//     }

//     if (x1 < x0) {
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }

//     int slope2 = yDiff * 2;
//     int y = y0;
//     int yProgress = 0;
//     int xDiff2 = xDiff*2;
//     int yInc = ((y0 < y1) ? 1 : -1);
//     if (swapXY) {
//         for (int x = x0; x <= x1; x++) {
//             setColor(rt, y, x, color);
//             yProgress += slope2;
//             if (yProgress > xDiff) {
//                 y += yInc;
//                 yProgress -= xDiff2;
//             }
//         }
//     } else {
//         for (int x = x0; x <= x1; x++) {
//             setColor(rt, x, y, color);
//             yProgress += slope2;
//             if (yProgress > xDiff) {
//                 y += yInc;
//                 yProgress -= xDiff2;
//             }
//         }
//     }
// }

// void crossProduct(float x0, float y0, float z0, float x1, float y1, float z1, float* cross) {
//     cross[0] = y0 * z1 - z0 * y1;
//     cross[1] = z0 * x1 - x0 * z1;
//     cross[2] = x0 * y1 - y0 * x1;
// }

// void calculateBarycentric(int x, int y, int a[2], int b[2], int c[2], float* barycentric) {
//     float cross[3];
//     crossProduct((float)c[0]-a[0], (float)b[0]-a[0], (float)a[0]-x, // vector 1
//                  (float)c[1]-a[1], (float)b[1]-a[1], (float)a[1]-y, // vector 2
//                  cross);
//     /* `pts` and `P` has integer value as coordinates
//        so `abs(u[2])` < 1 means `u[2]` is 0, that means
//        triangle is degenerate, in this case return something with negative coordinates */
//     if (std::abs(cross[2])<1) {
//         barycentric[0] = -1.f;
//         barycentric[1] = 1.f;
//         barycentric[2] = 1.f;
//     } else {
//         barycentric[0] = 1.f - (cross[0] + cross[1]) / cross[2]; 
//         barycentric[1] = cross[1] / cross[2];
//         barycentric[2] = cross[0] / cross[2];
//     }
// }

// Vec3f calculateBarycentric(int x, int y, Vec3f a, Vec3f b, Vec3f c) {
//     float cross[3];
//     crossProduct((float)c[0]-a[0], (float)b[0]-a[0], (float)a[0]-x, // vector 1
//                  (float)c[1]-a[1], (float)b[1]-a[1], (float)a[1]-y, // vector 2
//                  cross);
//     /* `pts` and `P` has integer value as coordinates
//        so `abs(u[2])` < 1 means `u[2]` is 0, that means
//        triangle is degenerate, in this case return something with negative coordinates */
//     if (std::abs(cross[2])<1) {
//         return Vec3f(-1.f, 1.f, 1.f);
//     } else {
//         return Vec3f(
//             1.f - (cross[0] + cross[1]) / cross[2],
//             cross[1] / cross[2],
//             cross[0] / cross[2]
//         );
//     }
// }


// void drawTriangleLineSweep_tiny(RenderTarget& rt, int a[2], int b[2], int c[2], Color color) {
//     // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
//     if (a[1]>b[1]) std::swap(a, b); 
//     if (a[1]>c[1]) std::swap(a, c); 
//     if (b[1]>c[1]) std::swap(b, c); 
//     int total_height = c[1]-a[1]; 
//     for (int y=a[1]; y<=b[1]; y++) { 
//         int segment_height = b[1]-a[1]+1; 
//         float alpha = (float)(y-a[1])/total_height; 
//         float beta  = (float)(y-a[1])/segment_height; // be careful with divisions by zero 

//         int A[2] = {(int)(a[0] + (c[0] - a[0]) * alpha), (int)(a[1] + (c[1] - a[1]) * alpha)};
//         int B[2] = {(int)(a[0] + (b[0] - a[0]) * beta), (int)(a[1] + (b[1] - a[1]) * beta)};
//         // Vec2i A = a + (c-a)*alpha; 
//         // Vec2i B = a + (b-a)*beta; 

//         if (A[0]>B[0]) std::swap(A, B); 
//         for (int j=A[0]; j<=B[0]; j++) { 
//             // image.set(j, y, color); // attention, due to int casts t0[1]+i != A.y 
//             setColor(rt, j, y, color);
//         } 
//     } 
//     for (int y=b[1]; y<=c[1]; y++) { 
//         int segment_height =  c[1]-b[1]+1; 
//         float alpha = (float)(y-a[1])/total_height; 
//         float beta  = (float)(y-b[1])/segment_height; // be careful with divisions by zero 
//         // Vec2i A = a + (c-a)*alpha; 
//         // Vec2i B = b + (c-b)*beta; 
//         int A[2] = {(int)(a[0] + (c[0] - a[0]) * alpha), (int)(a[1] + (c[1] - a[1]) * alpha)};
//         int B[2] = {(int)(b[0] + (c[0] - b[0]) * beta), (int)(b[1] + (c[1] - b[1]) * beta)};
//         if (A[0]>B[0]) std::swap(A, B); 
//         for (int j=A[0]; j<=B[0]; j++) { 
//             // image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
//             setColor(rt, j, y, color);
//         } 
//     } 
// }

// void drawTriangleLineSweep(RenderTarget& rt, int a[2], int b[2], int c[2], Color color) {
//     // Sort a-to-b
//     if (a[1] > b[1]) std::swap(a, b); 
//     if (a[1] > c[1]) std::swap(a, c); 
//     if (b[1] > c[1]) std::swap(b, c); 

//     int acLength = c[1] - a[1];
//     if (acLength <= 0) return;

//     int abLength = b[1] - a[1];
//     if (abLength > 0) {
//         for (int y = std::max(a[1], 0); y <= std::min(b[1], rt.height); y++) {
//             float abProgress = (float)(y - a[1]) / abLength;
//             float acProgress = (float)(y - a[1]) / acLength;

//             int abX = a[0] + abProgress * (b[0] - a[0]);
//             int acX = a[0] + acProgress * (c[0] - a[0]);
//             if (abX < acX) {
//                 std::swap(abX, acX);
//             }
//             for (int x = std::max(acX, 0); x <= std::min(abX, rt.width); x++) {
//                 setColor(rt, x, y, color);
//             }
//         }
//     }
   
//     int bcLength = c[1] - b[1];
//     if (bcLength > 0) {
//         for (int y = std::max(b[1], 0); y <= std::min(c[1], rt.height); y++) {
//             float bcProgress = (float)(y - b[1]) / bcLength;
//             float acProgress = (float)(y - a[1]) / acLength;

//             int bcX = b[0] + bcProgress * (c[0] - b[0]);
//             int acX = a[0] + acProgress * (c[0] - a[0]);
//             if (acX < bcX) {
//                 std::swap(acX, bcX);
//             }
//             for (int x = std::max(bcX, 0); x <= std::min(acX, rt.width); x++) {
//                 setColor(rt, x, y, color);
//             }
//         }
//     }
// }

// void drawTriangleBarycentric(RenderTarget& rt, int a[2], int b[2], int c[2], Color color) {
//     float minX = std::min(a[0], std::min(b[0], c[0]));
//     float minY = std::min(a[1], std::min(b[1], c[1]));

//     float maxX = std::max(a[0], std::max(b[0], c[0]));
//     float maxY = std::max(a[1], std::max(b[1], c[1]));

//     int yBound = std::clamp((int)maxY, 0, rt.height);
//     for (int y = std::clamp((int)minY, 0, rt.height); y <= yBound; y++) {
//         // Ignore the point if it's out of viewport's bounds
//         if (y < 0 || y >= rt.height)
//             continue;

//         bool started_drawing_triangle = false;
//         int xBound = std::clamp((int)maxX, 0, rt.width);
//         for (int x = std::clamp((int)minX, 0, rt.width); x <= xBound; x++) {
//             // Ignore the point if it's out of viewport's bounds
//             // if (x < 0 || x >= rt.width)
//             //     continue;

//             float barycentric[3];
//             calculateBarycentric(x, y, a, b, c, barycentric);
//             const bool insideTriangle = barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0;
//             if (!insideTriangle) {
//                 // If we were drawing a triangle on this line but are no more, then we can
//                 // just jump to another line, as there definitely won't be any more of this
//                 // triangle on this line
//                 if (started_drawing_triangle)
//                     break;

//                 continue;
//             }
//             started_drawing_triangle = true;

//             // Point is already in ndc, so we have already "projected into 3d",
//             // hence we don't need division by w.
//             // float z = barycentric.X() * v0.Z() + barycentric.Y() * v1.Z() + barycentric.Z() * v2.Z();

//             // int depth_buf_index = BufIndex(x, y);
//             // if (z < depth_buf[depth_buf_index])
//                 // continue;

//             // bool should_render;
//             // Color color;
//             // std::tie(should_render, color) = shader.Fragment(barycentric, face);

//             // if (should_render) {
//                 // depth_buf[depth_buf_index] = z;
//                 // framebuf.Set(x, y, color);
//             // }

//             setColor(rt, x, y, color);
//         }
//     }
// }

// void drawColouredModel(RenderTarget& rt, MModel& model) {
//     for (int i=0; i<model.nfaces(); i++) { 
//         std::vector<int> face = model.face(i);
//         int coords[3][2];
//         for (int j = 0; j < 3; j++) {
//             Vec3f world_coords = model.vert(face[j]);
//             coords[j][0] = (int)((world_coords.x + 1.) * rt.width / 2.f);
//             coords[j][1] = (int)((world_coords.y + 1.) * rt.height / 2.f);
//         }

//         // drawTriangleBarycentric(rt, coords[0], coords[1], coords[2], Color{rand() % 255, rand() % 255, rand() % 255, 255});
//         drawTriangleLineSweep(rt, coords[0], coords[1], coords[2], Color{rand() % 255, rand() % 255, rand() % 255, 255});
//         // drawTriangleLineSweep_tiny(rt, coords[0], coords[1], coords[2], Color{rand() % 255, rand() % 255, rand() % 255, 255});
//         // drawTriangleBarycentric(rt, coords[0], coords[1], coords[2], RED);
//     }
// }

// void drawFlatShadedModel(RenderTarget& rt, Vec3f lightDir, Color lightColor, MModel& model) {
//     for (int i=0; i<model.nfaces(); i++) { 
//         std::vector<int> face = model.face(i);
//         int coords[3][2];
//         Vec3f worldCoords[3];
//         for (int j = 0; j < 3; j++) {
//             worldCoords[j] = model.vert(face[j]);
//             coords[j][0] = (int)((worldCoords[j].x + 1.) * rt.width / 2.f);
//             coords[j][1] = (int)((worldCoords[j].y + 1.) * rt.height / 2.f);
//         }

//         lightDir = Vec3f(0, 0, -1);
//         Vec3f normal = cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]);
//         normal = normalize(normal);
//         float lightIntensity = dot(lightDir, normal);
//         Color color = Color{lightColor.r * lightIntensity, lightColor.g * lightIntensity, lightColor.b * lightIntensity, 255};

//         // Extremely scuffed backface culling
//         if (lightIntensity > 0) {
//             drawTriangleLineSweep(rt, coords[0], coords[1], coords[2], color);
//             // drawTriangleBarycentric(rt, coords[0], coords[1], coords[2], color);
//         }
//     }
// }

// void drawTriangleShaded(RenderTarget& rt, Vec3f a, Vec3f b, Vec3f c, Color color) {
//     float minX = std::min(a[0], std::min(b[0], c[0]));
//     float minY = std::min(a[1], std::min(b[1], c[1]));

//     float maxX = std::max(a[0], std::max(b[0], c[0]));
//     float maxY = std::max(a[1], std::max(b[1], c[1]));

//     int yBound = std::clamp((int)maxY, 0, rt.height);
//     for (int y = std::clamp((int)minY, 0, rt.height); y <= yBound; y++) {
//         // Ignore the point if it's out of viewport's bounds
//         if (y <= 0 || y >= rt.height)
//             continue;

//         bool started_drawing_triangle = false;
//         int xBound = std::clamp((int)maxX, 0, rt.width);
//         for (int x = std::clamp((int)minX, 0, rt.width); x <= xBound; x++) {
//             Vec3f barycentric = calculateBarycentric(x, y, a, b, c);
//             const bool insideTriangle = barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0;
//             if (!insideTriangle) {
//                 if (started_drawing_triangle)
//                     break;

//                 continue;
//             }
//             started_drawing_triangle = true;

//             // Point is already in ndc, so we have already "projected into 3d",
//             // hence we don't need division by w.
//             float z = barycentric[0] * a.z + barycentric[1] * b.z + barycentric[2] * c.z;

//             // float visZ = z * 0.5 + 0.5;
//             // color = Color{255 * visZ, 255 * visZ, 255 * visZ, 255};
//             setColor(rt, x, y, color, z);
//         }
//     }
// }

Vec3f worldToScreen(RenderTarget& rt, Vec3f world) {
    return Vec3f(
        (int)((world.x + 1.) * rt.width / 2.f),
        (int)((world.y + 1.) * rt.height / 2.f),
        world.z
    );
}

// void drawShaded(RenderTarget& rt, Vec3f lightDir, Color lightColor, MModel& model) {
//     for (int i=0; i<model.nfaces(); i++) { 
//         std::vector<int> face = model.face(i);
//         // int coords[3][2];
//         Vec3f worldCoords[3];
//         for (int j = 0; j < 3; j++) {
//             worldCoords[j] = model.vert(face[j]);
//         }

//         Vec3f normal = cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]);
//         normal = normalize(normal);
//         Vec3f view = Vec3f(0, 0, -1);

//         bool backface = normal.z > 0;
//         if (backface) continue;
        
//         float lightIntensity = dot(lightDir, normal);
//         lightIntensity = std::clamp(lightIntensity, 0.f, 1.f);
//         Color color = Color{lightColor.r * lightIntensity, lightColor.g * lightIntensity, lightColor.b * lightIntensity, 255};
//         drawTriangleShaded(rt, worldToScreen(rt, worldCoords[0]), worldToScreen(rt, worldCoords[1]), worldToScreen(rt, worldCoords[2]), color);
//     }
// }
















void drawTestLines(Renderer& renderer) {
    renderer.drawLine(renderer.rt, Vec2i(50, 20), Vec2i(100, 400), RED);
    renderer.drawLine(renderer.rt, Vec2i(20, 80), Vec2i(400, 100), GREEN);
    renderer.drawLine(renderer.rt, Vec2i(200, 100), Vec2i(10, 200), BLUE);
}

void drawTestTriangles(Renderer& renderer) {
    renderer.drawTriangle(renderer.rt, Vec3f(10, 70), Vec3f(50, 160), Vec3f(70, 80), RED);
    renderer.drawTriangle(renderer.rt, Vec3f(180, 50), Vec3f(150, 1), Vec3f(70, 180), GREEN);
    renderer.drawTriangle(renderer.rt, Vec3f(180, 150), Vec3f(120, 160), Vec3f(130, 180), BLUE);
}

void drawWireframeModel(Renderer& renderer) {
    for (int i = 0; i < renderer.model.nfaces(); i++) {
        std::vector<int> face = renderer.model.face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = renderer.model.vert(face[j]);
            Vec3f v1 = renderer.model.vert(face[(j + 1) % 3]);
            renderer.drawLine(renderer.rt, worldToScreen(renderer.rt, v0), worldToScreen(renderer.rt, v1), WHITE);
        }
    }
}

void drawRandomColorModel(Renderer& renderer) {
    // Re-seed every time so that the colors never change
    srand(0);

    for (int i = 0; i < renderer.model.nfaces(); i++) { 
        std::vector<int> face = renderer.model.face(i);
        Vec3f triangles[3];
        for (int j = 0; j < 3; j++) {
            triangles[j] = worldToScreen(renderer.rt, renderer.model.vert(face[j]));
        }

        Color randomColor = {
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(255)
        };
        renderer.drawTriangle(renderer.rt, triangles[0], triangles[1], triangles[2], randomColor);
    }
}

void drawModel(Renderer& renderer) {
    for (int i = 0; i < renderer.model.nfaces(); i++) { 
        std::vector<int> face = renderer.model.face(i);
        Vec3f worldSpaceTriangles[3];
        for (int j = 0; j < 3; j++) {
            worldSpaceTriangles[j] = renderer.model.vert(face[j]);
            // triangles[j] = worldToScreen(renderer.rt, renderer.model.vert(face[j]));
        }

        Vec3f normal = cross(worldSpaceTriangles[2] - worldSpaceTriangles[0], worldSpaceTriangles[1] - worldSpaceTriangles[0]);
        normal = normalize(normal);

        bool backface = normal.z > 0;
        if (backface) continue;
        
        float lightIntensity = dot(renderer.lightDir, normal);
        lightIntensity = std::clamp(lightIntensity, 0.f, 1.f);
        Color color = {
            static_cast<unsigned char>(renderer.lightColor.r * lightIntensity),
            static_cast<unsigned char>(renderer.lightColor.g * lightIntensity),
            static_cast<unsigned char>(renderer.lightColor.b * lightIntensity),
            static_cast<unsigned char>(255)
        };
        renderer.drawTriangle(renderer.rt,
            worldToScreen(renderer.rt, worldSpaceTriangles[0]),
            worldToScreen(renderer.rt, worldSpaceTriangles[1]),
            worldToScreen(renderer.rt, worldSpaceTriangles[2]),
            color);
    }
}


// void drawModel(Renderer& renderer) {}

// void drawLine(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {}
// void drawLineBresenham(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {}

// Left as an exercise to the interested reader: Wu's line drawing algorithm.
// Builds upon Bresenham's algorithm and adds anti-aliasing.
// void drawLineWu(RenderTarget& rt, int x0, int y0, int x1, int y1, Color color) {}

// void drawTriangle(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color) {}
// void drawTriangleLineSweep(RenderTarget& rt, Vec3f a, Vec3f b, Vec3f c, Color color) {}
// void drawTriangleBarycentric(RenderTarget& rt, Vec3f a, Vec3f b, Vec3f c, Color color) {}

// void drawTriangleShaded(RenderTarget& rt, Vec3f a, Vec3f b, Vec3f c)

// void flatShading() {}
// void goroudShading() {}
// void phongShading() {}
// void toonShading() {}

// void drawModel() {}

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

// Left as an exercise to the interested reader: Wu's line drawing algorithm.
// Builds upon Bresenham's algorithm and adds anti-aliasing.
void drawLineWu(RenderTarget& rt, Vec2i a, Vec2i b, Color color) {}

void drawTriangle(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color) {}

void drawTriangleLineSweep(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color) {}

void drawTriangleBarycentric(RenderTarget& rt, Vec2i a, Vec2i b, Vec2i c, Color color) {
    float minX = std::min(a.x, std::min(b.x, c.x));
    float minY = std::min(a.y, std::min(b.y, c.y));
    float maxX = std::max(a.x, std::max(b.x, c.x));
    float maxY = std::max(a.y, std::max(b.y, c.y));

    int yBound = std::clamp((int)maxY, 0, rt.height);
    for (int y = std::clamp((int)minY, 0, rt.height); y <= yBound; y++) {
        bool startedDrawingTriangle = false;
        int xBound = std::clamp((int)maxX, 0, rt.width);
        for (int x = std::clamp((int)minX, 0, rt.width); x <= xBound; x++) {
            Vec3f bary = barycentric(x, y, a, b, c);
            bool insideTriangle = bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0;
            if (insideTriangle) {
                startedDrawingTriangle = true;
                setColor(rt, x, y, color);
            } else if(startedDrawingTriangle) {
                break;
            }
        }
    }
}

void drawTriangleShaded(RenderTarget& rt, Vec3i a, Vec3i b, Vec3i c, Color color) {
}

void render(Renderer& renderer) {
    RenderTarget& rt = renderer.rt;

    // renderer.drawLine = drawLine;
    renderer.drawLine = drawLineBresenham;
    // renderer.drawLine = drawLineOptimisedBresenham;
    renderer.drawTriangle = drawTriangleBarycentric;

    // renderer.drawTriangle = drawTriangle;
    // renderer.drawTriangle = drawTriangleBarycentric0;

    clear(rt, BLACK);

    // drawLineOptimisedBresenham(renderer.rt, Vec2i(50, 20), Vec2i(100, 400), RED);
    // drawLineOptimisedBresenham(renderer.rt, Vec2i(20, 80), Vec2i(400, 100), GREEN);
    // drawLineOptimisedBresenham(renderer.rt, Vec2i(10, 100), Vec2i(200, 100), BLUE);

    // drawLineBresenham3(rt, 0, 50, 1000, 100, BLUE);
    // drawLineBresenham3(rt, 20, 100, 100, 1000, RED);
    // drawLineBresenham3(rt, 50, 20, 100, 400, RED);
    // drawLineBresenham3(rt, 20, 80, 400, 200, GREEN);
    // drawLineBresenham3(rt, 10, 100, 200, 100, BLUE);

    // scene(renderer);
    drawTestLines(renderer);
    // drawWireframeModel(renderer);
    drawTestTriangles(renderer);
    // drawRandomColorModel(renderer);
    drawModel(renderer);

    // int t0[3][2] = {{10, 70}, {50, 160}, {70, 80}}; 
    // int t1[3][2] = {{180, 50},  {150, 1}, {70, 180}}; 
    // int t2[3][2] = {{180, 150}, {120, 160}, {130, 180}}; 
    // drawTriangleBarycentric(rt, t0[0], t0[1], t0[2], RED); 
    // drawTriangleBarycentric(rt, t1[0], t1[1], t1[2], WHITE); 
    // drawTriangleBarycentric(rt, t2[0], t2[1], t2[2], GREEN);

    // // drawWireframeModel(rt, renderer.model);
    // // drawColouredModel(rt, renderer.model);
    // // drawFlatShadedModel(rt, renderer.lightDir, renderer.lightColor, renderer.model);
    // drawShaded(rt, renderer.lightDir, renderer.lightColor, renderer.model);
}

