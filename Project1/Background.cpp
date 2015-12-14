#include "Background.h"

int Background::prepareBGImage() {
	// Turn background to surface
	if ((backgnd_surface = SDL_LoadBMP(backgnd_imagefile)) == NULL) {
		SDL_Log("Failed load background image (\"%s\") for \"%s\".\n", SDL_GetError(), backgnd_imagefile);
		return -1;
	}
	else {
		// Convert to texture
		backgnd_texture = SDL_CreateTextureFromSurface(renderer, backgnd_surface);
		SDL_FreeSurface(backgnd_surface); // The back surface is no longer needed now that we have a texture for it.
		backgnd_surface = NULL;
		if (backgnd_texture == NULL) {
			SDL_Log("Failed to create texture from background surface (\"%s\").\n", SDL_GetError());
			return -1;
		}
	}
	return 0;
}

void Background::initialPosition() { //loading on screen
	SDL_RenderCopy(renderer, backgnd_texture, NULL, &bg_dest);
}

void Background::updatePosition() { //scrolling
	bg_src = bg_dest;
	if (-(bg_dest.x) >= bg_dest.w) {
		bg_dest.x = 0;
	}
	bg_dest.x -= 20;
	SDL_RenderCopy(renderer, backgnd_texture, &bg_src, &bg_dest);
}

Background::Background(SDL_Renderer *renderer)
{
	//initializing variables
	this->renderer = renderer;
	backgnd_imagefile = "Graphics/street.bmp";
	backgnd_surface = NULL;
	backgnd_texture = NULL;
	bg_src = { 0 };
	bg_dest = { 0 };
	rmask = 0, gmask = 0, bmask = 0, amask = 0;
	bg_width = 5120;

	//initial background destination
	bg_dest.x = 0;
	bg_dest.y = 0;
	bg_dest.w = 5120;
	bg_dest.h = 720;
}

Background::~Background()
{
}

void Background::destroyTexture() {
	if (backgnd_texture)
		SDL_DestroyTexture(backgnd_texture);
}
