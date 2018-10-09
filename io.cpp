#define SDL_MAIN_HANDLED
#include "stdio.h"
#include "stdlib.h"
#include "SDL2/sdl.h"

/******************************************************
 * DEFINES:
 *  -Macros for universal variables/hook-ups
 *****************************************************/
#define GAME_NAME   "Pipeline"
#define S_WIDTH     512
#define S_HEIGHT    512
#define POINTER_2D(name)        Uint32 (*name)[S_WIDTH]
#define SETUP_POINTER_2D(name)  (Uint32 (*)[S_WIDTH])name->pixels

/************************************************************
 * UPDATE_SCREEN:
 * Blits pixels from RAM to VRAM for rendering
 ***********************************************************/
void sendFrame(SDL_Texture* GPU_OUTPUT, SDL_Renderer * ren, SDL_Surface* frameBuf) 
{
    SDL_UpdateTexture(GPU_OUTPUT, NULL, frameBuf->pixels, frameBuf->pitch);
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, GPU_OUTPUT, NULL, NULL);
    SDL_RenderPresent(ren);
}

/*************************************************************
 * POLL_CONTROLS:
 * Updates the state of the application based on:
 * keyboard, mouse, touch screen, gamepad inputs 
 ************************************************************/
void pollUserInputs(bool & playing)
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) 
    {
        if(e.type == SDL_QUIT) 
        {
            playing = false;
        }
        if(e.key.keysym.sym == 'q' && e.type == SDL_KEYDOWN) 
        {
            playing = false;
        }
    }
}

/********************************************
 * Demonstrate Line Drawing for Project 02. 
 *******************************************/
void DrawLineTest(POINTER_2D(framePtr))
{
        Vertex vv[2];
        Attributes aa[2];
        aa[0].color = 0xffff0000;
        aa[0].color = 0xffff0000;
        vv[0].x = 300;
        vv[0].y = 300;
        vv[1].x = 500;
        vv[1].y = 350;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 350;
        vv[1].y = 500;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 250;
        vv[1].y = 100;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 100;
        vv[1].y = 250;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 500;
        vv[1].y = 250;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 100;
        vv[1].y = 250;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 250;
        vv[1].y = 500;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 350;
        vv[1].y = 100;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[1].x = 100;
        vv[1].y = 350;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        // Diagonals
        vv[0].x = 500;
        vv[0].y = 100;
        vv[1].x = 100;
        vv[1].y = 500;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[0].x = 100;
        vv[0].y = 100;
        vv[1].x = 500;
        vv[1].y = 500;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        // Straights
        vv[0].x = 100;
        vv[0].y = 300;
        vv[1].x = 500;
        vv[1].y = 300;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

        vv[0].x = 300;
        vv[0].y = 100;
        vv[1].x = 300;
        vv[1].y = 500;
        DrawPrimitive(LINE, vv, aa, 2, framePtr);

}


/*************************************************************
 * MAIN:
 * Main game loop, initialization, memory management
 ************************************************************/
int main()
{
    // -----------------------DATA TYPES----------------------
    SDL_Window* WIN;                // Our Window
    SDL_Renderer* REN;              // Interfaces CPU with GPU
    SDL_Texture * GPU_OUTPUT;       // GPU buffer image (GPU Memory)
    SDL_Surface* FRAME_BUF;         // CPU buffer image (Main Memory) 
    POINTER_2D(framePtr);           // Assists with setting pixels in FRAME_BUF

    // ------------------------INITIALIZATION-------------------
    SDL_Init(SDL_INIT_EVERYTHING);
    WIN = SDL_CreateWindow(GAME_NAME, 200, 200, S_WIDTH, S_HEIGHT, 0);
    REN = SDL_CreateRenderer(WIN, -1, SDL_RENDERER_SOFTWARE);
    FRAME_BUF = SDL_CreateRGBSurface(0, S_WIDTH, S_HEIGHT, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    FRAME_BUF = SDL_ConvertSurface(SDL_GetWindowSurface(WIN), SDL_GetWindowSurface(WIN)->format, 0);
    GPU_OUTPUT = SDL_CreateTextureFromSurface(REN, FRAME_BUF);
    framePtr = SETUP_POINTER_2D(FRAME_BUF);

    // Draw loop 
    bool running = true;
    while(running) 
    {           
        // Poll for user inputs
        pollUserInputs(running);

        // Draw Screen

        // Sample lines 
        DrawLineTest(framePtr);
		
        // Ensure framerate at 60fps, push to screen
        SDL_Delay(17);	  
        sendFrame(GPU_OUTPUT, REN, FRAME_BUF);
    }

    // Cleanup
    SDL_FreeSurface(FRAME_BUF);
    SDL_DestroyTexture(GPU_OUTPUT);
    SDL_DestroyRenderer(REN);
    SDL_DestroyWindow(WIN);
    SDL_Quit();
    return 0;
}
