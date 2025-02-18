#include "model.h"
#include "renderer.h"

#include "raygui.h"

#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <cmath>

struct Framebuffer {
    Image cpuTexture;
    Texture2D gpuTexture;	
    float* zBuffer;
};

Framebuffer allocateFramebuffer(int width, int height) {
    Image renderImage = {};
    renderImage.data = malloc(width * height * sizeof(Color));
    renderImage.width = width;
    renderImage.height = height;
    renderImage.mipmaps = 1;
    renderImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    Texture2D renderTexture = LoadTextureFromImage(renderImage);

    float* zBuffer = (float*)malloc(width * height * sizeof(float));
	
    return Framebuffer { renderImage, renderTexture, zBuffer };
}

void deallocateFramebuffer(Framebuffer fb) {
    UnloadImage(fb.cpuTexture);
    UnloadTexture(fb.gpuTexture);
    free(fb.zBuffer);
}

void drawDropDown(Rectangle rect, const char* label, const char* options, int* selected, bool* expanded, bool canOpen) {
    if (canOpen && GuiDropdownBox(rect, options, selected, *expanded)) {
        *expanded = !*expanded;
    }
    rect.x += 160;
    GuiLabel(rect, label);
}

int main ()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 800, "NMA ZS 2025");

    SetTargetFPS(10000);

    Framebuffer fb = allocateFramebuffer(GetRenderWidth(), GetRenderHeight());

    Renderer renderer;
    renderer.rt = RenderTarget{(Color*)fb.cpuTexture.data, fb.cpuTexture.width, fb.cpuTexture.height, fb.zBuffer};
    renderer.model = TexturedModel("assets/test_model.obj", "assets/diffuse.png");

    renderer.lightColor = WHITE;
    renderer.lightDir = Vec3f(0, -1, -1); 
    normalize(renderer.lightDir);

    while (!WindowShouldClose()) {
        int width = GetRenderWidth();
        int height = GetRenderHeight();

        if (width != fb.cpuTexture.width || height != fb.cpuTexture.height) {
            Framebuffer newFb = allocateFramebuffer(width, height);

            Color* oldPixels = (Color*)fb.cpuTexture.data;
            Color* newPixels = (Color*)newFb.cpuTexture.data;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    Color color = BLACK;
                    if (x < fb.cpuTexture.width && y < fb.cpuTexture.height) {
                        color = oldPixels[y * fb.cpuTexture.width + x];
                    }
                    newPixels[y * width + x] = color;
                }
            }

            deallocateFramebuffer(fb);
            fb = newFb;

            renderer.rt = {newPixels, width, height, fb.zBuffer};
        }

        static bool moveLight = false;
        if (moveLight) {
            float time = GetTime();
            renderer.lightDir = Vec3f(sin(time * 1.0), 0, cos(time * 1.0));
            renderer.lightDir = normalize(renderer.lightDir);
        } else {
            renderer.lightDir = Vec3f(0, 0, -1);
        }

        static int lineSelected = 0;
        static std::function<void(RenderTarget&, Vec2f, Vec2f, Color)> lineAlgs[] = {
            drawLine,
            drawLineBresenham,
            drawLineOptimisedBresenham,
            drawLineWu,
        };
        renderer.drawLine = lineAlgs[lineSelected];

        static int triangleSelected = 0;
        static std::function<void(Renderer&, Vec3f, Vec3f, Vec3f, Face&)> triangleAlgs[] = {
            drawTriangle,
            drawTriangleLineSweep,
            drawTriangleBarycentric,
        };
        renderer.drawTriangle = triangleAlgs[triangleSelected];

        static int sceneSelected = 0;
        static std::function<void(Renderer&)> scenes[] = {
            drawTest,
            drawWireframeModel,
            drawRandomColorModel,
            drawModel,
        };
        renderer.scene = scenes[sceneSelected];

        static int shadingSelected = 0;
        static std::function<Color(Renderer&, Face&, Vec3f)> shadingAlgs[] = {
            noShading,
            flatShading,
            goroudShading,
            phongShading,
        };
        // To prevent sampling textures on fake triangles
        if (sceneSelected == 0) {
            shadingSelected = 0;
        }
        renderer.shade = shadingAlgs[shadingSelected];
	    
        render(renderer);
    		UpdateTexture(fb.gpuTexture, fb.cpuTexture.data);
		
    		BeginDrawing();
    		{
    		    ClearBackground(BLACK);
    		    DrawTexture(fb.gpuTexture, 0, 0, WHITE);

    		    // GUI
    		    {
                GuiCheckBox(Rectangle{0, 25, 25, 25}, "Move light", &moveLight);

                static bool lineDropdownExpanded = false;
                drawDropDown(Rectangle{0, 50, 150, 25}, "Line algorithm",
                    "My custom;Bresenham;Optimised Bresenham;Wu", &lineSelected, &lineDropdownExpanded, true);

                static bool triangleDropdownExpanded = false;
                drawDropDown(Rectangle{0, 75, 150, 25}, "Triangle algorithm",
                    "My custom;Line sweep;Barycentric", &triangleSelected, &triangleDropdownExpanded,
                    !lineDropdownExpanded);

                static bool shadingDropdownExpanded = false;
                drawDropDown(Rectangle{0, 100, 150, 25}, "Shading algorithm",
                    "None;Flat;Goroud;Phong", &shadingSelected, &shadingDropdownExpanded,
                    !lineDropdownExpanded && !triangleDropdownExpanded);

                static bool sceneDropdownExpanded = false;
                drawDropDown(Rectangle{0, 125, 150, 25}, "Scene",
                    "Test;Wireframe;Random colors;Shaded", &sceneSelected, &sceneDropdownExpanded,
                    !lineDropdownExpanded && !triangleDropdownExpanded && !shadingDropdownExpanded);
    		    }
		    
            Color color = LIME;
            int fps = GetFPS();
            if ((fps < 30) && (fps >= 15)) color = ORANGE;
            else if (fps < 15) color = RED;

            DrawText(TextFormat("%.3f ms", GetFrameTime() * 1000.f), 0, 0, 20, color);
            DrawText(TextFormat("%2i FPS", fps), 150, 0, 20, color);

            }

        		EndDrawing();
        }

    deallocateFramebuffer(fb);
    CloseWindow();

    return 0;
}
