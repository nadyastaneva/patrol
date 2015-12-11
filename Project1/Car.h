#pragma once

#include "Header.h";

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
	int car_x;
	int car_y;
	int car_xvel;
	int car_yvel;
	Uint32 rmask = 0;
	Uint32 gmask = 0;
	Uint32 bmask = 0;
	Uint32 amask = 0;

	void updatePosition();

public:
	Car(SDL_Renderer *renderer);
	~Car();

	int prepareCarImage();
	void destroyTexture();

	void initialPosition();
	void slower();
	void faster();
	void jump();
};
