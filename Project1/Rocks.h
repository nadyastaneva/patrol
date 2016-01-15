#pragma once

#include "Header.h"

class Rocks
{
private:
	SDL_Renderer *renderer;
	int rock_r;
	char* rock_imagefile;
	unsigned char* rock_buffer;
	unsigned int rock_width;
	unsigned int rock_height;
	unsigned int lodepng_result;
	SDL_Surface* rock_surface;
	SDL_Texture* rock_texture;
	SDL_Rect rock_src;
	Uint32 rmask, gmask, bmask, amask;

public:
	Rocks(SDL_Renderer *renderer);
	~Rocks();
	SDL_Rect rock_dest;

	int prepareRockImage();
	void destroyTexture();

	void initialPosition();
	void updatePosition();
};

