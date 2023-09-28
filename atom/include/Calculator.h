#pragma once

#include <iostream>
#include "SDL.h"
#include "GL/glew.h"

#include "utils.h"
#include "Line.h"

#include <vector>

const char *computeShaderSource = R"(
    #version 450 core

    layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

    // lineHEIGHT HESABI

    const int w = 800;
    const int h = 600;

    layout(binding = 0) buffer InputData {
        float inputVectors[];
    };

    layout(binding = 1) buffer InputData2 {
        int map[];
    };

    // Çıkış verisi
    layout(binding = 2) buffer OutputData {
        int result[800];
    };

    void main() {

        vec2 pos = vec2(inputVectors[0], inputVectors[1]);
        vec2 dir = vec2(inputVectors[2], inputVectors[3]);
        vec2 plane = vec2(inputVectors[4], inputVectors[5]);

        for (int index = 0; index < w; index++) {
            int x = index;

            float camX = 2 * x / float(w) - 1;
            vec2 rayDir = vec2(dir.x + plane.x * camX, dir.y + plane.y * camX);

            ivec2 pos_map = ivec2(int(pos.x), int(pos.y));
            vec2 sideDist = vec2(0.0);

            vec2 deltaDist = vec2(
                (rayDir.x == 0) ? 1e30 : abs(1 / rayDir.x),
                (rayDir.y == 0) ? 1e30 : abs(1 / rayDir.y)
            );

            float perpWallDist = 0.0;

            ivec2 step = ivec2(0);

            int hit = 0;
            int side = 0;
            if (rayDir.x < 0)
            {
                step.x = -1;
                sideDist.x = (pos.x - pos_map.x) * deltaDist.x;
            }
            else
            {
                step.x = 1;
                sideDist.x = (pos_map.x + 1.0 - pos.x) * deltaDist.x;
            }

            if (rayDir.y < 0)
            {
                step.y = -1;
                sideDist.y = (pos.y - pos_map.y) * deltaDist.y;
            }
            else
            {
                step.y = 1;
                sideDist.y = (pos_map.y + 1.0 - pos.y) * deltaDist.y;
            }

            int i = 0;
            while (hit == 0)
            {
                if (i > 100)
                    break;
                i = i + 1;

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
                if (map[pos_map.x + 10 * pos_map.y] > 0)
                    hit = 1;
            }

            if (side == 0)
                perpWallDist = (sideDist.x - deltaDist.x);
            else
                perpWallDist = (sideDist.y - deltaDist.y);

            int lineHeight = int(h / perpWallDist);

            result[index] = lineHeight;
        }
    }
)";

class Calculator
{
private:
    int w = 800;
    int h = 600;
    GLuint computeProgram;
    GLuint inputBuffer, inputBufferMap, outputBuffer;

    int *linearMap;

public:
    Calculator(int *map)
    {
        GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(computeShader, 1, &computeShaderSource, NULL);
        glCompileShader(computeShader);

        GLint status;
        glGetShaderiv(computeShader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            GLint infoLogLength;
            glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *buffer = new GLchar[infoLogLength];

            GLsizei bufferSize;
            glGetShaderInfoLog(computeShader, infoLogLength, &bufferSize, buffer);
            std::cout << buffer << std::endl;

            exit(EXIT_FAILURE);
        }

        computeProgram = glCreateProgram();
        glAttachShader(computeProgram, computeShader);
        glLinkProgram(computeProgram);
        glUseProgram(computeProgram);

        linearMap = map;

        glDeleteShader(computeShader);

        glGenBuffers(1, &inputBuffer);
        glGenBuffers(1, &outputBuffer);
        glGenBuffers(1, &inputBufferMap);


        glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBufferMap);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 100, linearMap, GL_DYNAMIC_COPY);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * w, NULL, GL_DYNAMIC_COPY);

        GLuint bindingIndex = 0;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, inputBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex + 1, inputBufferMap);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex + 2, outputBuffer);
    }

    ~Calculator()
    {
        glDeleteBuffers(1, &inputBuffer);
        glDeleteBuffers(1, &outputBuffer);
        glDeleteBuffers(1, &inputBufferMap);
        glDeleteProgram(computeProgram);
    }

    LineConfig *calculateLines(Vec pos, Vec dir, Vec plane)
    {
        float data[] = {
            pos.x, pos.y,
            dir.x, dir.y,
            plane.x, plane.y};

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 6, data, GL_DYNAMIC_COPY);

        glUseProgram(computeProgram);

        glDispatchCompute(1, 1, 1);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        int *resultData;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
        resultData = static_cast<int *>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));

        if (!resultData)
        {
            std::cout << "no resultData" << std::endl;
            exit(EXIT_FAILURE);
        }

        int lineHeights[w];

        memcpy(lineHeights, resultData, sizeof(int) * w);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        LineConfig *lines = (LineConfig *)malloc(sizeof(LineConfig) * w);

        for (int x = 0; x < w; x++)
        {
            int lineHeight = lineHeights[x];
            lines[x] = LineConfig
            {
                Vec{1.f - x / (w / 2.f),
                    (lineHeight / 2.f) / (w / 2.f)},
                Vec{1.f - x / (w / 2.f),
                    -1.f * (lineHeight / 2.f) / (w / 2.f)},
                    Color { 1.f, 0.f, 0.f }
            };
        }
        return lines;
    }
};