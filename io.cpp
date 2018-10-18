#define SDL_MAIN_HANDLED
#include "stdio.h"
#include "stdlib.h"
#include "SDL2/sdl.h"

/****************************************
* Branch maintained by: Dane Christensen
****************************************/

/******************************************************
 * DEFINES:
 *  -Macros for universal variables/hook-ups
 *****************************************************/
#define GAME_NAME   "Pipeline"
#define S_WIDTH     512
#define S_HEIGHT    512
#define POINTER_2D(name)        Uint32 (*name)[S_WIDTH]
#define SETUP_POINTER_2D(name)  (Uint32 (*)[S_WIDTH])name->pixels
#define COLOR                   Uint32    
#define MAX_VERTICES 6
#define ABS(in) (in > 0 ? (in) : -(in))

/******************************************************
 * Types of primitives our pipeline will render.
 *****************************************************/
enum PRIMITIVES 
{
    TRIANGLE,
    LINE,
    POINT
};

/*****************************************************
 * Types of view matrices for clipping, transforms.
 ****************************************************/
enum VIEW_MATRICES
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

/****************************************************
 * Describes a two-dimensional point. 
 ****************************************************/
struct Vertex
{
    int x;
    int y;
};

/*******************************************************
 * Describes vertex-specific values for primitives.
 ******************************************************/
struct Attributes
{
    COLOR color;
};

/******************************************************
 * Your Description goes here.
 *****************************************************/
struct Transform
{
    // Your code goes here
};


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
void PollUserInputs(bool & playing)
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

/****************************************
 * Renders a triangle to the screen with 
 * the appropriate fill pattern
 ***************************************/
void DrawTriangle(POINTER_2D(framePtr), Vertex* triangle, Attributes* attrs, int count)
{
    // Your code goes here 
}

/****************************************
 * Renders a line to the screen with the
 * appropriate coloring
 ***************************************/
void DrawLine(POINTER_2D(framePtr), Vertex* line, Attributes* attrs, int count)
{
    // Error check
    if(count != 2) return;

    // Swap so it is left-to right
    if(line[0].x > line[1].x)
    {
        Vertex tmp = line[0];
        line[0] = line[1];
        line[1] = tmp;
    }

    // Special case horizontal lines
    int xCurs = line[0].x;
    int yCurs = line[0].y;
    if(line[0].y == line[1].y)
    {
        while(xCurs != line[1].x)
        {
            framePtr[yCurs][xCurs++] = attrs[0].color;
        }
        return;
    }

    // Special case vertical lines
    if(line[0].x == line[1].x)
    {
        int yInc = (line[0].y < line[1].y) ? 1 : -1;
        while(xCurs != line[1].x)
        {
            framePtr[yCurs][xCurs] = attrs[0].color;
            yCurs += yInc;
        }
        return;
    }

    // Bresenham members
    int yDelta = ABS(line[1].y - line[0].y);
    int xDelta = ABS(line[1].x - line[0].x);

    // X vs Y Dominant members 
    int* majorPtr, *minorPtr;
    int majorEnd, incMinor, incMajor, DeltaMinorX2, DeltaMinorX2MinusMajorX2, errTerm;    
    if(xDelta > yDelta)
    {
        majorPtr = &xCurs;
        minorPtr = &yCurs;
        majorEnd = line[1].x;
        incMinor = (line[1].y > line[0].y) ? 1 : -1;
        incMajor = 1;
        DeltaMinorX2 = (yDelta * 2);
        DeltaMinorX2MinusMajorX2 = DeltaMinorX2 - (xDelta * 2);
        errTerm = DeltaMinorX2 - xDelta;
    }
    // Y-Dominant run
    else
    {
        majorPtr = &yCurs;
        minorPtr = &xCurs;
        majorEnd = line[1].y;
        incMinor = 1;
        incMajor = (line[1].y > line[0].y) ? 1 : -1;
        DeltaMinorX2 = (xDelta * 2);
        DeltaMinorX2MinusMajorX2 = DeltaMinorX2 - (yDelta * 2);
        errTerm = DeltaMinorX2 - yDelta;
    }

    framePtr[yCurs][xCurs] = attrs[0].color;
    while(*majorPtr != majorEnd)
    {
        if(errTerm >= 0)
        {            
            *minorPtr += incMinor;
            errTerm += DeltaMinorX2MinusMajorX2;
        }
        else
        {
            errTerm += DeltaMinorX2;
        }
        *majorPtr += incMajor;        
        framePtr[yCurs][xCurs] = attrs[0].color;
    }
}

/****************************************
 * Renders a point to the screen with the
 * appropriate coloring
 ***************************************/
void DrawPoint(POINTER_2D(framePtr), Vertex* v, Attributes* attrs, int count)
{
    if(count == 0) return;

    // Set our pixel according to the attribute value!
    framePtr[v->y][v->x] = attrs->color;
}


/*************************************************************
 * TRANSFORM_VERTICES:
 * Where rotations, translations, scaling operations
 * transform the input vertices. NOTE: This does not 
 * include the camera view transform.
 ************************************************************/
