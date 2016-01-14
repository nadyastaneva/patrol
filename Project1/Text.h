#include "Header.h"

class ScoreScreen
{
private:
	SDL_Renderer *renderer;
	unsigned int scoreFont;
	unsigned int textColor;
	SDL_Surface* scoreCount;
	SDL_Rect text_rect;

public:
	ScoreScreen(SDL_Renderer *renderer);
	~ScoreScreen();

	void Score();
	void applySurface();
	void destroyTexture();
}; 
