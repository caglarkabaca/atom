#pragma once

#include <SDL.h>

/*
* 
*	istiyorsan classýn ismini deðiþtir
*	benim aklýma fikir gelmedi
*	direk burada bütün cizim þeylerini ayarlýyalým
* 
*/

class Render
{
public:
	Render(const char* title, int width, int height);
	~Render();
	void Clear();
	void Update();

	// getter | static biraz sýkýntý bu da ayný þekilde bi sýkýntýsý olabilir 
	//			sonuçta pointer yoluyoruz, geçici böyle çözelim
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();

	// çizim fonksiyonlarý
	void DrawRect(int x, int y, int w, int h, SDL_Color color);
	void DrawRectOutline(int x, int y, int w, int h, SDL_Color color);

	void DrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

private:
	SDL_Window* win;
	SDL_Renderer* renderer;
	const char* title;
	int width, height;
};

