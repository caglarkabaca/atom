#include "Surface.hpp"

using namespace Atom;

Surface::Surface(const char* path): pixels(NULL), textureSurface(NULL), convertedSurface(NULL) {

	textureSurface = IMG_Load(path);
	convertedSurface = SDL_ConvertSurfaceFormat(textureSurface, SDL_PIXELFORMAT_RGBA32, 0);
	pixels = convertedSurface->pixels;

}

Surface::~Surface() {
	if (textureSurface != NULL) {
		SDL_FreeSurface(textureSurface);
		textureSurface = NULL;
	}
	SDL_FreeSurface(convertedSurface);
}