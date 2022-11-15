#pragma once
#include <SDL.h>
#include <SDL_image.h>

namespace Atom {
	class Surface {
	public:
		Surface( const char* path);
		~Surface();
		
		void* pixels;
		SDL_Surface* textureSurface;
		SDL_Surface* convertedSurface;
	};
}

