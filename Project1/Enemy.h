#pragma once

#include "Header.h"

class Enemy
{
private:
	SDL_Renderer *renderer;
	int enemy_r;
	char* enemy_imagefile;
	unsigned char* enemy_buffer;
	unsigned int enemy_width;
	unsigned int enemy_height;
	unsigned int lodepng_result;
	SDL_Surface* enemy_surface;
	SDL_Texture* enemy_texture;
	SDL_Rect enemy_src;
	Uint32 rmask, gmask, bmask, amask;

public:
	Enemy(SDL_Renderer *renderer);
	~Enemy();
	SDL_Rect enemy_dest;
	int prepareEnemyImage();
	void destroyTexture();
	void initialPosition();
	void updatePosition();
};