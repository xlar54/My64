#include "MOS6569.h"
#include <iostream>
#include "windows.h"

MOS6569::MOS6569(void)
{
    // retutns zero on success else non-zero 
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        
        std::cout << "error initializing SDL: %s\n" << SDL_GetError();
    }
    else {
        win = SDL_CreateWindow("My64", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

        // triggers the program that controls 
        // your graphics hardware and sets flags 
        Uint32 render_flags = SDL_RENDERER_ACCELERATED;

        // creates a renderer to render our images 
        SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    
        SDL_RenderClear(rend);
        SDL_Rect fillRect = { 0, 0, width, height };
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(rend, &fillRect);
        SDL_RenderPresent(rend);
    
        surface = SDL_GetWindowSurface(win);
    }
}

void MOS6569::Write(UINT8_T value)
{
    if (value < 0x80)
        std::cout << charset[value];
    else
        std::cout << charset[value - 0x80];
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void MOS6569::Text(UINT16_T pos, UINT8_T value)
{
    gotoxy((pos % 40) + 1, (pos / 40) + 1);
    
    if (value < 0x80)
        std::cout << charset[value];
    else
        std::cout << charset[value - 0x80];
}

