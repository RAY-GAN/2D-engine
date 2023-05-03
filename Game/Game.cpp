#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game()
{  
	isRunning = false;
	Logger::Log("Game constructor called!");
}


Game::~Game()
{
	Logger::Log("Game destructor called!");
}


void Game::Initialize()
{	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error Initializing SDL.");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = 2560;//displayMode.w;
	windowHeight = 1440;//displayMode.h;

	//create SDL window
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);

	//check if window created successfully
	if (!window)
	{
		Logger::Err("Error Creating SDL window.");
		return;
	}

	//creater SDL renderer for the corresponding window 
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	//check if renderer created successfully
	if (!renderer)
	{
		Logger::Err("Error Creating SDL renderer.");
		return;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;

}



void Game::Setup()
{
	
	//Entity tank = Registry.CreateEntity();
	//tank.AddComponent<TransformComponent>();
	//tank.AddComponent<BoxColliderComponent>();
	//tank.AddComponent<SpriteComponent>("./assets/images/tank.png");
}


void Game::Run()
{
	Setup();
	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
			return;
		case SDL_KEYDOWN:
			if(sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			return;
		}
	}
}

void Game::Update()
{
	// If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
	int timeToWait = MILLISECSS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECSS_PER_FRAME)
	{
		SDL_Delay(timeToWait);
	}
	
	//The difference in ticks since the last frame in seconds
	double deltaTime = (double(SDL_GetTicks())- double(millisecsPreviousFrame)) / 1000.0;

	//Store the current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	//MovementSystem.Update();
	//CollisionSystem.Update();
	//DamageSystem.Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}
