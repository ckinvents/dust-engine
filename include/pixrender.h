#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#define WIDDERSHINS 1024
#define TURNWISE 896

typedef struct _PixBuffer {
    uint32_t* pixels;
    uint32_t width;
    uint32_t height;
} PixBuffer;

typedef struct _PixTex {
	uint32_t* pixData;
	uint32_t tileWidth;
	uint32_t tileHeight;
	uint8_t tileCount;
} PixTex;

typedef struct _PixSprite {
	PixTex* texture;
	uint8_t frameNum;
	double alphaNum;
	double scaleFactor;
	double x;
	double y;
} PixSprite;

// PixBuffer management
PixBuffer* PixBuffer_initPixBuffer(uint32_t width, uint32_t height);
void PixBuffer_delPixBuffer(PixBuffer* buffer);
// Drawable elements
void PixBuffer_drawColumn(PixBuffer* buffer, uint32_t x, int32_t y, int32_t h, SDL_Color color);
void PixBuffer_drawTexColumn(PixBuffer* buffer, uint32_t x, int32_t y, int32_t h, PixTex* texture, uint8_t tileNum, double alphaNum, uint32_t column, double fadePercent, SDL_Color targetColor);
void PixBuffer_drawRect(PixBuffer* buffer, SDL_Rect* rect, SDL_Color color);
void PixBuffer_drawHorizGradient(PixBuffer* buffer, SDL_Rect* rect, SDL_Color colTop, SDL_Color colBottom);
void PixBuffer_draw2DSprite(PixBuffer* buffer, PixSprite sprite, double angle);
// Full-buffer operations
void PixBuffer_fillBuffer(PixBuffer* target, uint32_t color, double alpha);
void PixBuffer_drawBuffOffset(PixBuffer* target, PixBuffer* source, uint32_t x, uint32_t y, int32_t xOff);
void PixBuffer_clearBuffer(PixBuffer* buffer);
// Filter effects
void PixBuffer_paletteFilter(PixBuffer* buffer, SDL_Color* palette, int paletteNum);
void PixBuffer_orderDither(PixBuffer* buffer, SDL_Color* palette, int paletteNum, double scaleFactor);
void PixBuffer_orderDither256(PixBuffer* buffer, double scaleFactor);
void PixBuffer_monochromeFilter(PixBuffer* buffer, SDL_Color targetColor, double fadePercent);
void PixBuffer_inverseFilter(PixBuffer* buffer);
// Pixel operations
uint32_t PixBuffer_toPixColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL_Color PixBuffer_toSDLColor(uint32_t pixColor);
uint32_t PixBuffer_blendAlpha(uint32_t baseColor, uint32_t addColor, double alphaNum);
uint32_t PixBuffer_getPix(PixBuffer* buffer, uint32_t x, uint32_t y);
uint32_t PixBuffer_getTex(PixTex* texture, uint8_t tileNum, uint32_t x, uint32_t y);
void PixBuffer_drawPix(PixBuffer* buffer, uint32_t x, uint32_t y, uint32_t color);
void PixBuffer_drawPixAlpha(PixBuffer* buffer, uint32_t x, uint32_t y, uint32_t color, double alphaNum);
void PixBuffer_drawPixDouble(PixBuffer* buffer, double x, double y, uint32_t color, double alphaNum);
// PixTex management
PixTex* PixTex_initFromRGBA(uint8_t* rgbaData, uint32_t tileWidth, uint32_t tileHeight, uint8_t numTiles);
void PixTex_delPixTex(PixTex* tex);
// PixSprite management
void PixSprite_initSprite(PixSprite* newSprite, PixTex* texture, double scaleFactor, double alphaNum, double x, double y);
#endif//RENDER_H
