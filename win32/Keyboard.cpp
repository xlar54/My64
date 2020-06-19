#include "Keyboard.h"

UINT8_T Keyboard::Getkey(void)
{
	static bool shiftMode;
	SDL_Event e;

	SDL_PollEvent(&e);

	if (e.type == SDL_KEYDOWN && shiftMode==false)
	{

		if (e.key.keysym.sym > 47 && e.key.keysym.sym < 58)
			return e.key.keysym.sym;

		if (e.key.keysym.sym > 96 && e.key.keysym.sym < 123)
			return e.key.keysym.sym - 32;

		switch (e.key.keysym.sym)
		{
			case SDLK_RETURN: return 13;
			case SDLK_SPACE: return 32;
			case SDLK_SEMICOLON: return ';';
			case SDLK_MINUS: return '-';
			case SDLK_SLASH: return '/';
			case SDLK_QUOTE: return 39;
			case SDLK_EQUALS: return '=';
			case SDLK_BACKSPACE: return 20;
			case SDLK_COMMA: return ',';
			case SDLK_PERIOD: return '.';
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
			{
				shiftMode = true;
				return 0;
			}

			default:
				return 0;
		}
	}

	if (e.type == SDL_KEYDOWN && shiftMode == true)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_1: return '!';
			case SDLK_2: return '@';
			case SDLK_3: return '#';
			case SDLK_4: return '$';
			case SDLK_5: return '%';
			case SDLK_6: return '^';
			case SDLK_7: return '&';
			case SDLK_8: return '*';
			case SDLK_9: return '(';
			case SDLK_0: return ')';
			case SDLK_SLASH: return '?';
			case SDLK_QUOTE: return '"';
			case SDLK_EQUALS: return '+';
			case SDLK_SEMICOLON: return ':';
			case SDLK_COMMA: return '<';
			case SDLK_PERIOD: return '>';
			default:
				return 0;
		}
	}

	if (e.type == SDL_KEYUP && shiftMode == true && (e.key.keysym.sym == SDLK_RSHIFT || e.key.keysym.sym == SDLK_LSHIFT))
	{
		shiftMode = false;
	}

	return 0;
}
