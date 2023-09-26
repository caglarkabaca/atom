#pragma once

#include <iostream>
#include "GL/glew.h"
#include "utils.h"

struct LineConfig {
    Vec begin;
    Vec end;
}; typedef struct LineConfig LineConfig;

class Line
{
private:
    GLuint shaderProgram;
    GLuint VBO, VAO;

public:
    Line(LineConfig config)
    {
        const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            void main() {
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            uniform vec3 lineColor;
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

        // Çizgi noktalarını tanımla
        float vertices[] = {
            config.begin.x, config.begin.y, // Başlangıç noktası
            config.end.x, config.end.y  // Bitiş noktası
        };

        // Vertex verilerini GPU'ya kopyala
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Vertex pozisyonları için vertex attrib pointer'ını yapılandır
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Shader'a çizgi rengini gönderme
        GLint lineColorLoc = glGetUniformLocation(shaderProgram, "lineColor");
        if (lineColorLoc != -1)
        {
            glUniform3f(lineColorLoc, 1.0f, 0.0f, 0.0f); // Çizgi rengi: Kırmızı
        }
        else
        {
            std::cerr << "Uniform 'lineColor' not found in the shader." << std::endl;
        }
    }

    void Use()
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
    }

    ~Line() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};