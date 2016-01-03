#include "Source.h"
#include "Car.h"
#include "Background.h"
#include "Hole.h"
#include "Rocks.h"


using namespace std;

void quit(Car* car, SDL_Renderer *renderer, SDL_Window *window, Background* bg, Hole* hole) {
	car->destroyTexture();
	bg->destroyTexture();
	hole->destroyTexture();
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	int window_width = 1280;
	int window_height = 720;
	int scrollingOffset = 0;
	int bg_width = 5120;
	int car_yvel = 0;
	int coord[5] = { 400, 600, 800, 1000, 1200 }; //array with x-coord for holes
	int coord_i = 0;

	

	SDL_Init(SDL_INIT_VIDEO);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		cout << "Error : " << IMG_GetError() << endl;

	window = SDL_CreateWindow("Patrol NBU Project ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);// SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//create background and car objects
	Background bg(renderer);
	Car car(renderer);
	Hole hole(renderer);
	Rocks rock(renderer);

	//prepare images
	if (car.prepareCarImage() == -1) {
		quit(&car, renderer, window, &bg, &hole);
		return 0;
	}
	if (bg.prepareBGImage() == -1) {
		quit(&car, renderer, window, &bg, &hole);
		return 0;
	}
	//prepare hole images
	if (hole.prepareHoleImage() == -1) {
		quit(&car, renderer, window, &bg, &hole);
		return 0;
	}
	//prepare rock images
	//...

	bool isRunning = true;
	SDL_Event event;
	int xpos = coord[coord_i]; //counter to go through the coord array
	while (isRunning) {


		//load background on screen + loop
		bg.initialPosition();
		bg.updatePosition();
		
		//load car object on screen + movement
		car.initialPosition();

		//load hole image on screen
		hole.initialPosition(xpos);
		hole.updatePosition();
		
		//load rock image
		//needs to scroll off-screen and then get destroyed when off-screen

		SDL_RenderPresent(renderer);
		Sleep(50);

		while (SDL_PollEvent(&event) != 0) {

			if (car.start_jump == 330)

			{

				car.start_jump = 430;

			}
			{

		}
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
					case SDLK_UP: car.jump();break;
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

	quit(&car, renderer, window, &bg, &hole);
	return 0;
}