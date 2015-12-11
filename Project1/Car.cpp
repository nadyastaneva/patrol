#include "Car.h";

int Car::prepareCarImage() {
	// Masks for car surface
	if (SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &car_c, &rmask, &gmask, &bmask, &amask) == SDL_FALSE) {
		SDL_Log("Failed to determine color masks from SDL_PIXELFORMAT_ABGR8888 (\"%s\").\n", SDL_GetError());
		return -1;
	}

	// Convert car image to a texture
	lodepng_result = lodepng_decode32_file(&car_buffer, &car_width, &car_height, car_imagefile);
	if (lodepng_result != 0) {
		SDL_Log("Failed lodepng_decode32_file() (\"%s\") for \"%s.\"\n", lodepng_error_text(lodepng_result), car_imagefile);
		return -1;
	}
	else {
		if ((car_surface = SDL_CreateRGBSurfaceFrom((void*)car_buffer, car_width, car_height, car_c, car_width * 4, rmask, gmask, bmask, amask)) == 0) {
			free(car_buffer);
			SDL_Log("Failed to create surface for logo image buffer (\"%s\").\n", SDL_GetError());
			return -1;
		}
		else {
			// Convert the car surface to a usable texture
			car_texture = SDL_CreateTextureFromSurface(renderer, car_surface);

			// Free the surface and buffer
			SDL_FreeSurface(car_surface);
			free(car_buffer);
			car_surface = NULL;
			car_buffer = NULL;

			// Error check
			if (car_texture == NULL) {
				SDL_Log("Failed to create texture from logo surface (\"%s\").\n", SDL_GetError());
				return -1;
			}
		}
	}
	return 0;
}


void Car::initialPosition() {
	SDL_RenderCopy(renderer, car_texture, NULL, &car_dest);

	car_src = car_dest;
}

void Car::updatePosition() {
	car_dest.x += car_xvel;
	car_dest.y += car_yvel;

	SDL_RenderCopy(renderer, car_texture, &car_src, &car_dest);

	car_src = car_dest;
}

Car::Car(SDL_Renderer *renderer)
{
	//initializing variables
	this->renderer = renderer;
	car_c = 0;
	car_imagefile = "car.png";
	car_buffer = NULL;
	car_width = 0, car_height = 0, lodepng_result = 0;
	car_surface = NULL;
	car_texture = NULL;
	car_src = { 0 };
	car_dest = { 0 };
	car_x = 0;
	car_y = 0;
	car_xvel = 0;
	car_yvel = 0;
	rmask = 0;
	gmask = 0;
	bmask = 0;
	amask = 0;

	//inital car destination on screen
	car_dest.x = 50;
	car_dest.y = 430;
	car_dest.w = 165;
	car_dest.h = 150;

}

Car::~Car()
{
}

void Car::destroyTexture() {
	if (car_texture)
		SDL_DestroyTexture(car_texture);
}

void Car::slower() {
	car_xvel = -10;
	updatePosition();
}

void Car::faster() {
	car_xvel = 10;
	updatePosition();
}

void Car::jump() {
	//tobedone car_yvel = 10; i posle animation za naobratno 
}
