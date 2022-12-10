#include "Raycasting.hpp"
#include <iostream>

// TEST THREADING
#include <SDL/SDL_thread.h>
#include <string>

#define MOVESPEED_MULTIPLER 250.0
#define ROTSPEED_MULTIPLER 10
#define UPDOWN_OFFSET M_PI / 8
#define MOVE_OFFSET 5 // duvarýn içine girmeyi engellemek için
#define DEPTH_OFFSET 0.4 // 0 - 1 arasý deðerler, 1 komple aydýnlýk yapar

#define DR 0.0174533 // 1 degree to radian

// posdan belirli açýya ýþýn yollar ve ýþýný izler
// ýþýnýn duvara çarptýðý yerde kordinatýný verir
// https://permadi.com/1996/05/ray-casting-tutorial-8/
Vector2D castTheRay(double ra, Vector2D pos, int mapSize, int gridSize, int** map, int& side) {
	double distH{ INT_MAX }, distV{ INT_MAX }; // placeholder 
	double hx{ pos.x }, hy{ pos.y };
	double vx{ pos.x }, vy{ pos.y };
	double rx{}, ry{}, xo{}, yo{};
	int dof = 0;

	// horizontal 
	dof = 0;
	if (ra == 0 || ra == M_PI) { rx = pos.x; ry = pos.y; dof = mapSize; }
	else {
		double aTan = -1 / tan(ra);
		if (ra > M_PI) {
			ry = ((int)pos.y / gridSize) * gridSize - 0.00000001;
			rx = (pos.y - ry) * aTan + pos.x;
			yo = -gridSize;
			xo = -yo * aTan;
		}
		if (ra < M_PI) {
			ry = ((int)pos.y / gridSize) * gridSize + gridSize;
			rx = (pos.y - ry) * aTan + pos.x;
			yo = gridSize;
			xo = -yo * aTan;
		}
	}

	while (dof < mapSize && rx >= 0 && ry >= 0 && rx <= mapSize * gridSize && ry <= mapSize * gridSize) {
		int mx{ (int)rx / gridSize }, my{ (int)ry / gridSize };
		if (mx < mapSize && my < mapSize && map[my][mx] > 0) {
			dof = mapSize;

			vx = rx;
			vy = ry;
			distV = sqrt(((rx - pos.x) * (rx - pos.x) + (ry - pos.y) * (ry - pos.y)));
		}
		else {
			rx += xo;
			ry += yo;
			dof++;
		}
	}

	// vertical
	dof = 0;
	if (ra == 0 || ra == M_PI) { rx = pos.x; ry = pos.y; dof = mapSize; }
	else {
		double nTan = -tan(ra);
		if (ra > M_PI / 2 && ra < 3 * M_PI / 2) {
			rx = ((int)pos.x / gridSize) * gridSize - 0.00000001;
			ry = (pos.x - rx) * nTan + pos.y;
			xo = -gridSize;
			yo = -xo * nTan;
		}
		if (ra < M_PI / 2 || ra > 3 * M_PI / 2) {
			rx = ((int)pos.x / gridSize) * gridSize + gridSize;
			ry = (pos.x - rx) * nTan + pos.y;
			xo = gridSize;
			yo = -xo * nTan;
		}
	}

	while (dof < mapSize && rx >= 0 && ry >= 0 && rx <= mapSize * gridSize && ry <= mapSize * gridSize) {
		int mx{ (int)rx / gridSize }, my{ (int)ry / gridSize };
		if (mx < mapSize && my < mapSize && map[my][mx] > 0) {
			dof = mapSize;

			hx = rx;
			hy = ry;
			distH = sqrt(((rx - pos.x) * (rx - pos.x) + (ry - pos.y) * (ry - pos.y)));
		}
		else {
			rx += xo;
			ry += yo;
			dof++;
		}
	}

	// hangisi kýsaysa onu alýyoruz
	if (distV < distH) {
		side = 0;
		return { vx, vy };
	}
	else {
		side = 1;
		return { hx, hy };
	}

}

Raycasting::Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY, int gridSize, double FOV, int threadCount):
	screenWidth(screenWidth), screenHeight(screenHeight), 
	mapX(mapX), mapY(mapY), entity(NULL), gridSize(gridSize), 
	FOV(FOV), mapMaxGrid(mapX), threadCount(threadCount) {

	if (mapY > mapX)
		mapMaxGrid = mapY;

	// memory allocation
	this->map = new int* [mapX];
	for (int i = 0; i < mapX; i++) {
		this->map[i] = new int[mapY];
	}

	// map copy
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			this->map[i][j] = *((map + i * mapY) + j);
		}
	}

}

