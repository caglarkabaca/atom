#pragma once
#include <SDL.h>
#include "Render.hpp"
#include "TextureManager.hpp"
#include "Entity.hpp"

class Raycasting
{
public:
	Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY, int gridSize, double FOV);
	~Raycasting();

	void SetEntity(Entity* entity);
	void DrawPixels(Render& render);
	void DrawPixelsTextured(TextureManager& txtManager, SDL_Texture** textureArray, int textureWidth);
	void ListenKeys(double frameTime);
private:
	int screenWidth, screenHeight;
	int mapX, mapY;
	int gridSize;
	int** map;
	double FOV;

	Entity* entity;
};

