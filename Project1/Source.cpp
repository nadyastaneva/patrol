#include "Header.h"

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

	// initialize SDL_ttf
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

		// Get the texture w/h so we can center it in the screen

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

	



	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		cout << "Error : " << IMG_GetError() << endl;

	window = SDL_CreateWindow("Patrol NBU Project ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);// SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Music & Sound Effects

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		cout << "Error :" << Mix_GetError << endl;

	Mix_Music *background = Mix_LoadMUS(" "); //for more than 10 secs
	Mix_Chunk *jump = Mix_LoadWAV("jump.wav");
	Mix_Chunk *explosion = Mix_LoadWAV("explosion.wav");
	

	// hole positions and counter
	int holeXs[10] = { 2123, 2820, 3305, 4023, 4512, 5621, 6302, 6834, 8000, 8544 };
	int holeCount = -1;

	// rock positions and counter
	int rockXs[13] = { 1352, 1889, 2588, 3043, 3267, 3823, 4190, 5031, 5321, 6104, 7221, 7713, 8302 };
	int rockCount = -1;

	// enemy positions and counter
	int enemyXs[4] = { 2532, 3471, 4179, 5721 };
	int enemyCount = -1;

	// create objects
	Background bg(renderer);
	Hole hole(renderer);
	Rocks rock(renderer);
	Car car(renderer);
	Enemy enemy(renderer);

	// prepare images
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

	SDL_Event event;
	bool isRunning = true;
	bool quitLoop = false;

	// game start
	while (isRunning && !quitLoop) {

		int start = SDL_GetTicks();
		// load background on screen
		bg.initialPosition();
		bg.updatePosition();

		// load hole image on screen
		if (hole.hole_dest.x <= -90 && holeCount < 11)
		{
			holeCount++;
			hole.hole_dest.x = holeXs[holeCount];
		}
		hole.initialPosition();
		hole.updatePosition();

		// load rock image
		if (rock.rock_dest.x <= -90 && rockCount < 14)
		{
			rockCount++;
			rock.rock_dest.x = rockXs[rockCount];
		}
		rock.initialPosition();
		rock.updatePosition();

		// load enemy image
		if (enemy.enemy_dest.x <= -90 && enemyCount < 14)
		{
			enemyCount++;
			enemy.enemy_dest.x = enemyXs[enemyCount];
		}
		enemy.initialPosition();
		enemy.updatePosition();

		// load car object on screen
		car.initialPosition();

		car.car_dest.y += car.gravity; // adds gravity to car object
		car.jumping = true;

		if (car.car_dest.y == car.groundPos)
		{
			car.gravity = 0; // gravity doesn't affect while on ground
		}
		if (car.car_dest.y < car.jumpHeight)
		{
			car.gravity = 8; // gravity returns the car to the ground
			car.updateYPosition();
			car.jumping = false;
		}

		// collision check for holes
		if ((car.car_dest.y + car.car_dest.h) >= hole.hole_dest.y + 10) // collision on Y-axis
		{
			if (car.car_dest.x <= hole.hole_dest.x && (car.car_dest.x + car.car_dest.w) >= hole.hole_dest.x + 10) // collision on X-axis - front/top
			{   
				Mix_PlayChannel(-1, explosion, 0);
				isRunning = false;
			}
			else if (car.car_dest.x <= (hole.hole_dest.x + hole.hole_dest.w) && (car.car_dest.x + car.car_dest.w) >= (hole.hole_dest.x + hole.hole_dest.w)) // collision on X-axis - back
			{
				Mix_PlayChannel(-1, explosion, 0);
				isRunning = false;
			}
		}
		// collision check for rocks
		if ((car.car_dest.y + car.car_dest.h) >= rock.rock_dest.y) // collision on Y-axis
		{
			if (car.car_dest.x <= rock.rock_dest.x && (car.car_dest.x + car.car_dest.w) >= rock.rock_dest.x + 10) // collision on X-axis - front/top
			{
				Mix_PlayChannel(-1, explosion, 0);
				isRunning = false;
			}
			else if (car.car_dest.x <= (rock.rock_dest.x + rock.rock_dest.w - 5) && (car.car_dest.x + car.car_dest.w) >= (rock.rock_dest.x + rock.rock_dest.w - 5)) // collision on X-axis - back
			{
				Mix_PlayChannel(-1, explosion, 0);
				isRunning = false;
			}
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
					case SDLK_UP: car.jump(); 
						
						Mix_PlayChannel(-1, jump, 0);
						break;
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


	Mix_Quit();
	Mix_FreeChunk(jump);
	//Mix_FreeMusic();


	showscore(renderer, window_width, window_height);
	quit(&car, renderer, window, &bg, &hole, &rock, &enemy);
	return 0;
}