void TransformVertices( Vertex inputVerts[], Attributes inputAttrs[], int numInput, 
                        Vertex transVerts[], Attributes transAttrs[], int & numTrans, 
                        Transform* trans)
{
    // Dummy code - your code will replace this
    for(numTrans = 0; numTrans < numInput; numTrans++)
    {
        transVerts[numTrans] = inputVerts[numTrans];
        transAttrs[numTrans] = inputAttrs[numTrans];
    }
}

/*************************************************************
 * CLIP_VERTICES:
 * Depending on our view type - clip to the frustrum that 
 * maps to our screen's aspect ratio.  
 ************************************************************/
void ClipVertices(Vertex transVerts[], Attributes transAttrs[], int numTrans, 
                  Vertex clippedVerts[], Attributes clippedAttrs[], int & numClipped, 
                  VIEW_MATRICES view)
{
    // Dummy code - your code will replace this
    for(numClipped = 0; numClipped < numTrans; numClipped++)
    {
        clippedVerts[numClipped] = transVerts[numClipped];
        clippedAttrs[numClipped] = transAttrs[numClipped];
    }
}

/*************************************************************
 * VIEW_TRANSFORM_VERTICES:
 * Converts our clipped geometry to screen space. 
 * This usually means using PERSPECTIVE or ORTHOGRAPHIC 
 * views.
 ************************************************************/
void ViewTransformVertices( Vertex clippedVerts[], Attributes clippedAttrs[], int numClipped, 
                            Vertex viewVerts[], Attributes viewAttrs[], int & numView, 
                            VIEW_MATRICES view)
{
    // Dummy code - your code will replace this
    for(numView = 0; numView < numClipped; numView++)
    {
        viewVerts[numView] = clippedVerts[numView];
        viewAttrs[numView] = clippedAttrs[numView];
    }
}

/***************************************************************************
 * Processes the indiecated PRIMITIVES type through stages of:
 *  1) Transformation
 *  2) Clipping
 *  3) Perspective or Orthographic projection
 *  4) Vertex Interpolation
 *  5) Fragment Shading
 **************************************************************************/
void DrawPrimitive(PRIMITIVES type, 
                    Vertex inputVerts[], 
                    Attributes inputAttrs[],
                    int numIn, 
                    POINTER_2D(framePtr), 
                    Transform * transFormMatrix = NULL, 
                    VIEW_MATRICES view_m = PERSPECTIVE)
{
    // Matrix Transformations
    Vertex transVerts[MAX_VERTICES];
    Attributes transAttrs[MAX_VERTICES];
    int numTrans;    
    TransformVertices(inputVerts, inputAttrs, numIn, transVerts, transAttrs, numTrans, transFormMatrix); 

    // Clip to our frustrum
    Vertex clippedVerts[MAX_VERTICES];
    Attributes clippedAttrs[MAX_VERTICES];
    int numClipped; 
    ClipVertices(transVerts, transAttrs, numTrans, clippedVerts, clippedAttrs, numClipped, view_m);

    // View space transform
    Vertex viewVerts[MAX_VERTICES];
    Attributes viewAttrs[MAX_VERTICES];
    int numView;
    ViewTransformVertices(clippedVerts, clippedAttrs, numClipped, viewVerts, viewAttrs, numView, view_m);

    // Vertex Interpolation & Fragment Drawing
    switch(type)
    {
        case POINT:
            DrawPoint(framePtr, viewVerts, viewAttrs, numView);
            break;
        case LINE:
            DrawLine(framePtr, viewVerts, viewAttrs, numView);
            break;
        case TRIANGLE:
            DrawTriangle(framePtr, viewVerts, viewAttrs, numView);
            break;
    }
}

/***********************************************
 * Sets the screen to the indicated color value.
 **********************************************/
void ClearScreen(POINTER_2D(framePtr), COLOR color = 0xff000000)
{
    for(int y = 0; y < S_HEIGHT; y++)
    {
        for(int x = 0; x < S_WIDTH; x++)
        {
            framePtr[y][x] = color;
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

/************************************************************
 * UPDATE_SCREEN:
 * Blits pixels from RAM to VRAM for rendering
 ***********************************************************/
void SendFrame(SDL_Texture* GPU_OUTPUT, SDL_Renderer * ren, SDL_Surface* frameBuf) 
{
    SDL_UpdateTexture(GPU_OUTPUT, NULL, frameBuf->pixels, frameBuf->pitch);
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, GPU_OUTPUT, NULL, NULL);
    SDL_RenderPresent(ren);
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
        PollUserInputs(running);

        // Refresh Screen
        ClearScreen(framePtr);

        // Draw a point on the screen 
        Vertex v[1];
        Attributes a[1];
        v[0].x = 10;
        v[0].y = 10;
        a[0].color = 0xffff0000;
        DrawPrimitive(POINT, v, a, 1, framePtr);

        // Sample lines 
        DrawLineTest(framePtr);

        // Ensure framerate at 60fps, push to screen
        SDL_Delay(17);	  
        SendFrame(GPU_OUTPUT, REN, FRAME_BUF);
    }

    // Cleanup
    SDL_FreeSurface(FRAME_BUF);
    SDL_DestroyTexture(GPU_OUTPUT);
    SDL_DestroyRenderer(REN);
    SDL_DestroyWindow(WIN);
    SDL_Quit();
    return 0;
}
