#pragma once

#include <SDL.h>

/*
* 
*	istiyorsan class�n ismini de�i�tir
*	benim akl�ma fikir gelmedi
*	direk burada b�t�n cizim �eylerini ayarl�yal�m
* 
*/

class Render
{
public:
	Render(const char* title, int width, int height);
	~Render();
	void Clear();
	void Update();

	// �izim fonksiyonlar�
	void DrawRect(int x, int y, int w, int h, SDL_Color color);
	void DrawRectOutline(int x, int y, int w, int h, SDL_Color color);

	static SDL_Renderer* renderer; //renderer'� static ve public yapt�m. ��nk� TextureManager'da kullanmam gerekti.

private:
	SDL_Window* win;

	const char* title;
	int width, height;
};

