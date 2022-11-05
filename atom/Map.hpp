#pragma once

class Map {

public:
	Map();
	~Map();

	void LoadMap(int array[20][20]);
	void DrawMap();

private:
	int map[20][20];

};