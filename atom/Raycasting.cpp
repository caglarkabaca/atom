#include "Raycasting.hpp"
#include <iostream>

#define MOVESPEED_MULTIPLER 500.0
#define ROTSPEED_MULTIPLER 10
#define UPDOWN_OFFSET M_PI / 8
#define MOVE_OFFSET 5 // duvarýn içine girmeyi engellemek için
#define DEPTH_OFFSET 0.4 // 0 - 1 arasý deðerler, 1 komple aydýnlýk yapar

// posdan belirli açýya ýþýn yollar ve ýþýný izler
// ýþýnýn duvara çarptýðý yerde kordinatýný verir
Vector2D castTheRay(double ra, Vector2D pos, int mapSize, int gridSize, int** map, int& side) {
	double distH{ INT_MAX }, distV{ INT_MAX }; // placeholder 
	double hx{ pos.x }, hy{ pos.y };
	double vx{ pos.x }, vy{ pos.y };
	int dof = 0;
	// horizontal

	double rx{}, ry{}, xo{}, yo{};

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

	if (distV < distH) {
		side = 0;
		return { vx, vy };
	}
	else {
		side = 1;
		return { hx, hy };
	}

}

// posdan belirli açýya ýþýn yollar ve ýþýný izler
// ýþýnýn zeminin olan yerin yerde kordinatýný verir
Vector2D castTheRayFloorCeiling(double ra, Vector2D pos, int mapSize, int gridSize, int** map, int& side) {
	double distH{ INT_MAX }, distV{ INT_MAX }; // placeholder 
	double hx{ pos.x }, hy{ pos.y };
	double vx{ pos.x }, vy{ pos.y };
	int dof = 0;
	// horizontal

	double rx{}, ry{}, xo{}, yo{};

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
		if (mx < mapSize && my < mapSize && map[my][mx] == 0) {
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
		if (mx < mapSize && my < mapSize && map[my][mx] == 0) {
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

	if (distV < distH) {
		side = 0;
		return { vx, vy };
	}
	else {
		side = 1;
		return { hx, hy };
	}

}

Raycasting::Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY, int gridSize, double FOV):
	screenWidth(screenWidth), screenHeight(screenHeight), mapX(mapX), mapY(mapY), entity(NULL), gridSize(gridSize), FOV(FOV) {

	// memory allociton
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

void Raycasting::DrawPixels(Render& render) {

	Vector2D pos{ entity->getPos() }, dir{ entity->getDir() };
	double angle = entity->getAngle();
	double shear = entity->getShear();

	double DR = 0.0174533;
	const double sizeOfColumn = screenWidth / FOV;

	// bunlar map classýnda olucak
	int mapSize = mapY;
	// BURASI DEÐÝÞMESÝ LAZIM ÞUAN SADECE KARE MAPLER ÇALIÞIYOR

	// cast the rays
	double rx{}, ry{}, ra{}, xo{}, yo{};
	ra = angle - DR * FOV / 2;

	if (ra < 0)
		ra += 2 * M_PI;
	if (ra > 2 * M_PI)
		ra -= 2 * M_PI;

	for (int i = 0; i < screenWidth; i++) {
		int side = 0;
		Vector2D ray = castTheRay(ra, pos, mapSize, gridSize, map, side);
		int mx{ (int)ray.x / gridSize }, my{ (int)ray.y / gridSize };
		double dist{ sqrt(((ray.x - pos.x) * (ray.x - pos.x) + (ray.y - pos.y) * (ray.y - pos.y))) };

		// bulunan kare (mx, my)
		// tam kordinat için (hx, hy) ve (vx, vy) yi karþýlaþtýrýp dist i küçük olaný alýcaz
		// mesafe dist

		// eðer minimapte ray i göstermek istiyorsak buraya ekleyebiliriz

		// fix fisheye
		double ca = angle - ra;
		if (ca < 0)
			ca -= 2 * M_PI;
		dist *= cos(ca);

		double lineH = (gridSize * screenHeight) / dist;
		/*if (lineH > screenHeight)
			lineH = screenHeight;*/
		double lineO = screenHeight / 2 - lineH / 2;

		// up down movement
		double offset = tan(shear) * screenHeight;
		lineO += offset;

		SDL_Color color;
		switch (map[my][mx])
		{
		case 1:  color = { 255, 0, 0, 255 };  break; //red
		case 2:  color = { 0, 255, 0, 255 };  break; //green
		case 3:  color = { 0, 0, 255, 255 };   break; //blue
		case 4:  color = { 255, 255, 255, 255 };  break; //white
		default: color = { 255, 255, 0, 255 }; break; //yellow
		}

		if (side == 1) { color.r *= 0.25; color.g *= 0.25; color.b *= 0.25; };

		render.DrawRect(i, lineO, 1, lineH, color);

		ra += DR * FOV / screenWidth;
		if (ra < 0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;

	}

}

void Raycasting::DrawPixelsTextured(TextureManager& txtManager, SDL_Texture** textureArray, int textureWidth) {

	Vector2D pos{ entity->getPos() }, dir{ entity->getDir() };
	double angle = entity->getAngle();
	double shear = entity->getShear();

	double DR = 0.0174533;
	const double sizeOfColumn = screenWidth / FOV;

	// bunlar map classýnda olucak
	int mapSize = mapY;
	// BURASI DEÐÝÞMESÝ LAZIM ÞUAN SADECE KARE MAPLER ÇALIÞIYOR

	// cast the rays
	double rx{}, ry{}, ra{}, xo{}, yo{};
	ra = angle - DR * FOV / 2;

	if (ra < 0)
		ra += 2 * M_PI;
	if (ra > 2 * M_PI)
		ra -= 2 * M_PI;

	for (int i = 0; i < screenWidth; i++) {
		int side = 0;
		Vector2D ray = castTheRay(ra, pos, mapSize, gridSize, map, side);
		int mx{ (int)ray.x / gridSize }, my{ (int)ray.y / gridSize };
		double dist{ sqrt(((ray.x - pos.x) * (ray.x - pos.x) + (ray.y - pos.y) * (ray.y - pos.y)))};

		// bulunan kare (mx, my)
		// tam kordinat için (hx, hy) ve (vx, vy) yi karþýlaþtýrýp dist i küçük olaný alýcaz
		// mesafe dist

		// eðer minimapte ray i göstermek istiyorsak buraya ekleyebiliriz

		// fix fisheye
		double ca = angle - ra;
		if (ca < 0)
			ca -= 2 * M_PI;
		dist *= cos(ca);

		double lineH = (gridSize * screenHeight) / dist;
		//if (lineH > screenHeight) // yukarý aþaðý için kapattým
		//	lineH = screenHeight;
		double lineO = screenHeight / 2 - lineH / 2;

		// up down movement
		double offset = tan(shear) * screenHeight;
		lineO += offset;

		double wallX{};
		if (side == 0) {
			wallX = ray.x - (mx * gridSize);
		}
		else {
			wallX = ray.y - (my * gridSize);
		}

		int tx = (int) (wallX * textureWidth / gridSize);


		// ters texturelar düzeliyor
		if (side == 0 && ra < M_PI)
			tx = textureWidth - tx;

		if (side == 1 && ra > M_PI / 2 && ra < 3 * M_PI / 2)
			tx = textureWidth - tx;

		// uzaklar daha karanlýk
		double darkness = lineH / screenHeight + DEPTH_OFFSET;
		if (darkness > 1)
			darkness = 1;
		SDL_SetTextureColorMod(textureArray[map[my][mx]], 255 * darkness, 255 * darkness, 255 * darkness);

		SDL_Rect src{ tx, 0, 1, textureWidth };
		SDL_Rect dst{ i, lineO, 1, lineH };
		txtManager.Draw(textureArray[map[my][mx]], src, dst);

		ra += DR * FOV / screenWidth;
		if (ra < 0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;

	}

}

void Raycasting::ListenKeys(double frameTime) {

	Vector2D pos = entity->getPos(), dir = entity->getDir();
	double angle = entity->getAngle();
	double shear = entity->getShear();
	//speed modifiers
	double moveSpeed = frameTime * MOVESPEED_MULTIPLER; //the constant value is in squares/second

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