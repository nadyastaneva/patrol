
#include "SDL.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
	SDL_Window *window = nullptr;
	SDL_Surface *windowSurface = nullptr;
	SDL_Surface *imageSurface = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		cout << "Video Initialization Error: " << SDL_GetError() << endl;
	else {
		window = SDL_CreateWindow("SDL Coding Is F**HARD ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
		if (window == NULL)
			cout << "Window Creation Error : " << SDL_GetError() << endl;
		else {
			windowSurface = SDL_GetWindowSurface(window);
			imageSurface = SDL_LoadBMP("street2.bmp");
			if (imageSurface == NULL)
				cout << "Image loading Error: " << SDL_GetError() << endl;
			else {
				SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
				SDL_UpdateWindowSurface(window);
				SDL_Delay(4500);
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	//cout << "dfgd";
	return 0;


}