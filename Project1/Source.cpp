#include "Source.h"
#include "Car.h"

using namespace std;

void quit(Car* car, SDL_Renderer *renderer, SDL_Window *window, SDL_Texture* backgnd_texture) {

	car->destroyTexture();

	if (backgnd_texture) SDL_DestroyTexture(backgnd_texture);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	const char backgnd_imagefile[] = "street.bmp";
	SDL_Surface* backgnd_surface = NULL;
	SDL_Texture* backgnd_texture = NULL;
	SDL_Rect bg_src = { 0 };
	SDL_Rect bg_dest = { 0 };
	Uint32 rmask = 0, gmask = 0, bmask = 0, amask = 0;
	
	int bg_width = 5120;
	int window_width = 1280;
	int window_height = 720;

	SDL_Init(SDL_INIT_VIDEO);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		cout << "Error : " << IMG_GetError() << endl;

	window = SDL_CreateWindow("Patrol NBU Project ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, 0);// SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Car car(renderer);

	// LOAD THE BACKGROUND BITMAP INTO A SURFACE & CONVERT IT TO A TEXTURE:
	// ... the surface is only temporary, but rendering requires a texture.
	if ((backgnd_surface = SDL_LoadBMP(backgnd_imagefile)) == NULL) {
		// Failure ...
		SDL_Log("Failed load background image (\"%s\") for \"%s\".\n", SDL_GetError(), backgnd_imagefile);
		quit(&car, renderer, window, backgnd_texture); 
		return 0;
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
			quit(&car, renderer, window, backgnd_texture); 
			return 0;
		}
	}


	//background dest
	bg_dest.x = 0;
	bg_dest.y = 0;
	bg_dest.w = 5120;
	bg_dest.h = 720;
	

	if (car.prepareCarImage() == -1) {
		quit(&car, renderer, window, backgnd_texture);
		return 0;
	}
	
	bool isRunning = true;
	SDL_Event event;
	while (isRunning) {
		if ((bg_dest.x) <= -(window_width*4)) {
			bg_dest.x = 0;
		}
		
		//move bg
		// Render the entire background to the entire screen:
		if (bg_src.w == 0) {
			SDL_RenderCopy(renderer, backgnd_texture, NULL, &bg_dest);
		}
		else {
			SDL_RenderCopy(renderer, backgnd_texture, &bg_src, &bg_dest);
		}
				
		Sleep(100);
		//background scrolling
		bg_src = bg_dest;
		bg_dest.x -= 40;
	
		car.initialPosition();

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			else {
				switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT: car.slower(); break;
						case SDLK_a: car.slower(); break;
						case SDLK_RIGHT: car.faster(); break;
						case SDLK_d: car.faster(); break;
						case SDLK_UP: car.jump(); break;
						case SDLK_w:  car.jump();  break;
						default:break;
					}
					break;
				/*case SDL_KEYUP:
					switch (event.key.keysym.sym) {
					case SDLK_LEFT:if (car_xvel < 0)car_xvel = 0; break;
					case SDLK_a:if (car_xvel < 0)car_xvel = 0; break;
					case SDLK_RIGHT:if (car_xvel > 0)car_xvel = 0; break;
					case SDLK_d:if (car_xvel > 0)car_xvel = 0; break;
					case SDLK_UP:if (car_yvel < 0)car_yvel = 0; break;
					case SDLK_w:if (car_yvel < 0)car_yvel = 0; break;
					case SDLK_DOWN:if (car_yvel > 0)car_yvel = 0; break;
					case SDLK_s:if (car_yvel > 0)car_yvel = 0; break;
					default:break;
					}
					break;*/

				default:
					break;
				}
			}
		}

	}

	quit(&car, renderer, window, backgnd_texture);

	return 0;
}