#pragma once
#include <SDL.h>
#include "Render.hpp"
#include "Entity.hpp"

class Raycasting
{
public:
	Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY);
	~Raycasting();

	void SetEntity(Entity* entity);

	void DrawPixels(Render& render);
private:
	int screenWidth, screenHeight;
	int mapX, mapY;
	int** map;

	Entity* entity;
};

