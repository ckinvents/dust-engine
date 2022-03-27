#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pixrender.h"
#include "misc_util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "sandbox_test.h"
#include "dustrender.h"
#include <time.h>

#define SCALE 8
const unsigned int WIDTH = WIDDERSHINS/SCALE;
const unsigned int HEIGHT = TURNWISE/SCALE;

// Test renderer
SDL_Renderer* renderer = NULL;
SDL_Texture* drawTex = NULL;
SDL_Window* window = NULL;
SDL_Event event;

SDL_Color BLACK = {0,0,0,255};



int main(int argc, char* argv[])
{
    srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	const uint8_t* keys = SDL_GetKeyboardState(NULL);

	window = SDL_CreateWindow(
		"DustEngine Demo Test Experiment Example Practice (WIP) Thingy",
		30, 30,
		WIDTH*SCALE, HEIGHT*SCALE,
		SDL_WINDOW_OPENGL
	);

    // SDL renderer initialization
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetScale(renderer, SCALE, SCALE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Render target initialization
	drawTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_NONE);

	PixBuffer* screen = PixBuffer_initPixBuffer(WIDTH, HEIGHT);

    // Dust engine init
    DustType* test_type = Test_createConfig(WIDTH, HEIGHT);
    DustWorld* world = DustWorld_createNew(WIDTH, HEIGHT);
    DustWorld_init(world, test_type);

    int quit = FALSE;
    SDL_Event sdl_event;
    int running = 0;
    unsigned int dustType = 0;
    unsigned int button;
    unsigned int test_col_default = world->types[SAND].shadeFunction(world->dust, D_OFFSET_NO) | 0xFF;
    DustPos test = {0, 0};
	while(!quit)
	{
        unsigned int test_col = test_col_default;
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT ||
                (sdl_event.type == SDL_KEYDOWN &&
                    sdl_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
            {
                quit = TRUE;
            }
            else if (sdl_event.type == SDL_KEYDOWN &&
                        sdl_event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                running = !running;
            }
            // Dust selection test code
            else if (sdl_event.type == SDL_MOUSEWHEEL && sdl_event.wheel.y > 0 ||
                sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                dustType++;
            }
            else if (sdl_event.type == SDL_MOUSEWHEEL && sdl_event.wheel.y < 0 ||
                sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.scancode == SDL_SCANCODE_LEFT)
            {
                dustType--;
            }
            dustType %= NUM_TYPES;
            test_col_default = world->types[dustType].shadeFunction(world->dust, D_OFFSET_NO) | 0xFF;
        }
        if (running == 1)
        {
            DustWorld_update(world);
        }
        // Cursor test code
        button = SDL_GetMouseState(&test.x, &test.y);
        test.x /= SCALE;
        test.y /= SCALE;
        if (button & SDL_BUTTON_LMASK)
        {
            test_col = 0xff0000ff;
            DustWorld_draw(world, dustType, test);
        }
        else if (button & SDL_BUTTON_RMASK)
        {
            test_col = 0xff0000ff;
            DustWorld_erase(world, test);
        }

        PixBuffer_fillBuffer(screen, 0x331133FF, 1.0);//0x24081CFF, 1.0);
        DustRender_renderWorld(screen, world, 0, 0);
        PixBuffer_drawPix(screen, test.x, test.y, test_col);
        PixBuffer_orderDither256(screen, 0);
        SDL_UpdateTexture(drawTex, NULL, screen->pixels, WIDTH * sizeof(int32_t));
        SDL_RenderCopy(renderer, drawTex, NULL, NULL);
        SDL_RenderPresent(renderer);
	}
    SDL_Rect rect;

	// Clean up and quit
    DustWorld_destroy(world);
    Test_destroyConfig(test_type, HEIGHT);
	PixBuffer_delPixBuffer(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	SDL_Quit();
	return 0;
}
