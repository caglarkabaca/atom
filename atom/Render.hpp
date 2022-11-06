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

	// getter | static biraz s�k�nt� bu da ayn� �ekilde bi s�k�nt�s� olabilir 
	//			sonu�ta pointer yoluyoruz, ge�ici b�yle ��zelim
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();

	// �izim fonksiyonlar�
	void DrawRect(int x, int y, int w, int h, SDL_Color color);
	void DrawRectOutline(int x, int y, int w, int h, SDL_Color color);

	void DrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

private:
	SDL_Window* win;
	SDL_Renderer* renderer;
	const char* title;
	int width, height;
};

