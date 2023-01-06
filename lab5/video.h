#include <lcom/lcf.h>
#include <math.h>
#include "video_macros.h"

int (vg_mode)(uint16_t mode);
int (map_video_memory)(uint16_t mode);
int (free_map_memory)(void);
int vg_set_pixel(uint16_t x, uint16_t y,uint32_t color);
int vg_draw_line(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

uint32_t color_mask(uint32_t size, uint32_t pos);
uint32_t direct_color_mode_rgb(uint16_t row, uint16_t col, uint32_t first, uint8_t step);
int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);
int draw_sprite(uint16_t x,uint16_t y, xpm_image_t img,uint8_t *sprite);
