#include "Map.hpp"
// Haritayý yükleme ve çizme

Map::Map() {
}

Map::~Map() {

}

void Map::DrawMap(Render* renderer, int map[24][24]) {
	renderer->DrawRectOutline(1280 - 288, 720 - 288, 288, 288, { 0,0,0 });
	int type = 0;
	for (int y = 24; y >=0; y--) {
		for (int x = 0; x < 24; x++) {
			type = map[y][x];
			switch (type) {
			case 1:
				renderer->DrawRect(1280 - 288 + x * 12, 720 - 288 + y * 12, 12, 12, { 190, 190, 190, 255});
				break;
			case 2:
				renderer->DrawRect(1280 - 288 + x * 12, 720 - 288 + y * 12, 12, 12, { 0, 0, 255, 255});
				break;
			case 3:
				renderer->DrawRect(1280 - 288 + x * 12, 720 - 288 + y * 12, 12, 12, { 255, 255, 255 , 255});
				break;
			case 4:
				renderer->DrawRect(1280 - 288 + x * 12, 720 - 288 + y * 12, 12, 12, { 255, 255, 0 , 255});
				break;
			case 5:
				renderer->DrawRect(1280 - 288 + x * 12, 720 - 288 + y * 12, 12, 12, { 255, 0, 0 , 255});
				break;
			default:
				renderer->DrawRect(1280 - 288 + x * 12, 720 - 288 + y * 12, 12, 12, { 0, 0, 0 , 255});
				break;
			}
		}
	}
	
}

void Map::DrawPlayer(Render* renderer, double playerX, double playerY) {

	renderer->DrawRect(1280 - 288 + playerX * 12 / 128, 720 - 288 + playerY * 12 / 128, 12, 12, { 0, 255, 0 , 255});
}
