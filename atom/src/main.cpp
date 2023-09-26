#include <iostream>
#include <math.h>
#include "SDL.h"
#include "GL/glew.h"

#include "Line.h"
#include "utils.h"

int map[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 2, 2, 2, 2, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 1},
    {1, 0, 0, 2, 2, 2, 2, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct Vec pos = {2.f, 2.f};
struct Vec dir = {-1.f, 0.f};
struct Vec plane = {0.f, 0.66f};

int main(int argc, char *argv[])
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("atom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    //SDL_GL_SetSwapInterval(0); // VSYNC
    glewInit();

    int w = 800; // WITDH
    int h = 600; // HEIGHT

    Line line = Line();
    LineConfig lines[w];

    Uint32 lastUpdate = SDL_GetTicks();
    float deltaTime;

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        Uint64 startTime = SDL_GetPerformanceCounter();
        Uint32 currentTick = SDL_GetTicks();
        deltaTime = (currentTick - lastUpdate) / 1000.f;
        lastUpdate = currentTick;

        float moveSpeed = deltaTime * 2.5f;
        float rotSpeed = deltaTime * 2.25f;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) {
                    pos.x += dir.x * moveSpeed;
                    pos.y += dir.y * moveSpeed;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    pos.x -= dir.x * moveSpeed;
                    pos.y -= dir.y * moveSpeed;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    float old_dirx = dir.x;
                    dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
                    dir.y = old_dirx * sin(rotSpeed) + dir.y * cos(rotSpeed);
                    float old_planex = plane.x;
                    plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
                    plane.y = old_planex * sin(rotSpeed) + plane.y * cos(rotSpeed);
                }
                if (event.key.keysym.sym == SDLK_a) {
                    float old_dirx = dir.x;
                    dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
                    dir.y = old_dirx * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
                    float old_planex = plane.x;
                    plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
                    plane.y = old_planex * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
                }
            }
        }

        for (int x = 0; x < w; x++)
        {
            float camX = 2 * x / (float)w - 1;
            struct Vec rayDir = {dir.x + plane.x * camX, dir.y + plane.y * camX};

            struct Veci pos_map = {int(pos.x), int(pos.y)};
            struct Vec sideDist = {};

            struct Vec deltaDist = {
                (rayDir.x == 0) ? 1e30 : std::abs(1 / rayDir.x),
                (rayDir.y == 0) ? 1e30 : std::abs(1 / rayDir.y),
            };

            double perpWallDist;

            struct Veci step = {};

            int hit = 0;
            int side;
            if (rayDir.x < 0)
            {
                step.x = -1;
                sideDist.x = (pos.x - pos_map.x) * deltaDist.x;
            }
            else
            {
                step.x = 1;
                sideDist.x = (pos_map.x + 1.f - pos.x) * deltaDist.x;
            }

            if (rayDir.y < 0)
            {
                step.y = -1;
                sideDist.y = (pos.y - pos_map.y) * deltaDist.y;
            }
            else
            {
                step.y = 1;
                sideDist.y = (pos_map.y + 1.f - pos.y) * deltaDist.y;
            }

            while (hit == 0)
            {
                if (sideDist.x < sideDist.y)
                {
                    sideDist.x += deltaDist.x;
                    pos_map.x += step.x;
                    side = 0;
                }
                else
                {
                    sideDist.y += deltaDist.y;
                    pos_map.y += step.y;
                    side = 1;
                }
                if (map[pos_map.x][pos_map.y] > 0)
                    hit = 1;
            }

            if (side == 0)
                perpWallDist = (sideDist.x - deltaDist.x);
            else
                perpWallDist = (sideDist.y - deltaDist.y);

            int lineHeight = (int)(h / perpWallDist);

            lines[x] = LineConfig{
                Vec{1.f - x / (w / 2.f),
                    (lineHeight / 2.f) / (w / 2.f)},
                Vec{1.f - x / (w / 2.f),
                    -1.f * (lineHeight / 2.f) / (w / 2.f)},
                (map[pos_map.x][pos_map.y] == 1) ? Color {1.f, 0.f, 0.f} : Color{0.f, 1.f, 0.f}
                };
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        line.SetLines(lines, w);
        line.Use();

        SDL_GL_SwapWindow(window);


        Uint64 endTime = SDL_GetPerformanceCounter();
        
        float elapsed = (endTime - startTime) / (float)SDL_GetPerformanceFrequency();
        std::cout << "FPS: " << 1.f / elapsed << std::endl;
    }

    std::cout << "quiting" << std::endl;
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}