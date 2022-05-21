#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "graphics.h"

#define BASE_ADDR 0x00800000
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
  volatile uint32_t *ctrl;
  volatile uint32_t *vram;
} BitMap;

typedef struct {
  uint16_t width;
  uint16_t height;
} Screen;

static BitMap planes[] = {
  { .ctrl = VKY3_B_BMAP_LAYER0_CTRL, .vram = VKY3_B_BMAP_LAYER0_VRAM },
  { .ctrl = VKY3_B_BMAP_LAYER1_CTRL, .vram = VKY3_B_BMAP_LAYER1_VRAM }
};

static Screen screen = { .width=320, .height=200 };

static uint32_t * color_lut[] = {
  VKY3_B_COLOR_LUT0,
  VKY3_B_COLOR_LUT1,
  VKY3_B_COLOR_LUT2,
  VKY3_B_COLOR_LUT3,
  VKY3_B_COLOR_LUT4,
  VKY3_B_COLOR_LUT5,
  VKY3_B_COLOR_LUT6,
  VKY3_B_COLOR_LUT7,
};

// SETCOLOR <lut>, <color>, <red>, <green>, <blue>
void gfx_setcolor(uint8_t lut, uint8_t color, uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t *color_ptr = color_lut[lut] + color;
  *color_ptr = 255 << 3 | red << 2 | green << 1 | blue;
}

void gfx_mode(uint32_t mode) {
  *VKY3_B_MCR = mode;
}

// `BITMAP <plane>, <visible>, <lut> [, <address>]`
void gfx_bitmap(uint8_t plane, uint8_t visible, uint8_t lut, uint32_t address) {
  *(planes[plane].vram) = BASE_ADDR - address;
  *(planes[plane].ctrl) = lut << 1 | visible;
}

void gfx_clrbitmap(uint8_t plane) {

}


void gfx_plot(uint8_t plane, int16_t x, int16_t y, uint8_t color) {
  //if (x >= 0 && x < screen.width && y >= 0 && y < screen.height) {
    uint8_t *loc = (BASE_ADDR + (*planes[plane].vram) + (screen.width * y) + x);
    *loc = color;
  //}
}

void gfx_fill(uint8_t plane, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
  int16_t width = abs(x2-x1);
  int16_t height = abs(y2-y1);
  int16_t x = MIN(x1, x2);
  int16_t y = MIN(y1, y2);

  uint8_t *loc = 0;
  for (int i=0; i<height; i++) {
    memset(loc, color, width);
    loc += screen.width;
  }
}

void gfx_line(uint32_t plane, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  uint32_t dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
  uint32_t dy = abs(y2-y1), sy = y1<y2 ? 1 : -1; 
  uint32_t err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    gfx_plot(plane, x1, y1, color);
    if (x1==x2 && y1==y2) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x1 += sx; }
    if (e2 < dy) { err += dx; y1 += sy; }
  }	
}