#include "Hole.h"

int Hole::prepareHoleImage() {
	// Masks for hole surface
	if (SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &hole_h, &rmask, &gmask, &bmask, &amask) == SDL_FALSE) {
		SDL_Log("Failed to determine color masks from SDL_PIXELFORMAT_ABGR8888 (\"%s\").\n", SDL_GetError());
		return -1;
	}

	// Convert hole image to a texture
	lodepng_result = lodepng_decode32_file(&hole_buffer, &hole_width, &hole_height, hole_imagefile);
	if (lodepng_result != 0) {
		SDL_Log("Failed lodepng_decode32_file() (\"%s\") for \"%s.\"\n", lodepng_error_text(lodepng_result), hole_imagefile);
		return -1;
	}
	else {
		if ((hole_surface = SDL_CreateRGBSurfaceFrom((void*)hole_buffer, hole_width, hole_height, hole_h, hole_width * 4, rmask, gmask, bmask, amask)) == 0) {
			free(hole_buffer);
			SDL_Log("Failed to create surface for logo image buffer (\"%s\").\n", SDL_GetError());
			return -1;
		}
		else {
			// Convert the hole surface to a usable texture
			hole_texture = SDL_CreateTextureFromSurface(renderer, hole_surface);

			// Free the surface and buffer
			SDL_FreeSurface(hole_surface);
			free(hole_buffer);
			hole_surface = NULL;
			hole_buffer = NULL;

			// Error check
			if (hole_texture == NULL) {
				SDL_Log("Failed to create texture from logo surface (\"%s\").\n", SDL_GetError());
				return -1;
			}
		}
	}
	return 0;
}

void Hole::initialPosition() {
	SDL_RenderCopy(renderer, hole_texture, NULL, &hole_dest);
}

void Hole::updatePosition() { //scrolling
	hole_dest.x -= 20;
	SDL_RenderCopy(renderer, hole_texture, &hole_src, &hole_dest);
}

Hole::Hole(SDL_Renderer *renderer)
{
	//initializing variables
	this->renderer = renderer;
	hole_h = 0;
	hole_imagefile = "Graphics/hole2.png";
	hole_buffer = NULL;
	hole_width = 0, hole_height = 0, lodepng_result = 0;
	hole_surface = NULL;
	hole_texture = NULL;
	hole_src = { 0 };
	hole_dest = { 0 };
	rmask = 0, gmask = 0, bmask = 0, amask = 0;

	//destination of first hole
	hole_dest.x = 650;
	hole_dest.y = 530;
	hole_dest.w = 80;
	hole_dest.h = 47;
}

Hole::~Hole()
{
}

void Hole::destroyTexture() {
	if (hole_texture)
		SDL_DestroyTexture(hole_texture);
}