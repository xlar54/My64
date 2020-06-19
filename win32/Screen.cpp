#include "Screen.h"
#include <iostream>
#include "windows.h"

Screen::Screen(void)
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
        rend = SDL_CreateRenderer(win, -1, render_flags);

        SDL_RenderClear(rend);
        SDL_Rect fillRect = { 0, 0, width, height };
        SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(rend, &fillRect);
        SDL_RenderPresent(rend);

        colors = ((SDL_Color*)malloc(sizeof(SDL_Color) * 16));

        setColors(colors);

        surface = SDL_GetWindowSurface(win);

        SDL_SetPaletteColors(surface->format->palette, colors, 0, 16);

    }
}

void Screen::setColors(SDL_Color* colors)
{
    colors[0].r = 0x00; colors[0].g = 0x00; colors[0].b = 0x00; colors[0].a = 0xff;
    colors[1].r = 0xff; colors[1].g = 0xff; colors[1].b = 0xff; colors[1].a = 0xff;
    colors[2].r = 0x88; colors[2].g = 0x00; colors[2].b = 0x00; colors[2].a = 0xff;
    colors[3].r = 0xaa; colors[3].g = 0xff; colors[3].b = 0xee; colors[3].a = 0xff;
    colors[4].r = 0xcc; colors[4].g = 0x44; colors[4].b = 0xcc; colors[4].a = 0xff;
    colors[5].r = 0x00; colors[5].g = 0xcc; colors[5].b = 0x55; colors[5].a = 0xff;
    colors[6].r = 0x00; colors[6].g = 0x00; colors[6].b = 0xaa; colors[6].a = 0xff;
    colors[7].r = 0xee; colors[7].g = 0xee; colors[7].b = 0x77; colors[7].a = 0xff;
    colors[8].r = 0xdd; colors[8].g = 0x88; colors[8].b = 0x55; colors[8].a = 0xff;
    colors[9].r = 0x66; colors[9].g = 0x44; colors[9].b = 0x00; colors[9].a = 0xff;
    colors[10].r = 0xff; colors[10].g = 0x77; colors[10].b = 0x77; colors[10].a = 0xff;
    colors[11].r = 0x33; colors[11].g = 0x33; colors[11].b = 0x33; colors[11].a = 0xff;
    colors[12].r = 0x77; colors[12].g = 0x77; colors[12].b = 0x77; colors[12].a = 0xff;
    colors[13].r = 0xaa; colors[13].g = 0xff; colors[13].b = 0x66; colors[13].a = 0xff;
    colors[14].r = 0x00; colors[14].g = 0x88; colors[14].b = 0xff; colors[14].a = 0xff;
    colors[15].r = 0xbb; colors[15].g = 0xbb; colors[15].b = 0xbb; colors[15].a = 0xff;
}

void Screen::SetPixel(UINT16_T x, UINT8_T y, UINT8_T coloridx)
{
    SDL_SetRenderDrawColor(rend, colors[coloridx].r, colors[coloridx].g, colors[coloridx].b, colors[coloridx].a);
    SDL_RenderDrawPoint(rend, x, y);
}

void Screen::Write(UINT8_T value)
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

void Screen::Text(UINT16_T pos, UINT8_T value)
{
    gotoxy((pos % 40) + 1, (pos / 40) + 1);

    if (value < 0x80)
        std::cout << charset[value];
    else
        std::cout << charset[value - 0x80];
}