Raycasting::~Raycasting() {
	for (int i = 0; i < mapX; i++)
		delete[] map[i];
	delete[] map;
}

void Raycasting::SetEntity(Entity* entity) { this->entity = entity; }

/*
	RENDER
	 Debug
	 Wall
	 FloorCeiling
*/

void Raycasting::DrawWalls(TextureManager& txtManager, SDL_Texture** textureArray, int textureWidth, int* b, int* e) {

	Vector2D pos{ entity->getPos() }, dir{ entity->getDir() };
	float angle = entity->getAngle();
	float shear = entity->getShear();

	const float sizeOfColumn = screenWidth / FOV;
	float offset = tan(shear) * screenHeight;

	int mapSize = mapMaxGrid;

	// cast the rays
	float ra{};
	ra = angle - DR * FOV / 2;
	if (ra < 0)
		ra += 2 * M_PI;
	if (ra > 2 * M_PI)
		ra -= 2 * M_PI;

	// wallcasting
	for (int i = 0; i < screenWidth; i++) {

		int side = 0;
		Vector2D ray = castTheRay(ra, pos, mapSize, gridSize, map, side);
		int mx{ (int)ray.x / gridSize }, my{ (int)ray.y / gridSize };
		float dist{(float) (sqrt(((ray.x - pos.x) * (ray.x - pos.x) + (ray.y - pos.y) * (ray.y - pos.y)))) };

		// bulunan kare (mx, my)
		// tam kordinat için (hx, hy) ve (vx, vy) yi karþýlaþtýrýp dist i küçük olaný alýcaz
		// mesafe dist

		// eðer minimapte ray i göstermek istiyorsak buraya ekleyebiliriz

		// fix fisheye
		float ca = angle - ra;
		if (ca < 0)
			ca -= 2 * M_PI;
		dist *= cos(ca);

		float lineH = (gridSize * screenHeight) / dist;
		//if (lineH > screenHeight) // yukarý aþaðý için kapattým
		//	lineH = screenHeight;
		float lineO = screenHeight / 2 - lineH / 2;

		// up down movement
		lineO += offset;

		float wallX{};
		if (side == 0) 
			wallX = ray.x - (mx * gridSize);
		else 
			wallX = ray.y - (my * gridSize);

		int tx = (int) (wallX * textureWidth / gridSize);

		// ters texturelar düzeliyor
		if (side == 0 && ra < M_PI)
			tx = textureWidth - tx - 1;

		if (side == 1 && ra > M_PI / 2 && ra < 3 * M_PI / 2)
			tx = textureWidth - tx - 1;

		// uzaklar daha karanlýk
		float darkness = lineH / screenHeight + DEPTH_OFFSET;
		if (darkness > 1)
			darkness = 1;
		SDL_SetTextureColorMod(textureArray[map[my][mx]], 255 * darkness, 255 * darkness, 255 * darkness);

		SDL_Rect src{ tx, 0, 1, textureWidth };
		SDL_Rect dst{ i, lineO, 1, lineH };
		txtManager.Draw(textureArray[map[my][mx]], src, dst);

		b[i] = lineO;
		e[i] = lineO + lineH;

		ra += DR * FOV / screenWidth;
		if (ra < 0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;
	}
}

// threadler için deniyorum
struct CalcStruct {
	int screenHeight;
	int screenWidth;
	float offset;
	int gridSize;
	int textureWidth;
	Vector2D pos;
	Uint32* pixels;
	Uint32* fpixels;
	Uint32* cpixels;
	float ra;
	float angle;
	int begin;
	int end;
	float FOV;
	int* b;
	int* e;
};

int fcCalc(void* data) {

	CalcStruct cData = *((CalcStruct*)data);

	int screenHeight = cData.screenHeight;
	int screenWidth = cData.screenWidth;
	float offset = cData.offset;
	int gridSize = cData.gridSize;
	int textureWidth = cData.textureWidth;
	Vector2DF pos{cData.pos.x, cData.pos.y};
	Uint32* pixels = cData.pixels;
	Uint32* fpixels = cData.fpixels;
	Uint32* cpixels = cData.cpixels;
	float FOV = cData.FOV;
	int* b = cData.b;
	int* e = cData.e;
	
	float ra = cData.ra;
	float angle = cData.angle;
	
	for (int i = cData.begin; i < cData.end; i++) {

		// pre calc for fps boost
		float rcos = cos(ra);
		float rsin = sin(ra);
		float dcos = cos(ra - angle);
		float pxg = pos.x / gridSize;
		float pyg = pos.y / gridSize;
		float halfsH = screenHeight / 2;

		//floor
		for (int j = screenHeight; j > halfsH + offset; j--) {

			if (e[i] > j)
				break;

			float n = (halfsH / (halfsH - j + offset)) / dcos;
			float x = pxg - rcos * n;
			float y = pyg - rsin * n;

			int tx = (x - std::floor(x)) * textureWidth;
			int ty = (y - std::floor(y)) * textureWidth;

			if (tx < 0)
				tx += textureWidth;
			if (ty < 0)
				ty += textureWidth;

			int fpos = int(j - 1) * screenWidth + i;
			int tpos = ty * textureWidth + tx;
			pixels[fpos] = fpixels[tpos];

		}

		//ceiling
		for (int j = 0; j < halfsH + offset; j++) {

			if (b[i] < j)
				break;

			float n = (halfsH / (halfsH - j + offset)) / dcos;
			float x = pxg + rcos * n;
			float y = pyg + rsin * n;

			int tx = (x - std::floor(x)) * textureWidth;
			int ty = (y - std::floor(y)) * textureWidth;

			if (tx < 0)
				tx += textureWidth;
			if (ty < 0)
				ty += textureWidth;

			int cpos = int(j) * screenWidth + i;
			int tpos = ty * textureWidth + tx;
			pixels[cpos] = cpixels[tpos];
		}

		ra += DR * FOV / screenWidth;
		if (ra < 0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;

	}

	return true;
}

void Raycasting::DrawFloorCeiling(TextureManager& txtManager, SDL_Texture* texture, 
	Atom::Surface& floor, Atom::Surface& ceiling, 
	int textureWidth, int threadCount,
	int* b, int* e) {

	// floorceiling
	Vector2D pos = entity->getPos();
	float angle = entity->getAngle();
	float ra = angle - DR * FOV / 2;
	float offset = tan(entity->getShear()) * screenHeight;

	Uint32* pixels = new Uint32[screenHeight * screenWidth];
	Uint32* fpixels = (Uint32*)floor.pixels;
	Uint32* cpixels = (Uint32*)ceiling.pixels;

	if (ra < 0)
		ra += 2 * M_PI;
	if (ra > 2 * M_PI)
		ra -= 2 * M_PI;

	memset(pixels, 255, screenWidth * screenHeight * sizeof(Uint32));
	
	CalcStruct** cDatas = new CalcStruct*[threadCount];
	SDL_Thread** threads = new SDL_Thread * [threadCount];

	for (int i = 0; i < threadCount; i++) {
		CalcStruct* cData = new CalcStruct;
		cData->screenHeight = screenHeight;
		cData->screenWidth = screenWidth;
		cData->offset = offset;
		cData->gridSize = gridSize;
		cData->textureWidth = textureWidth;
		cData->pos = pos;
		cData->pixels = pixels;
		cData->fpixels = fpixels;
		cData->cpixels = cpixels;
		cData->ra = ra;
		cData->angle = angle;
		cData->FOV = FOV;
		cData->begin = screenWidth * i / threadCount;
		cData->end = screenWidth * (i + 1) / threadCount;
		cData->b = b;
		cData->e = e;

		std::string tname = "fct" + std::to_string(i);
		SDL_Thread* fcT = SDL_CreateThread(fcCalc, tname.c_str(), (void*)cData);

		ra += DR * (screenWidth / threadCount) * FOV / screenWidth;
		if (ra < 0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;

		cDatas[i] = cData;
		threads[i] = fcT;
	}

	int t = true;
	for (int i = 0; i < threadCount; i++) {
		SDL_WaitThread(threads[i], &t);
	}

	for (int i = 0; i < threadCount; i++) {
		delete cDatas[i];
	}
	delete[] cDatas;

	SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(Uint32));
	delete[] pixels;
	txtManager.DrawP(texture, NULL, NULL);
}

/*
	MOVE
*/
void Raycasting::ListenKeys(double frameTime) {

	Vector2D pos = entity->getPos(), dir = entity->getDir();
	float angle = entity->getAngle();
	float shear = entity->getShear();
	//speed modifiers
	float moveSpeed = frameTime * MOVESPEED_MULTIPLER; //the constant value is in squares/second

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	int xo{MOVE_OFFSET}, yo{MOVE_OFFSET};
	
	if (dir.x < 0)
		xo = -xo;

	if (dir.y < 0)
		yo = -yo;

	//move forward if no wall in front of you
	if (currentKeyStates[SDL_SCANCODE_W]) {
		if (map[(int)(pos.y + yo) / gridSize ][(int)(pos.x + dir.x * moveSpeed + xo) / gridSize ] == 0) {
			entity->addPosX(dir.x * moveSpeed);
		}
		if (map[(int)(pos.y + dir.y * moveSpeed + yo) / gridSize][(int)(pos.x + xo) / gridSize] == 0) {
			entity->addPosY(dir.y * moveSpeed);
		}
	}
	//move backwards if no wall behind you
	if (currentKeyStates[SDL_SCANCODE_S]) {
		if (map[(int)(pos.y - yo) / gridSize][(int)(pos.x - dir.x * moveSpeed - xo) / gridSize] == 0) {
			entity->addPosX(-dir.x * moveSpeed);
		}
		if (map[(int)(pos.y - dir.y * moveSpeed - yo) / gridSize][(int)(pos.x - xo) / gridSize] == 0) {
			entity->addPosY(-dir.y * moveSpeed);
		}
	}

	if (currentKeyStates[SDL_SCANCODE_D]) {
		/*if (map[(int)(pos.y + dir.x * moveSpeed - MOVE_OFFSET) / gridSize][(int)(pos.x - dir.y * moveSpeed + MOVE_OFFSET) / gridSize] == 0) {
			entity->addPosX(-dir.y * moveSpeed);
			entity->addPosY(dir.x * moveSpeed);
		}*/

		if (map[(int)(pos.y + dir.x * moveSpeed + xo) / gridSize][(int)(pos.x - yo) / gridSize] == 0) {
			entity->addPosY(+dir.x * moveSpeed);
		}
		if (map[(int)(pos.y + xo) / gridSize][(int)(pos.x - dir.y * moveSpeed - yo) / gridSize] == 0) {
			entity->addPosX(-dir.y * moveSpeed);
		}
	}

	if (currentKeyStates[SDL_SCANCODE_A]) {
		/*if (map[(int)(pos.y - dir.x * moveSpeed + MOVE_OFFSET) / gridSize][(int)(pos.x + dir.y * moveSpeed - MOVE_OFFSET) / gridSize] == 0) {
			entity->addPosX(+dir.y * moveSpeed);
			entity->addPosY(-dir.x * moveSpeed);
		}*/

		if (map[(int)(pos.y - dir.x * moveSpeed - xo) / gridSize][(int)(pos.x + yo) / gridSize] == 0) {
			entity->addPosY(-dir.x * moveSpeed);
		}
		if (map[(int)(pos.y - xo) / gridSize][(int)(pos.x + dir.y * moveSpeed + yo) / gridSize] == 0) {
			entity->addPosX(+dir.y * moveSpeed);
		}

	}

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_MOUSEMOTION) {
			double rotSpeed = frameTime * ROTSPEED_MULTIPLER * abs(e.motion.xrel); //the constant value is in radians/second
			double rotSpeedY = frameTime * ROTSPEED_MULTIPLER * abs(e.motion.yrel); //the constant value is in radians/second
			//rotate to the right
			if (e.motion.xrel > 0) {
				angle += rotSpeed * 0.1;
				if (angle > 2 * M_PI)
					entity->setAngle(angle - 2 * M_PI);
				else
					entity->setAngle(angle);
				dir = { cos(angle), sin(angle) };
				entity->setDir(dir);
			}
			//rotate to the left
			if (e.motion.xrel < 0) {
				angle -= rotSpeed * 0.1;
				if (angle < 0)
					entity->setAngle(angle + 2 * M_PI);
				else
					entity->setAngle(angle);
				dir = { cos(angle), sin(angle) };
				entity->setDir(dir);
			}

			if (e.motion.yrel > 0) {
				shear -= rotSpeedY * 0.1;
				if (shear < -UPDOWN_OFFSET)
					entity->setShear(-UPDOWN_OFFSET);
				else
					entity->setShear(shear);
			}

			if (e.motion.yrel < 0) {
				shear += rotSpeedY * 0.1;
				if (shear > UPDOWN_OFFSET)
					entity->setShear(UPDOWN_OFFSET);
				else
					entity->setShear(shear);
			}
		}
	}

}