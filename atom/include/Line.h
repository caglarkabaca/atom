#pragma once

#include <iostream>
#include "GL/glew.h"
#include "utils.h"

struct LineConfig {
    Vec begin;
    Vec end;
    Color color;
}; typedef struct LineConfig LineConfig;

class Line
{
private:
    GLuint shaderProgram;
    GLuint VBO, VAO;

    int last_count;

public:
    Line()
    {
        const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec3 aColor;

            out vec3 lineColor;

            void main() {
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                lineColor = aColor;
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            in vec3 lineColor;
            void main() {
                FragColor = vec4(lineColor, 1.0);
            }
        )";

        GLuint vertexShader, fragmentShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Vertex verilerini GPU'ya kopyala
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

    }

    void SetLines(LineConfig* lines, int count) {
        
        float vertices[count * 10];
        last_count = count;

        int index = 0;
        for (int i = 0; i < count * 10 - 9; i+=10) {
            vertices[i]     = lines[index].begin.x;// x1
            vertices[i + 1] = lines[index].begin.y;// y1
            vertices[i + 2] = lines[index].color.r;// r
            vertices[i + 3] = lines[index].color.g;// g
            vertices[i + 4] = lines[index].color.b;// b

            vertices[i + 5] = lines[index].end.x;// x2
            vertices[i + 6] = lines[index].end.y;// y2
            vertices[i + 7] = lines[index].color.r;// r
            vertices[i + 8] = lines[index].color.g;// g
            vertices[i + 9] = lines[index].color.b;// b
            index += 1;
        }
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Vertex pozisyonları için vertex attrib pointer'ını yapılandır
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }

    void Use()
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, last_count * 2);
    }

    ~Line() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};