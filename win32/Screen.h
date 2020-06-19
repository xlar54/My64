#pragma once
#include <SDL.h> 
#include <SDL_timer.h>
#include "..\types.h"

class Screen
{
private:
	SDL_Window* win = NULL;
	SDL_Renderer* rend = NULL;
	SDL_Surface* surface = NULL;
	SDL_Color* colors = NULL;

	int width = 320;
	int height = 200;

	UINT8_T charset[0x80] = {
		'@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
		'P','Q','R','S','T','U','V','W','X','Y','Z','[',' ',']','^','',
		' ','!','"','#','$','%','&','"','(',')','*','+',',','-','.','/',
		'0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',
		' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
		' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
		' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
		' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
	};

	void setColors(SDL_Color* colors);


public:
	Screen(void);
	void Write(UINT8_T value);
	void SetPixel(UINT16_T x, UINT8_T y, UINT8_T coloridx);
	void Text(UINT16_T pos, UINT8_T value);

};

