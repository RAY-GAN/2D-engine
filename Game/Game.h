#ifndef GAME_H
#define GAME_H

#include <SDL.h>

const int FPS = 60;
const int MILLISECSS_PER_FRAME = 1000 / FPS;

class Game 
{

private:

	bool isRunning;
	int millisecsPreviousFrame = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:

	Game();

	void Initialize();

	void Run();

	void Setup();

	void ProcessInput();

	void Update();

	void Render();

	void Destroy();

	~Game();

	int windowWidth;
	int windowHeight;


};


#endif

