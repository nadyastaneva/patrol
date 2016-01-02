#pragma once

#include "Header.h"

class Car
{
private:
	SDL_Renderer *renderer;
	int car_c;
	char* car_imagefile;
	unsigned char* car_buffer;
	unsigned int car_width;
	unsigned int car_height;
	unsigned int lodepng_result;
	SDL_Surface* car_surface;
	SDL_Texture* car_texture;
	SDL_Rect car_src;
   SDL_Rect car_dest;
	int car_x, car_y, car_xvel, car_yvel;
	Uint32 rmask, gmask, bmask, amask;

	void updatePosition();

public:
	Car(SDL_Renderer *renderer);
	~Car();

	int prepareCarImage();
	void destroyTexture();
	int start_jump = 0;
	
	void initialPosition();
	void slower();
	void faster();
	void jump();
};
