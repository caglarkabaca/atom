#include <SDL.h>

#include <SDL_ttf.h>
#include <string>

#include "Structs.hpp"
#include "Render.hpp"
#include "TextureManager.hpp"
#include "Raycasting.hpp"
#include "Entity.hpp"
#include "Map.hpp"
#include <iostream>

// ATOM
#include "Surface.hpp"

#define WIDTH	1280
#define HEIGHT	720
#define TITLE	"Title"

#define FPS		144

int main(int, char* []) {
	Map minimap;
	Render render(TITLE, WIDTH, HEIGHT);
	TextureManager txtManager(render);

	int map[24][24] = {
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	int gridSize = 128;

	double FOV = 70;
	Vector2D pos = { 22 * gridSize, 12 * gridSize };
	double angle = M_PI;

	Entity player(pos, angle);
	Raycasting rayCasting(WIDTH, HEIGHT, (int*)map, 24, 24, gridSize, FOV);
	rayCasting.SetEntity(&player);

	int sizeArrayTexture = 6;
	SDL_Texture** textureArray = new SDL_Texture * [sizeArrayTexture];

	int txtSize = 512;
	textureArray[0] = NULL; //null
	textureArray[1] = txtManager.LoadTexture("assets/gridbox/blue3.png");
	textureArray[2] = txtManager.LoadTexture("assets/gridbox/green2.png");
	textureArray[3] = txtManager.LoadTexture("assets/gridbox/brown.png");
	textureArray[4] = txtManager.LoadTexture("assets/gridbox/red.png");
	textureArray[5] = txtManager.LoadTexture("assets/gridbox/yellow.png");


	SDL_Texture* texture = SDL_CreateTexture(render.getRenderer(), 
		SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
	Atom::Surface floor("assets/gridbox/grey3.png");
	Atom::Surface ceiling("assets/gridbox/orange.png");

	double oldTime = 0, time = 0;


	bool isRunning = true;
	int delta = 0, fps_first = 0, fps_last = 0; // FPS limitörü için

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("assets/OpenSans.ttf", 32);

	SDL_Event event;
	// game loop
	while (isRunning) {
		//Fps limitorü
		fps_first = SDL_GetTicks();
		delta = fps_first - fps_last;
		if (delta <= 1000 / FPS)
			SDL_Delay(1000 / FPS - delta);
		double fps = (delta > 0) ? 1000.0f / delta : 0.0f;
		render.Clear();

		// q => quit
		// f => toggle fullscreen
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				isRunning = false;
			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.scancode == SDL_SCANCODE_Q)
					isRunning = false;
			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.scancode == SDL_SCANCODE_F)
					render.ToggleFullscreen();
		}

		// Bütün çizdirme kodu bu aralýkta olmalý

		rayCasting.DrawFloorCeiling(txtManager, texture, floor, ceiling, txtSize);
		rayCasting.DrawWalls(txtManager, textureArray, txtSize);

		////Minimap
		//minimap.DrawMap(&render, map);
		//minimap.DrawPlayer(&render, player.getPos().x, player.getPos().y);

		//fps text
		std::string s = std::to_string((int)fps);
		char fpst[100];
		sprintf_s(fpst, "FPS: %s Threads: 2", s);
		SDL_Surface* surface = TTF_RenderText_Solid(font, fpst, { 255, 255, 255 });
		SDL_Texture* texture = SDL_CreateTextureFromSurface(render.getRenderer(), surface);
		SDL_Rect dst{ 0, 0, surface->w, surface->h };
		SDL_RenderCopy(render.getRenderer(), texture, NULL, &dst);

		render.Update();
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0;
		rayCasting.ListenKeys(frameTime);
		fps_last = fps_first;

	}

	// free texture datas
	for (int i = 0; i < sizeArrayTexture; i++) {
		SDL_DestroyTexture(textureArray[i]);
		textureArray[i] = NULL;
	}
	delete[] textureArray;

	// otomatik render.~Render() çaðýrýyor, gerek yok yazmaya
	// render.~Render();

	TTF_CloseFont(font);
	TTF_Quit();

	return 0;
}