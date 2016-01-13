#pragma once

#include "Header.h";

class Background
{
private:
	SDL_Renderer *renderer;
	char* backgnd_imagefile;
	SDL_Surface* backgnd_surface;
	SDL_Texture* backgnd_texture;
	SDL_Rect bg_src;
	SDL_Rect bg_dest;
	Uint32 rmask, gmask, bmask, amask;
	int bg_width;
	int count;

public:
	Background(SDL_Renderer *renderer);
	~Background();

	int Background::prepareBGImage();
	void Background::destroyTexture();

	void Background::initialPosition();
	void Background::updatePosition();
};