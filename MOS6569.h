#pragma once
#include <SDL.h> 
//#include <SDL_image.h> 
#include <SDL_timer.h>
#include "types.h"

class MOS6569
{
private:
	SDL_Window* win = NULL;
	SDL_Surface* surface = NULL;
	int width = 800;
	int height = 600;
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

public:
	MOS6569(void);
	void Write(UINT8_T value);
	void Text(UINT16_T pos, UINT8_T value);


};

