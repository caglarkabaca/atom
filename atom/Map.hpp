#pragma once
#include "Render.hpp"
#include "TextureManager.hpp"

class Map {

public:
	Map();
	~Map();

	void DrawMap(Render* renderer, int map[24][24]);
	void DrawPlayer(Render* renderer, double playerX, double playerY);
private:

};