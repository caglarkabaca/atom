#include "Raycasting.hpp"

#include <iostream>

Raycasting::Raycasting(int screenWidth, int screenHeight, int* map, int mapX, int mapY):
	screenWidth(screenWidth), screenHeight(screenHeight), mapX(mapX), mapY(mapY), entity(NULL) {

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

	Vector2D pos = entity->getPos(), dir = entity->getDir(), plane = entity->getPlane();

	for (int x = 0; x < screenWidth; x++) {

		// ray pos and raydir
		double cameraX = 2 * x / double(screenWidth) - 1; // x in camera space
		Vector2D rayDir{ dir.x + plane.x * cameraX, dir.y + plane.y * cameraX };

		// mapin hangi bloðundayýz
		Vector2DI Vmap{ int(pos.x), int(pos.y) };

		// length of ray from current position to next x or y-side
		Vector2D sideDist{};

		//length of ray from one x or y-side to next x or y-side
		Vector2D deltaDist = { (rayDir.x == 0) ? 1e30 : std::abs(1 / rayDir.x), (rayDir.y == 0) ? 1e30 : std::abs(1 / rayDir.y) };
		double perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		Vector2DI step{};

		int hit = 0; //was there a wall hit?
		int side{}; //was a NS or a EW wall hit?

		//calculate step and initial sideDist
		if (rayDir.x < 0) {
			step.x = -1;
			sideDist.x = (pos.x - Vmap.x) * deltaDist.x;
		}
		else {
			step.x = 1;
			sideDist.x = (Vmap.x + 1.0 - pos.x) * deltaDist.x;
		}

		if (rayDir.y < 0) {
			step.y = -1;
			sideDist.y = (pos.y - Vmap.y) * deltaDist.y;
		}
		else {
			step.y = 1;
			sideDist.y = (Vmap.y + 1.0 - pos.y) * deltaDist.y;
		}

		//perform DDA
		while (hit == 0)
		{
			//jump to next map square, either in x-direction, or in y-direction
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				Vmap.x += step.x;
				side = 0;
			}
			else
			{
				sideDist.y += deltaDist.y;
				Vmap.y += step.y;
				side = 1;
			}
			//Check if ray has hit a wall
			if (map[Vmap.x][Vmap.y] > 0) hit = 1;
		}

		//Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
		if (side == 0) perpWallDist = (sideDist.x - deltaDist.x);
		else          perpWallDist = (sideDist.y - deltaDist.y);

		//Calculate height of line to draw on screen
		int lineHeight = (int)(screenHeight / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + screenHeight / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + screenHeight / 2;
		if (drawEnd >= screenHeight)drawEnd = screenHeight - 1;

		//choose wall color
		SDL_Color color{};
		switch (map[Vmap.x][Vmap.y])
		{
		case 1:  color = {255, 0, 0, 255};  break; //red
		case 2:  color = {0, 255, 0, 255};  break; //green
		case 3:  color = {0, 0, 255, 255};   break; //blue
		case 4:  color = {255, 255, 255, 255};  break; //white
		default: color = {255, 255, 0, 255}; break; //yellow
		}

		//give x and y sides different brightness
		if (side == 1) { color.r *= 0.25; color.g *= 0.25; color.b *= 0.25; };

		//verLine(x, drawStart, drawEnd, color);
		render.DrawLine(x, drawStart, x, drawEnd, color);

	}

}

void Raycasting::ListenKeys(double frameTime) {

	Vector2D pos = entity->getPos(), dir = entity->getDir(), plane = entity->getPlane();
	//speed modifiers
	double moveSpeed = frameTime * 5.0; //the constant value is in squares/second

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	//move forward if no wall in front of you
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		if (map[int(pos.x + dir.x * moveSpeed)][int(pos.y)] == false) entity->addPosX(dir.x * moveSpeed);
		if (map[int(pos.x)][int(pos.y + dir.y * moveSpeed)] == false) entity->addPosY(dir.y * moveSpeed);
	}
	//move backwards if no wall behind you
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		if (map[int(pos.x - dir.x * moveSpeed)][int(pos.y)] == false) entity->addPosX(-1 * dir.x * moveSpeed);
		if (map[int(pos.x)][int(pos.y - dir.y * moveSpeed)] == false) entity->addPosY(-1 * dir.y * moveSpeed);
	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		if (map[int(pos.x - dir.y * moveSpeed)][int(pos.y)] == false) entity->addPosX(-1 * dir.y * moveSpeed);
		if (map[int(pos.x)][int(pos.y + dir.x * moveSpeed)] == false) entity->addPosY(dir.x * moveSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		if (map[int(pos.x + dir.y * moveSpeed)][int(pos.y)] == false) entity->addPosX(dir.y * moveSpeed);
		if (map[int(pos.x)][int(pos.y - dir.x * moveSpeed)] == false) entity->addPosY(-1 * dir.x * moveSpeed);
	}

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_MOUSEMOTION) {
			double rotSpeed = frameTime * 3.0 * abs(e.motion.xrel); //the constant value is in radians/second
			//rotate to the right
			if (e.motion.xrel > 0)
			{
				//both camera direction and camera plane must be rotated
				double oldDirX = dir.x;
				dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
				dir.y = oldDirX * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
				double oldPlaneX = plane.x;
				plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
				plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
				entity->setDir(dir);
				entity->setPlane(plane);
			}
			//rotate to the left
			if (e.motion.xrel < 0)
			{
				//both camera direction and camera plane must be rotated
				double oldDirX = dir.x;
				dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
				dir.y = oldDirX * sin(rotSpeed) + dir.y * cos(rotSpeed);
				double oldPlaneX = plane.x;
				plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
				plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
				entity->setDir(dir);
				entity->setPlane(plane);
			}
		}
	}
}