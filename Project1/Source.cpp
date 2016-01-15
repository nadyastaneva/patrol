#include "Source.h"
#include "Car.h"
#include "Background.h"
#include "Hole.h"
#include "Rocks.h"
#include"Enemy.h"
#include <sstream>
#include "SDL_ttf.h"

using namespace std;

void quit(Car* car, SDL_Renderer *renderer, SDL_Window *window, Background* bg, Hole* hole, Rocks* rock, Enemy* enemy) {
	car->destroyTexture();
	bg->destroyTexture();
	hole->destroyTexture();
	rock->destroyTexture();
	enemy->destroyTexture();
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}

void showscore(SDL_Renderer *renderer, int window_width, int window_height) {
	SDL_RenderClear(renderer);

	std::string text = "Your Score: ";

	//Initialize SDL_ttf
	if (TTF_Init() != 0) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return;
	}
	TTF_Font* font = TTF_OpenFont("arial.ttf", 20);
	if (font == NULL) {
		printf("Unable to load font: %s \n", TTF_GetError());
	}
	else {
		SDL_Color textColor = { 250, 0, 0 };
		SDL_Surface* textsurfice = TTF_RenderText_Solid(font, text.c_str(), textColor);

		SDL_Rect text_rect;

		if (textsurfice == NULL)
		{
			return;
		}

		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textsurfice);


		//Get the texture w/h so we can center it in the screen

		SDL_QueryTexture(textTexture, NULL, NULL, &text_rect.w, &text_rect.h);
		text_rect.x = window_width / 2 - text_rect.w / 2;
		text_rect.y = window_height / 2 - text_rect.h / 2;
		SDL_RenderCopy(renderer, textTexture, NULL, &text_rect);
		SDL_RenderPresent(renderer);
		Sleep(4000);
		SDL_FreeSurface(textsurfice);
		TTF_CloseFont(font);
	}
}

int main(int argc, char* argv[])
{
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	int window_width = 1280;
	int window_height = 720;
	int bg_width = 5120;

	SDL_Init(SDL_INIT_VIDEO);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		cout << "Error : " << IMG_GetError() << endl;

	window = SDL_CreateWindow("Patrol NBU Project ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);// SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	 
	//create objects
	Background bg(renderer);
	Hole hole(renderer);
	Rocks rock(renderer);
	Car car(renderer);
	Enemy enemy(renderer);

	//init hole positions and counter
	int holeXs[3] = { 2000,3500,5000 };
	int holeCount = 0;

	// init rock positions and counter
	int rockXs[3] = { 2200,3800,5100 };
	int rockCount = 0;

	//prepare images
	if (bg.prepareBGImage() == -1) {
		quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
		return 0;
	}
	if (hole.prepareHoleImage() == -1) {
		quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
		return 0;
	}
	if (rock.prepareRockImage() == -1) {
		quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
		return 0;
	}
	if (car.prepareCarImage() == -1) {
		quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
		return 0;
	}
	if (enemy.prepareEnemyImage() == -1) {
		quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
		return 0;
	}


	bool isRunning = true;
	SDL_Event event;
	bool quitLoop = false;

	
	

	while (isRunning && !quitLoop) {

		int start = SDL_GetTicks();
		//load background on screen + loop
		bg.initialPosition();
		bg.updatePosition();

		//load hole image on screen
		if (hole.hole_dest.x <= -90 && holeCount < 3)
		{
			holeCount++;
			hole.hole_dest.x = holeXs[holeCount];
		}
		hole.initialPosition();
		hole.updatePosition();

		//load rock image
		if (rock.rock_dest.x <= -90 && rockCount < 3)
		{
			rockCount++;
			rock.rock_dest.x = rockXs[rockCount];
		}
		rock.initialPosition();
		rock.updatePosition();

		//load enemy image
		enemy.initialPosition();
		enemy.updatePosition();

		//load car object on screen + movement
		car.initialPosition();

		car.car_dest.y += car.gravity; //adds gravity to car object
		car.jumping = true;

		if (car.car_dest.y == 430) 
		{
			car.gravity = 0; //gravity doesn't affect while on ground
		}
		if (car.car_dest.y < 330)
		{
			car.gravity = 8;
			car.updateYPosition();
			car.jumping = false;
		}

		SDL_RenderPresent(renderer);

		Sleep(50);


		while (SDL_PollEvent(&event) != 0 && !quitLoop) {
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
					case SDLK_w: car.jump(); break;
					case SDLK_ESCAPE: quitLoop = true; break;
					default:break;
					}
					break;
				default:
					break;
				}
			}
		}

	}

	showscore(renderer, window_width, window_height);
	quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
	return 0;
}