#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include "lcom/vbe.h"
#include "video.h"

static char *video_mem; // frame-buffer VM address  
static mmap_t mem_map;

static unsigned x_res;
static unsigned y_res;
static unsigned bytes_per_pixel;
static unsigned red_mask_size;
static unsigned red_field_pos;
static unsigned green_mask_size;
static unsigned green_field_pos;
static unsigned blue_mask_size;
static unsigned blue_field_pos;

int (map_video_memory)(uint16_t mode){
    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
    int r;				    
    
    vbe_mode_info_t mode_info;
    vbe_get_mode_info(mode, &mode_info);

    x_res = mode_info.XResolution;
    y_res = mode_info.YResolution;
    bytes_per_pixel = ceil(mode_info.BitsPerPixel/8);

    red_mask_size = mode_info.RedMaskSize;
    red_field_pos = mode_info.RedFieldPosition;
    green_mask_size = mode_info.GreenMaskSize;
    green_field_pos = mode_info.GreenFieldPosition;
    blue_mask_size = mode_info.BlueMaskSize;
    blue_field_pos = mode_info.BlueFieldPosition;
    
    vram_base = mode_info.PhysBasePtr;
    vram_size = x_res * y_res * bytes_per_pixel;

    lm_alloc(vram_size, &mem_map);
   
    /* Allow memory mapping */

    //guarda a base e limite da memória física
    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    //pede ao sistema acesso à memoria fisica
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    return 0;
}

int (vg_mode)(uint16_t mode){ //set mode
    reg86_t r86;
    memset(&r86, 0, sizeof(r86)); // zero the structure

    r86.intno = VBE_INT; //Interrupt number
    r86.ah = AH_CALL_FUN; //When invoking a VBE function, the AH register must be set to 0x4F
    r86.al = AL_SET_VBE_MODE;
    r86.bx = mode | BX_SET_MODE;

    if(sys_int86(&r86) != OK){
        printf("sys_int86() failed.\n");
        return 1;
    }
    return 0;
}


int (free_map_memory)(){
    lm_free(&mem_map);
    return 0;
}

int (vg_set_pixel) ( uint16_t x, uint16_t y, uint32_t color){
    if(x > x_res || y > y_res || x< 0 || y< 0){
		return 1;
	}
    //paint n bytes with the correspondent part of the color variable
    for (unsigned i = 0; i < bytes_per_pixel; i++)
    {
        //nao percebo oq esta acontecedo aqui!!!
        unsigned pos = x_res * bytes_per_pixel * y + x * bytes_per_pixel + i;
        video_mem[pos] = (uint8_t) GET_N_BYTE(color, i);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    //dentro da linha, vai desenhando o pixels por coluna
    for (uint16_t i = 0; i < len; i++)
    {
        vg_set_pixel(x+i,y, color);
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

    //vai fazendo draw line por linha
    for (uint16_t i = 0; i < height; i++)
    {
        vg_draw_hline(x,y+i, width, color);
    }
    return 0;
}
uint32_t (color_mask)(uint32_t size, uint32_t pos){
    return ((1 << size) -1) << pos;   
}

uint32_t (direct_color_mode_rgb)(uint16_t row, uint16_t col, uint32_t first, uint8_t step){
    uint32_t r,g,b;
    uint32_t red_mask = color_mask(red_mask_size, red_field_pos);
    uint32_t green_mask = color_mask(green_mask_size, green_field_pos);
    uint32_t blue_mask = color_mask(blue_mask_size, blue_field_pos);
    
    r = ((((red_mask & first) >> red_field_pos) + col * step) % (1 << red_mask_size)) << red_field_pos;

	g = ((((green_mask & first) >> green_field_pos) + row * step) % (1 << green_mask_size)) << green_field_pos;

	b = ((((blue_mask & first) >> blue_field_pos) + (col + row) * step) % (1 << red_mask_size)) << blue_field_pos;

    return r | g | b; 
}

int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
    unsigned int width = x_res/no_rectangles;
    unsigned int height = y_res/no_rectangles;
    uint32_t color;
    //NAO PERCEBO OQ SE PASSA AQUI
    for (unsigned int row = 0; row < no_rectangles; row++)
    {
        for (unsigned int col = 0; col < no_rectangles; col++)
        {
            switch (mode)
            {
            //mode 0x105 is the only indexed mode
            case 0x105:
                color = (first + (row * no_rectangles + col) * step) % (1 << bytes_per_pixel*8);
                break;
            
            default:
                color = direct_color_mode_rgb(row, col, first, step);
                break;
            }
            vg_draw_rectangle(col*width, row*height, width, height, color);
        }   
    }
    return 0;
}

int draw_sprite(uint16_t x,uint16_t y, xpm_image_t img,uint8_t *sprite){
    for (int row = 0; row < img.height; row++)
    {
        for (int col = 0; col < img.width; col++)
        {
            vg_set_pixel(x+col, y+row, sprite[img.width*row + col]);
        }
    }
    return 0;
}
