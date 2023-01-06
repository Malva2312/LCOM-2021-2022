#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include "video.h"

static char *video_mem;
static char *video_buff;

static unsigned x_res;
static unsigned y_res;
static unsigned bytes_per_pixel;
static unsigned red_mask_size;
static unsigned red_field_pos;
static unsigned green_mask_size;
static unsigned green_field_pos;
static unsigned blue_mask_size;
static unsigned blue_field_pos;
static unsigned bytes_per_pixel;


unsigned(bits_to_byte_ceil)(unsigned bits_per_pixel){
  double bytes = bits_per_pixel / 8.0;
  return ceil(bytes); //need to do ceil because of the mode 0x110 which takes 15 bits per pixel
}

int(vg_mode)(uint16_t mode){
  //alterar a struct
  reg86_t r86;

  memset(&r86, 0, sizeof(r86));	/* zero the structure */

  r86.intno = VBE_INTNO_BIOS_VIDEO_SERVICES;
  r86.ah = VBE_AH_CALL_FUNCTION;
  r86.al = VBE_AL_SET_VBE_MODE;
  r86.bx = mode | VBE_BX_SET_MODE;

  //chamar a função sys_int86
  if( sys_int86(&r86) != OK ) {
      printf("sys_int86() failed.\n");
      return 1;
  }

  return 0;
}

int (map_video_memory) (uint16_t mode){
  struct minix_mem_range mr;
  unsigned int vram_base;  /* VRAM's physical addresss */
  unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
  int r;	

  vbe_mode_info_t mode_info;
  vbe_get_mode_info(mode, &mode_info);
  
  /*save the variables that are useful in static variables*/
  x_res = mode_info.XResolution;
  y_res = mode_info.YResolution;
  bytes_per_pixel = bits_to_byte_ceil(mode_info.BitsPerPixel);
  vram_base = mode_info.PhysBasePtr;
  vram_size = x_res * y_res * bytes_per_pixel;
  red_mask_size = mode_info.RedMaskSize;
  red_field_pos = mode_info.RedFieldPosition;
  green_mask_size = mode_info.GreenMaskSize;
  green_field_pos = mode_info.GreenFieldPosition;
  blue_mask_size = mode_info.BlueMaskSize;
  blue_field_pos = mode_info.BlueFieldPosition;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  video_buff = malloc(vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

void (refresh_buffer)() {
  memcpy((void*)video_mem, video_buff, x_res * y_res * bytes_per_pixel);
}

void (clear_buffer)(){
  memset(video_buff, 0, x_res*y_res*bytes_per_pixel);
}

int (vg_set_pixel) (uint32_t color, uint16_t x, uint16_t y){
  if(x < 0 || y < 0 || x > x_res || y > y_res){
    printf("Pixel out of the screen.\n");
    return 1;
  }

  void* pos = video_buff + x_res*bytes_per_pixel*y + x*bytes_per_pixel;
  memcpy(pos, &color, bytes_per_pixel);

  return 0;
}

int (vg_draw_vline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for (uint16_t i = 0; i < len; i++){
    vg_set_pixel(color, x, y+i);
  }
  return 0;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for (uint16_t i = 0; i < len; i++){
    vg_set_pixel(color, x+i, y);
  }
  return 0;
}

int (draw_rectangle)(uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height){

  for (uint16_t i = 0; i < height; i++){
    vg_draw_hline(x, y + i, width, color);
  }
  return 0;
}

uint32_t (color_mask)(uint32_t size, uint32_t pos){
  //(1 << size) - 1 = Number in binary composed of n(size) 1's 
  return ((1 << size) - 1) << pos;
}

uint32_t (direct_color_mode_RGB)(uint16_t row, uint16_t col, uint32_t first, uint8_t step){
  uint32_t r, g, b;
  uint32_t red_mask = color_mask(red_mask_size, red_field_pos);
  uint32_t green_mask = color_mask(green_mask_size, green_field_pos);
  uint32_t blue_mask = color_mask(blue_mask_size, blue_field_pos);

   r = ((((first & red_mask) >> red_field_pos) + col * step) % (1 << red_mask_size)) << red_field_pos;
   g = ((((first & green_mask) >> green_field_pos) + row * step) % (1 << green_mask_size)) << green_field_pos;
   b = ((((first & blue_mask) >> blue_field_pos) + (col + row) * step) % (1 << blue_mask_size)) << blue_field_pos;

  return r | g | b;
}

int (draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  unsigned int width = x_res / no_rectangles;
  unsigned height = y_res / no_rectangles;
  uint32_t color;

  for(unsigned int row = 0; row < no_rectangles; row++){
    for(unsigned int col = 0; col < no_rectangles; col++){
        //mode 0x105 is the only indexed mode, and is the only that takes only 1 byte per pixel
        if(bytes_per_pixel == 1){
          color = (first + (row * no_rectangles + col) * step) % (1 << bytes_per_pixel*8); //from point 5.3 on the sheet
        }
        else{
          color = direct_color_mode_RGB(row, col, first, step);
        }
        draw_rectangle(color, col*width, row*height, width, height);
    }
  }
  return 0;
}

int (draw_sprite)(uint16_t x, uint16_t y, xpm_image_t img){
    uint32_t *sprite = (uint32_t*)img.bytes;
    for (int row = 0; row < img.height; row++){
      for (int col = 0; col < img.width; col++){
        uint32_t color = sprite[img.width*row + col];
        if(color != TRANSPARENCY_COLOR_8_8_8_8){
          vg_set_pixel(sprite[img.width*row + col], col+x, row+y);
        }
    }
  }
  return 0;
}
