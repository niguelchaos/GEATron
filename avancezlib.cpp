#include "avancezlib.h"
#include <iostream>
#include <SDL_ttf.h>
#include "SDL.h"
#include <stdio.h>
#include <string>
#include <cstdlib>

// Creates the main window. Returns true on success.
bool AvancezLib::init(int width, int height)
{
	SDL_Log("Initializing the engine...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}
	IMG_Init(IMG_INIT_JPG);
	IMG_Init(IMG_INIT_PNG);

	//Create window
	window = SDL_CreateWindow("aVANCEZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	TTF_Init();
	font = TTF_OpenFont("data/space_invaders.ttf", 12); //this opens a font style and sets a size
	if (font == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// initialize the keys
	key.fire = false;	key.left = false;	key.right = false, key.esc = false;
	key.up = false;		key.down = false;

	this->width = width;
	this->height = height;

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0, 0, 109, 0xFF);

	//Clear screen
	SDL_RenderClear(renderer);

	SDL_Log("Engine up and running...\n");
	return true;
}


// Destroys the avancez library instance
void AvancezLib::destroy()
{
	SDL_Log("Shutting down the engine\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);

	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}

void AvancezLib::quit() {
	destroy();
	exit(0);
}


void AvancezLib::processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				key.esc = true;
				break;
			case SDLK_SPACE:
				key.fire = true;
				break;
			case SDLK_LEFT:
				key.left = true;
				break;
			case SDLK_RIGHT:
				key.right = true;
				break;
			case SDLK_UP:
				key.up = true;
				//std::cout << "		^^		UP DETECTED" << std::endl;
				break;
			case SDLK_DOWN:
				key.down = true;
				//std::cout << "				DOWN DETECTED" << std::endl;
				break;

			case SDLK_w:
				key.w = true;
				break;
			case SDLK_a:
				key.a = true;
				break;
			case SDLK_s:
				key.s = true;
				break;
			case SDLK_d:
				key.d = true;
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				key.fire = false;
				break;
			case SDLK_LEFT:
				key.left = false;
				break;
			case SDLK_RIGHT:
				key.right = false;
				break;
			case SDLK_UP:
				key.up = false;
				break;
			case SDLK_DOWN:
				key.down = false;
				break;

			case SDLK_w:
				key.w = false;
				break;
			case SDLK_a:
				key.a = false;
				break;
			case SDLK_s:
				key.s = false;
				break;
			case SDLK_d:
				key.d = false;
				break;
			}
		}

	}
}

void AvancezLib::swapBuffers() {
	//Update screen
	SDL_RenderPresent(renderer);
}

void AvancezLib::clearWindow() {
	//Clear screen
	SDL_RenderClear(renderer);
}


Sprite * AvancezLib::createSprite(const char * path)
{
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (surf == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);

	Sprite * sprite = new Sprite(renderer, texture);

	return sprite;
}

void AvancezLib::drawText(int x, int y, const char * msg)
{
	SDL_Color black = { 0, 0, 0 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Surface* surf = TTF_RenderText_Solid(font, msg, black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf); //now you can convert it into a texture

	int w = 0;
	int h = 0;
	SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
	SDL_Rect dst_rect = { x, y, w, h };

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surf);
}
void AvancezLib::drawImage(const char* path, int minX, int minY, int maxX, int maxY)
{
	SDL_Rect rect;
	rect.x = minX;
	rect.y = minY;
	rect.w = maxX;
	rect.h = maxY;
	SDL_Surface* surf = IMG_Load(path);
	if (surf == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
	}

	//Create texture from surface pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
	}

	SDL_RenderCopy(renderer, texture, NULL, &rect);

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);
}

void AvancezLib::drawCell(Vector2D minPos, Vector2D maxPos) {


	SDL_Rect rect;

	rect = { (int)minPos.x, (int)minPos.y, (int)(maxPos.x - minPos.x), (int)(maxPos.y - minPos.y) };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}
\
void AvancezLib::fillRect(Vector2D minPos, Vector2D maxPos, int r, int g, int b) {
	SDL_Rect rect{};
	rect = { (int)minPos.x, (int)minPos.y, (int)(maxPos.x - minPos.x), (int)(maxPos.y - minPos.y) };

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

float AvancezLib::getElapsedTime()
{
	return SDL_GetTicks() / 1000.f;
}

int AvancezLib::getHeight() {
	return height;
}
int AvancezLib::getWidth() {
	return width;
}

void AvancezLib::getKeyStatus(KeyStatus & keys)
{
	keys.fire = key.fire;
	keys.left = key.left;
	keys.right = key.right;
	keys.up = key.up;
	keys.down = key.down;
	keys.esc = key.esc;

	keys.w = key.w;
	keys.a = key.a;
	keys.s = key.s;
	keys.d = key.d;
	 
}


Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture)
{
	this->renderer = renderer;
	this->texture = texture;
}


void Sprite::draw(int x, int y)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


void Sprite::destroy()
{
	SDL_DestroyTexture(texture);
}

