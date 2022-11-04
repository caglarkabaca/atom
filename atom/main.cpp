#include <SDL.h>
#include <math.h> // sin()
#include "Render.hpp"

#define WIDTH	1366
#define HEIGTH	768
#define TITLE	"Title"

#define FPS		60

int main(int, char* []) {

	Render render(TITLE, WIDTH, HEIGTH);

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

		// -----------------------------


		render.Update();
		fps_last = fps_first;
	}

	// otomatik render.~Render() çaðýrýyor, gerek yok yazmaya
	// render.~Render();
	return 0;
}