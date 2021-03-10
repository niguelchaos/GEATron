#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "vector2D.h"
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
class Sprite
{
	SDL_Renderer * renderer;
	SDL_Texture * texture;

public:

	Sprite(SDL_Renderer * renderer, SDL_Texture * texture);

	// Destroys the sprite instance
	void destroy();

	// Draw the sprite at the given position.
	// Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
	// (All sprites are 32*32 pixels, clipping is not supported)
	void draw(int x, int y);
};


class AvancezLib
{
	int width;
	int height;
public:
	// Destroys the avancez library instance
	void destroy();

	// Destroys the avancez library instance and exits
	void quit();

	// Creates the main window. Returns true on success.
	bool init(int width, int height);

	// Clears the screen and draws all sprites and texts which have been drawn
	// since the last update call.
	// If update returns false, the application should terminate.
	void processInput();

	void swapBuffers();

	void clearWindow();

	// Create a sprite given a string.
	// All sprites are 32*32 pixels.
	Sprite* createSprite(const char* name);

	// Draws the given text.
	void drawText(int x, int y, const char* msg);
	//draws grid cell
	void drawCell(Vector2D minPos, Vector2D maxPos);
	void fillRect(Vector2D minPos, Vector2D maxPos, int r, int g, int b);
	void LoadSound(const char* filename, Mix_Chunk* sound);
	void LoadMp3(const char* filename, Mix_Music* mp3sound);
	void PlaySound(int loop, int soundNum);
	void PlayMp3(int loop, int mp3Num);
	void CheckMp3IsPlaying();
	void CheckSoundIsPlaying();

	int isMp3Playing();
	void finishMp3();

	int AvancezLib::isSoundPlaying();
	void AvancezLib::finishSound();

	// loads and draws image
	void drawImage(const char* path, int minX, int minY, int maxX, int maxY);
	// Return the total time spent in the game, in seconds.
	float getElapsedTime();


	int getWidth();
	int getHeight();

	struct KeyStatus
	{
		bool fire; // space
		bool left; // left arrow
		bool right; // right arrow
		bool up; // up arrow
		bool down; // down arrow

		bool esc; // escape button
		bool r; // reset
		bool b; // background

		bool a; // left arrow
		bool d; // right arrow
		bool w; // up arrow
		bool s; // down arrow

	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);

private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	TTF_Font* font;
	//SDL_AudioSpec fmt;
	Mix_Chunk* revving = NULL;
	Mix_Chunk* speeding = NULL;
	Mix_Music* crash = NULL;
	Mix_Music* tictac = NULL;
	Mix_Music* erynoice = NULL;

	int channel = 0; 
	int mp3playing = 0;
	int soundPlaying = 0;
	KeyStatus key;
};

