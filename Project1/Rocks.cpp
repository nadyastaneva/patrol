#include "Rocks.h"

int Rocks::prepareRockImage() {
	// Masks for rock surface
	if (SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &rock_r, &rmask, &gmask, &bmask, &amask) == SDL_FALSE) {
		SDL_Log("Failed to determine color masks from SDL_PIXELFORMAT_ABGR8888 (\"%s\").\n", SDL_GetError());
		return -1;
	}

	// Convert rock image to a texture
	lodepng_result = lodepng_decode32_file(&rock_buffer, &rock_width, &rock_height, rock_imagefile);
	if (lodepng_result != 0) {
		SDL_Log("Failed lodepng_decode32_file() (\"%s\") for \"%s.\"\n", lodepng_error_text(lodepng_result), rock_imagefile);
		return -1;
	}
	else {
		if ((rock_surface = SDL_CreateRGBSurfaceFrom((void*)rock_buffer, rock_width, rock_height, rock_r, rock_width * 4, rmask, gmask, bmask, amask)) == 0) {
			free(rock_buffer);
			SDL_Log("Failed to create surface for logo image buffer (\"%s\").\n", SDL_GetError());
			return -1;
		}
		else {
			// Convert the rock surface to a usable texture
			rock_texture = SDL_CreateTextureFromSurface(renderer, rock_surface);

			// Free the surface and buffer
			SDL_FreeSurface(rock_surface);
			free(rock_buffer);
			rock_surface = NULL;
			rock_buffer = NULL;

			// Error check
			if (rock_texture == NULL) {
				SDL_Log("Failed to create texture from logo surface (\"%s\").\n", SDL_GetError());
				return -1;
			}
		}
	}
	return 0;
}

void Rocks::initialPosition() {
	SDL_RenderCopy(renderer, rock_texture, NULL, &rock_dest);
}

void Rocks::updatePosition() { //scrolling
	rock_dest.x -= 20;
	SDL_RenderCopy(renderer, rock_texture, &rock_src, &rock_dest);
	if (rock_dest.x <= -81) {
		destroyTexture(); //destroys texture when off-screen
	}
}

Rocks::Rocks(SDL_Renderer *renderer)
{
	//initializing variables
	this->renderer = renderer;
	rock_r = 0;
	rock_imagefile = "Graphics/rock2.png";
	rock_buffer = NULL;
	rock_width = 0, rock_height = 0, lodepng_result = 0;
	rock_surface = NULL;
	rock_texture = NULL;
	rock_src = { 0 };
	rock_dest = { 0 };
	rmask = 0, gmask = 0, bmask = 0, amask = 0;

	//destination of first rock
	rock_dest.x = 850;
	rock_dest.y = 530;
	rock_dest.w = 80;
	rock_dest.h = 52;
}

Rocks::~Rocks()
{
}

void Rocks::destroyTexture() {
	if (rock_texture)
		SDL_DestroyTexture(rock_texture);
}