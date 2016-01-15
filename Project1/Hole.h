#pragma once

#include "Header.h"

class Hole
{
private:
	SDL_Renderer *renderer;
	int hole_h;
	char* hole_imagefile;
	unsigned char* hole_buffer;
	unsigned int hole_width;
	unsigned int hole_height;
	unsigned int lodepng_result;
	SDL_Surface* hole_surface;
	SDL_Texture* hole_texture;
	SDL_Rect hole_src;	
	Uint32 rmask, gmask, bmask, amask;


public:
	Hole(SDL_Renderer *renderer);
	~Hole();
	SDL_Rect hole_dest;
	int prepareHoleImage();
	void destroyTexture();
	void initialPosition();
	void updatePosition();

};

