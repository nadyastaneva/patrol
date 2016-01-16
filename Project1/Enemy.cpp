#include "Enemy.h"

int Enemy::prepareEnemyImage() {
	// Masks for enemy surface
	if (SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &enemy_r, &rmask, &gmask, &bmask, &amask) == SDL_FALSE) {
		SDL_Log("Failed to determine color masks from SDL_PIXELFORMAT_ABGR8888 (\"%s\").\n", SDL_GetError());
		return -1;
	}

	// Convert enemy image to a texture
	lodepng_result = lodepng_decode32_file(&enemy_buffer, &enemy_width, &enemy_height, enemy_imagefile);
	if (lodepng_result != 0) {
		SDL_Log("Failed lodepng_decode32_file() (\"%s\") for \"%s.\"\n", lodepng_error_text(lodepng_result), enemy_imagefile);
		return -1;
	}
	else {
		if ((enemy_surface = SDL_CreateRGBSurfaceFrom((void*)enemy_buffer, enemy_width, enemy_height, enemy_r, enemy_width * 4, rmask, gmask, bmask, amask)) == 0) {
			free(enemy_buffer);
			SDL_Log("Failed to create surface for logo image buffer (\"%s\").\n", SDL_GetError());
			return -1;
		}
		else {
			// Convert the enemy surface to a usable texture
			enemy_texture = SDL_CreateTextureFromSurface(renderer, enemy_surface);

			// Free the surface and buffer
			SDL_FreeSurface(enemy_surface);
			free(enemy_buffer);
			enemy_surface = NULL;
			enemy_buffer = NULL;

			// Error check
			if (enemy_texture == NULL) {
				SDL_Log("Failed to create texture from logo surface (\"%s\").\n", SDL_GetError());
				return -1;
			}
		}
	}
	return 0;
}

void Enemy::initialPosition() {
	SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_dest);
}

void Enemy::updatePosition() { //scrolling
	enemy_dest.x -= 12;
	SDL_RenderCopy(renderer, enemy_texture, &enemy_src, &enemy_dest);
}

Enemy::Enemy(SDL_Renderer *renderer)
{
	//initializing variables
	this->renderer = renderer;
	enemy_r = 0;
	enemy_imagefile = "Graphics/pigeons.png";
	enemy_buffer = NULL;
	enemy_width = 0, enemy_height = 0, lodepng_result = 0;
	enemy_surface = NULL;
	enemy_texture = NULL;
	enemy_src = { 0 };
	enemy_dest = { 0 };
	rmask = 0, gmask = 0, bmask = 0, amask = 0;

	//destination of first enemy
	enemy_dest.x = 1050;
	enemy_dest.y = 90;
	enemy_dest.w = 100;
	enemy_dest.h = 121;
}

Enemy::~Enemy()
{
}

void Enemy::destroyTexture() {
	if (enemy_texture)
		SDL_DestroyTexture(enemy_texture);
}