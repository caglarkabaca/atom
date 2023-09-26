#include "SDL.h"

#include <SDL_ttf.h>
#include <string>

#include "Structs.hpp"
#include "Render.hpp"
#include "TextureManager.hpp"
#include "Raycasting.hpp"
#include "Entity.hpp"
#include "Map.hpp"

#include <math.h>
#include <iostream>

// ATOM
#include "Surface.hpp"

#define WIDTH	1280
#define HEIGHT	720
#define TITLE	"Title"

#define FPS		144
#define THREADCOUNT 4

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
	float angle = M_PI;

	Entity player(pos, angle);
	Raycasting rayCasting(WIDTH, HEIGHT, (int*)map, 24, 24, gridSize, FOV, THREADCOUNT);
	rayCasting.SetEntity(&player);

	int sizeArrayTexture = 6;
	SDL_Texture** textureArray = new SDL_Texture * [sizeArrayTexture];

	int txtSize = 128;
	textureArray[0] = NULL; //null
	textureArray[1] = txtManager.LoadTexture("assets/128/Atom_Blue.png");
	textureArray[2] = txtManager.LoadTexture("assets/128/Atom_Green.png");
	textureArray[3] = txtManager.LoadTexture("assets/128/Atom_Purple.png");
	textureArray[4] = txtManager.LoadTexture("assets/128/Atom_Red.png");
	textureArray[5] = txtManager.LoadTexture("assets/128/Atom_Yellow.png");

	SDL_Texture* texture = SDL_CreateTexture(render.getRenderer(), 
		SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	Atom::Surface floor("assets/128/Atom_Black.png");
	Atom::Surface ceiling("assets/128/Atom_LBlue.png");
	bool hideFC = false;

	float oldTime = 0, time = 0;
	bool isRunning = true;
	int delta = 0, fps_first = 0, fps_last = 0; // FPS limit�r� i�in

	float lfps = 1000, hfps = 0;
	float afps = 0;
	int fpsc = 1;

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("assets/OpenSans.ttf", 28);

	// game loop
	while (isRunning) {

        std::cout << "loop" << std::endl;

		//Fps limitor�
		fps_first = SDL_GetTicks();
		delta = fps_first - fps_last;
		if (delta <= 1000 / FPS)
			SDL_Delay(1000 / FPS - delta);
		float fps = (delta > 0) ? 1000.0f / delta : 0.0f;
		render.Clear();

		// EVENTLERI DUZENLEMEM LAZIM POLLEVENT SISTEMI KURMAM LAZIM
		// q => quit
		// f => toggle fullscreen
		// z => hide floor and ceiling
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_Q])
			isRunning = false;
		else if (currentKeyStates[SDL_SCANCODE_F])
			render.ToggleFullscreen();
		else if (currentKeyStates[SDL_SCANCODE_Z])
			hideFC = !hideFC;

		

		// B�t�n �izdirme kodu bu aral�kta olmal�

		// b ve e �uan ge�ici ama k�saca duvarlar�n arkas�n�n renderlanmamas�n� sa�l�or
		int b[WIDTH];
		int e[WIDTH];
		rayCasting.DrawWalls(txtManager, textureArray, txtSize, b, e);
		if (!hideFC)
			rayCasting.DrawFloorCeiling(txtManager, texture, floor, ceiling, txtSize, THREADCOUNT, b, e);
		rayCasting.DrawWalls(txtManager, textureArray, txtSize, b, e);

		// iki kere draw walls �ok az d���r�yor

		////Minimap
		//minimap.DrawMap(&render, map);
		//minimap.DrawPlayer(&render, player.getPos().x, player.getPos().y);

		oldTime = time;
		time = SDL_GetTicks();
		float frameTime = (time - oldTime) / 1000.0;
		rayCasting.ListenKeys(frameTime);
		fps_last = fps_first;

		//fps text
		if (fps > hfps)
			hfps = fps;
		if (fps < lfps)
			lfps = fps;
		afps += (fps - afps) / fpsc;
		++fpsc;
		std::string s = std::to_string((int)fps);
		std::string fpst = "FPS: " + s + " Threads: " + std::to_string(THREADCOUNT);
		SDL_Surface* surface = TTF_RenderText_Solid(font, fpst.c_str(), {255, 255, 255});
		SDL_Texture* texture = SDL_CreateTextureFromSurface(render.getRenderer(), surface);
		SDL_Rect dst{ 0, 0, surface->w, surface->h };
		int h = surface->h;
		SDL_RenderCopy(render.getRenderer(), texture, NULL, &dst);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		fpst = "Frame time: " + std::to_string(frameTime);
		surface = TTF_RenderText_Solid(font, fpst.c_str(), { 255, 255, 255 });
		texture = SDL_CreateTextureFromSurface(render.getRenderer(), surface);
		dst = { 0, h, surface->w, surface->h };
		SDL_RenderCopy(render.getRenderer(), texture, NULL, &dst);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		fpst = "Min: " + std::to_string(lfps) + " Max: " + std::to_string(hfps) + " Average: " + std::to_string(afps);
		surface = TTF_RenderText_Solid(font, fpst.c_str(), { 255, 255, 255 });
		texture = SDL_CreateTextureFromSurface(render.getRenderer(), surface);
		dst = { 0, 2*h, surface->w, surface->h };
		SDL_RenderCopy(render.getRenderer(), texture, NULL, &dst);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		render.Update();
	}

	// free texture datas
	for (int i = 0; i < sizeArrayTexture; i++) {
		SDL_DestroyTexture(textureArray[i]);
		textureArray[i] = NULL;
	}
	delete[] textureArray;

	// otomatik render.~Render() �a��r�yor, gerek yok yazmaya
	// render.~Render();

	TTF_CloseFont(font);
	TTF_Quit();

	return 0;
}