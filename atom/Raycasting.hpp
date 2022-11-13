#pragma once
#include <SDL.h>
#include "Render.hpp"
#include "TextureManager.hpp"
#include "Entity.hpp"

class Raycasting
{
public:
	Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY, int gridSize);
	~Raycasting();

	void SetEntity(Entity* entity);
	void DrawPixels(Render& render, double FOV);
	void DrawPixelsTextured(TextureManager& txtManager, SDL_Texture** textureArray, int textureWidth, double FOV);
	void ListenKeys(double frameTime);
private:
	int screenWidth, screenHeight;
	int mapX, mapY;
	int gridSize;
	int** map;

	Entity* entity;
};

