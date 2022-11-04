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
	Render(const char* title, int width, int heigth);
	~Render();
	void Clear();
	void Update();

	// �izim fonksiyonlar�
	void DrawRect(int x, int y, int w, int h, SDL_Color color);
	void DrawRectOutline(int x, int y, int w, int h, SDL_Color color);
private:
	SDL_Window* win;
	SDL_Renderer* renderer;
	const char* title;
	int width, height;
};

