#include <lcom/lcf.h>
#include <math.h>
#include "video_macros.h"

/**
 * @brief Initialize a given graphic mode
 * 
 * @param mode Mode of operation
 * @return 0 if OK, 1 otherwise
 */
int (vg_mode)(uint16_t mode);
/**
 * @brief Bits to bytes (ceiling aproximation)
 * 
 * @param bits_per_pixel Number of bits per pixel
 * @return Number of bytes per pixel
 */
unsigned(bits_to_byte_ceil)(unsigned bits_per_pixel);
/**
 * @brief Create a virtual adress space for video memory
 * 
 * @param mode Mode of operation
 * @return 0 if OK, 1 otherwise
 */
int (map_video_memory) (uint16_t mode);
/**
 * @brief Copy the secondary buffer contents to the main buffer
 * 
 */
void (refresh_buffer)();
/**
 * @brief Clear the secondary buffer
 * 
 */
void (clear_buffer)();
/**
 * @brief Set the color of a pixel
 * 
 * @param color Color of the pixel
 * @param x X position
 * @param y Y position
 * @return 0 if OK, 1 otherwise
 */
int (vg_set_pixel) (uint32_t color, uint16_t x, uint16_t y);
/**
 * @brief Draw a vertical line of pixels
 * 
 * @param x X position
 * @param y Y position
 * @param len Length of the line
 * @param color Color of the line
 * @return 0 if OK, 1 otherwise
 */
int (vg_draw_vline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draw a horizontal line of pixels
 * 
 * @param x X position
 * @param y Y position
 * @param len Length of the line
 * @param color Color of the line
 * @return 0 if OK, 1 otherwise
 */
int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draw a rectangle
 * 
 * @param color Color of the pixel
 * @param x X position
 * @param y Y position
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @return 0 if OK, 1 otherwise
 */
int (draw_rectangle)(uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
/**
 * @brief Draw a pattern of different colored rectangles
 * 
 * @param no_rectangles Number of rectangles per side
 * @param first Color of first rectangle
 * @param step Step for next rectangle color
 * @return 0 if OK, 1 otherwise
 */
int (draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
/**
 * @brief Draw a sprite
 * 
 * @param x X position
 * @param y Y position
 * @param img Xpm of the image
 */
int (draw_sprite)(uint16_t x, uint16_t y, xpm_image_t img);
