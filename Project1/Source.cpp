
#include "SDL.h"
#include "SDL_image.h"
#include "lodepng.h"
#include <iostream>

using namespace std;



//kdk
//dd


int main(int argc, char* argv[])
{
	

	SDL_Window *window            = nullptr;
	SDL_Renderer *renderer    = nullptr;
	
	const char backgnd_imagefile[] = "street2.bmp";
	SDL_Surface* backgnd_surface = NULL;
	SDL_Texture* backgnd_texture = NULL;
	int car_c = 0;
	Uint32 rmask = 0, gmask = 0, bmask = 0, amask = 0;
	const char car_imagefile[] = "car.png";
	unsigned char* car_buffer = NULL;
	unsigned int car_width = 0, car_height = 0, lodepng_result = 0;
	SDL_Surface* car_surface = NULL;
	SDL_Texture* car_texture = NULL;
	SDL_Rect car_destination = { 0 };
	

	SDL_Init(SDL_INIT_VIDEO);
	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		cout << "Error : " << IMG_GetError() << endl;	
	
	window = SDL_CreateWindow("Patrol NBU Project ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
		
	renderer = SDL_CreateRenderer(window, -1, 0);// SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	
	// LOAD THE BACKGROUND BITMAP INTO A SURFACE & CONVERT IT TO A TEXTURE:
	// ... the surface is only temporary, but rendering requires a texture.
	if ((backgnd_surface = SDL_LoadBMP(backgnd_imagefile)) == NULL) {
		// Failure ...
		SDL_Log("Failed load background image (\"%s\") for \"%s\".\n", SDL_GetError(), backgnd_imagefile);
		goto exit_app;
	}
	else {
		// Convert process
		backgnd_texture = SDL_CreateTextureFromSurface(renderer, backgnd_surface);
		SDL_FreeSurface(backgnd_surface); // The back surface is no longer needed now that we have a texture for it.
		backgnd_surface = NULL;
		// Error check
		if (backgnd_texture == NULL) {
			// Failure ...
			SDL_Log("Failed to create texture from background surface (\"%s\").\n", SDL_GetError());
			goto exit_app;
		}
	}
	// PREPARE COLOR MASKS FOR LOGO SURFACE:
	if (SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &car_c, &rmask, &gmask, &bmask, &amask) == SDL_FALSE) {
		// Failure ...
		SDL_Log("Failed to determine color masks from SDL_PIXELFORMAT_ABGR8888 (\"%s\").\n", SDL_GetError());
		goto exit_app;
	}

	// LOAD LOGO PNG IMAGE INTO A BYTE BUFFER & ULTIMATELY CONVERT TO A TEXTURE FOR RENDERING:
	// ... this always decodes to 32-bit ABGR raw image.
	lodepng_result = lodepng_decode32_file(&car_buffer, &car_width, &car_height, car_imagefile);
	if (lodepng_result != 0) {
		// Failure ...
		SDL_Log("Failed lodepng_decode32_file() (\"%s\") for \"%s.\"\n", lodepng_error_text(lodepng_result), car_imagefile);
		goto exit_app;
	}
	else {
		
		
		if ((car_surface = SDL_CreateRGBSurfaceFrom((void*)car_buffer, car_width, car_height, car_c, car_width * 4, rmask, gmask, bmask, amask)) == 0) {
			// Failure ...
			free(car_buffer);
			SDL_Log("Failed to create surface for logo image buffer (\"%s\").\n", SDL_GetError());
			goto exit_app;
		}
		else {
			// Convert the car surface to a usable texture
			car_texture = SDL_CreateTextureFromSurface(renderer, car_surface);

			// free the surface and buffer
			SDL_FreeSurface(car_surface);
			free(car_buffer); 
			car_surface = NULL;
			car_buffer = NULL;

			//Error check
			if (car_texture == NULL) {
				SDL_Log("Failed to create texture from logo surface (\"%s\").\n", SDL_GetError());
				goto exit_app;
			}
		}
	}

	






	bool runs = true;
	SDL_Event action;

	while (runs)
	{
		while (SDL_PollEvent(&action) != 0)
		{
			if (action.type == SDL_QUIT)
				runs = false;
		}

		
		// Render the entire background to the entire screen:
		SDL_RenderCopy(renderer, backgnd_texture, NULL, NULL);

		// dimensions and positioning
		car_destination.x = 50;
		car_destination.y = 430;
		car_destination.w = 290;
		car_destination.h = 150;
		SDL_RenderCopy(renderer, car_texture, NULL, &car_destination);

		SDL_RenderPresent(renderer);
		
	}

	

exit_app:
	if (car_texture) SDL_DestroyTexture(car_texture);
	if (backgnd_texture) SDL_DestroyTexture(backgnd_texture);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	//test end

	SDL_Quit();
	
	return 0;


}