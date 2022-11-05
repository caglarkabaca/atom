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

	// çizim fonksiyonlarý
	void DrawRect(int x, int y, int w, int h, SDL_Color color);
	void DrawRectOutline(int x, int y, int w, int h, SDL_Color color);

	static SDL_Renderer* renderer; //renderer'ý static ve public yaptým. Çünkü TextureManager'da kullanmam gerekti.

private:
	SDL_Window* win;

	const char* title;
	int width, height;
};

