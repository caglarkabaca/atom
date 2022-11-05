#include <SDL.h>
#include <math.h> // sin()
#include "Render.hpp"
#include "TextureManager.hpp"

#define WIDTH	1366
#define HEIGHT	768 //typo
#define TITLE	"Title"

#define FPS		60

int main(int, char* []) {

	Render render(TITLE, WIDTH, HEIGHT);
	TextureManager textureManager(render);

	SDL_Texture* grass = textureManager.LoadTexture("assets/block1.png"); // Bunu döngünün içine sokunca Memory kullanýmý lineer artýyor. Buraya çektim

	bool isRunning = true;
	int delta = 0, fps_first = 0, fps_last = 0; // FPS limitörü için
	SDL_Event event;
	double i = 1, j = 1;
	// game loop
	
	while (isRunning) {
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				isRunning = false;

		//Fps limitorü
		fps_first = SDL_GetTicks();
		delta = fps_first - fps_last;
		if (delta <= 1000 / FPS)
			continue;

		render.Clear();

		// Bütün çizdirme kodu bu aralýkta olmalý

		if (i > 360)
			j = -1;
		if (i < 0)
			j = 1;
		i += j;

		render.DrawRect(500, 300 + 45 * sin(i * 0.1), 64, 64, { 255, 0, 0, 255 });
		render.DrawRectOutline(575, 300 + 45 * sin(-i * 0.1), 64, 64, {0, 255, 0, 255});
		render.DrawRect(650, 300 + 45 * sin(i * 0.1), 64, 64, { 0, 0, 255, 255 });


		SDL_Rect src{0, 0, 64, 64}, dest{575, 300 + 45 * sin(-i * 0.1), 64, 64};
		textureManager.Draw(grass, src, dest);

		render.Update();
		fps_last = fps_first;
	}

	// otomatik render.~Render() çaðýrýyor, gerek yok yazmaya
	// render.~Render();
	return 0;
}