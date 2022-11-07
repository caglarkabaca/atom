#include <SDL.h>

#include "Structs.hpp"
#include "Render.hpp"
#include "TextureManager.hpp"
#include "Raycasting.hpp"
#include "Entity.hpp"

#define WIDTH	1366
#define HEIGHT	768
#define TITLE	"Title"

#define FPS		144

int main(int, char* []) {

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

	Raycasting rayCasting(WIDTH, HEIGHT, (int*)map, 24, 24);
	Entity player({22, 12}, {-1, 0}, {0, 0.66});

	int sizeArrayTexture = 5;
	SDL_Texture** textureArray = new SDL_Texture* [sizeArrayTexture];

	textureArray[0] = txtManager.LoadTexture("assets/pics/colorstone.png");
	textureArray[1] = txtManager.LoadTexture("assets/pics/bluestone.png");
	textureArray[2] = txtManager.LoadTexture("assets/pics/greystone.png");
	textureArray[3] = txtManager.LoadTexture("assets/pics/mossy.png");
	textureArray[4] = txtManager.LoadTexture("assets/pics/redbrick.png");

	rayCasting.SetEntity(&player);

	double oldTime = 0, time = 0;

	bool isRunning = true;
	int delta = 0, fps_first = 0, fps_last = 0; // FPS limitörü için
	SDL_Event event;
	// game loop
	while (isRunning) {
		//Fps limitorü
		fps_first = SDL_GetTicks();
		delta = fps_first - fps_last;
		if (delta <= 1000 / FPS)
			continue;

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
		//rayCasting.DrawPixels(render);
		rayCasting.DrawPixelsTextured(txtManager, textureArray);
		render.Update();

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
	return 0;
}