#pragma once
#include <SDL.h>
#include "Render.hpp"
#include "TextureManager.hpp"
#include "Entity.hpp"

class Raycasting
{
public:
	Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY);
	~Raycasting();

	void SetEntity(Entity* entity);
	void DrawPixels(Render& render);
	void DrawPixelsTextured(TextureManager& txtManager, SDL_Texture** textureArray);
	void ListenKeys(double frameTime);
private:
	int screenWidth, screenHeight;
	int mapX, mapY;
	int** map;

	Entity* entity;
};

