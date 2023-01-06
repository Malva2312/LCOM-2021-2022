// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "video.h"
#include <kbc.h>

extern uint8_t data;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) { //switch the video adapter to the graphics mode specified in its argumen
  //IMPLEMENTED
  map_video_memory(mode);
  //graphic mode
  if(vg_mode(mode)){
      vg_exit();
      printf("failed to go into graphic mode");
      return 1;
  }
  tickdelay(micros_to_ticks(delay * 1e6));

  //back to default text mode
  vg_exit();
  free_map_memory();

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  //IMPLEMENTED
  map_video_memory(mode);
  //graphic mode
  if(vg_mode(mode)){
      vg_exit();
      printf("failed to go into graphic mode");
      return 1;
  }
  // x and y specify the coordinates of the rectangle's top-left corner
  vg_draw_rectangle(x, y, width, height, color);

  int ipc_status;
  message msg;
  int r;

  uint8_t kbc_bit_no = 1;
  if (kbd_subscribe_int(&kbc_bit_no)) return 1;

  while (data != ESC)
    {
        if((r = driver_receive(ANY, &msg, &ipc_status))!=0){
            printf("error driver received");
            return 1;
        }
        if(is_ipc_notify(ipc_status)){
          switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE:
                if(msg.m_notify.interrupts & BIT(kbc_bit_no)){
                    kbc_ih();
                }
            break;
          }
        }
  }
  //back to text
  vg_exit();
  if(kbd_unsubscribe_int() != OK){
    printf("Error unsubscribing KBC.\n");
    return 1;
  } 
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  //IMPLEMENTED
  map_video_memory(mode);
  //change the video mode
  if(vg_mode(mode)){
      vg_exit();
      printf("failed to go into graphic mode");
      return 1;
  }
  //draw a pattern of colored (filled) rectangles on the screen
  draw_pattern(mode, no_rectangles, first, step);
  //upon receiving ESC, reset to text mode
   int ipc_status;
  message msg;
  int r;

  uint8_t kbc_bit_no = 1;
  if (kbd_subscribe_int(&kbc_bit_no)) return 1;

  while (data != ESC)
    {
        if((r = driver_receive(ANY, &msg, &ipc_status))!=0){
            printf("error driver received");
            return 1;
        }
        if(is_ipc_notify(ipc_status)){
          switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE:
                if(msg.m_notify.interrupts & BIT(kbc_bit_no)){
                    kbc_ih();
                }
            break;
          }
        }
  }
  //back to text
  vg_exit();
  if(kbd_unsubscribe_int() != OK){
    printf("Error unsubscribing KBC.\n");
    return 1;
  } 
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) { // draw a pixmap that is provided as an XPM image
  //

  uint16_t mode = 0x105;
  map_video_memory(mode);
  //change the video mode
  if(vg_mode(mode)){
      vg_exit();
      printf("failed to go into graphic mode");
      return 1;
  }

  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &img); //convert an XPM into a pixmap

  draw_sprite(x, y, img, sprite);
  //upon receiving ESC, reset to text mode
   int ipc_status;
  message msg;
  int r;

  uint8_t kbc_bit_no = 1;
  if (kbd_subscribe_int(&kbc_bit_no)) return 1;

  while (data != ESC)
    {
        if((r = driver_receive(ANY, &msg, &ipc_status))!=0){
            printf("error driver received");
            return 1;
        }
        if(is_ipc_notify(ipc_status)){
          switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE:
                if(msg.m_notify.interrupts & BIT(kbc_bit_no)){
                    kbc_ih();
                }
            break;
          }
        }
  }
  //back to text
  vg_exit();
  if(kbd_unsubscribe_int() != OK){
    printf("Error unsubscribing KBC.\n");
    return 1;
  } 
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
