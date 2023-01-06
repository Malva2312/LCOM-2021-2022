#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <lcom/pixmap.h>

#include <stdbool.h>
#include <stdint.h>

#include "video/video.h"
#include "keyboard/kbc.h"
#include "game/handlers.h"
#include "game/images.h"
#include "game/view.h"
#include "serialport/serialport.h"
#include "rtc/rtc.h"

extern uint8_t scancode[2];
extern uint8_t data;
extern bool full_code_ready, kbd_valid;
extern bool mouse_valid, mouse_packet_ready;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(proj_main_loop)(int argc, char* argv[])
{
  /*initialize graphic mode*/
  
  map_video_memory(VIDEO_MODE);
  if(vg_mode(VIDEO_MODE) != OK){
    vg_exit();
    printf("Failed to go into graphic mode.\n");
    return 1;
  }

  int ipc_status;
  message msg;
  int r;

  uint8_t mouse_int_bit = 2;
  uint8_t timer_int_bit = 0;
  uint8_t kbc_int_bit = 1; 
  uint8_t rtc_int_bit = 8; 
  uint8_t ser_int_bit = 4; 

  if (kbd_subscribe_int(&kbc_int_bit) != OK){
    printf("Error subscribing to keyboard.\n");
    return 1;
  }

  if (timer_subscribe_int(&timer_int_bit) != OK){
    printf("Error subscribing to timer.\n");
    return 1;
  } 

  if (mouse_subscribe_int(&mouse_int_bit)) {
    printf("Error subscribing to kbc.\n");
    return 1;
  }
  if (disable_mouse()){
    printf("Error disabling mouse.\n");
    return 1;
  }
  if(mouse_enbl_data_reporting()){
    printf("Error enabiling data reporting on mouse.\n");
    return 1;
  }
  if (enable_mouse()){
    printf("Error enabling mouse.\n");
    return 1;
  }

  if (rtc_subscribe_int(&rtc_int_bit) != OK){
    printf("Error subscribing to RTC.\n");
    return 1;
  } 
  rtc_update_darkmode();

  if (ser_subscribe_int(&ser_int_bit) != OK){
    printf("Error subscribing to serial port.\n");
    return 1;
  } 
  //ser_init();

  load_all_images();

  drawMenu();
  refresh_buffer();

  while( data != KBC_ESC_KEY ) { //terminar quando o scancode é 0x81
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & BIT(kbc_int_bit)) { /* subscribed interrupt */
                      kbc_ih();
                      if (kbd_valid & full_code_ready) mainHandler(KEYBOARD);
                  }
                  
                  if (msg.m_notify.interrupts & BIT(timer_int_bit)) { /* subscribed interrupt */
                      timer_int_handler();
                      mainHandler(TIMER);
                  }

                  if (msg.m_notify.interrupts & BIT(mouse_int_bit)) { /* subscribed interrupt */
                      mouse_ih();
                      if(mouse_valid & mouse_packet_ready) mainHandler(MOUSE);
                  }

                  if (msg.m_notify.interrupts & BIT(ser_int_bit)) { /* subscribed interrupt */
                      ser_read_data();
                      mainHandler(SERIALPORT);
                  }

                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }
  printf("Finishing\n");
  while (!ser_read_data()) printf("Read trash\n");

  //back to text
  if(vg_exit() != OK){
    printf("Error exiting video.\n");
    return 1;
  }
  

  if(timer_unsubscribe_int() != OK){
    printf("Error unsubscribing KBC.\n");
    return 1;
  } 
  if (disable_mouse()){
      printf("Error disabling mouse.\n");
      return 1;
  }
  if(mouse_disable_data_reporting()){
      printf("Error disabiling data reporting on mouse.\n");
      return 1;
  }
  if (enable_mouse()){
      printf("Error disabling mouse.\n");
      return 1;
  }
  if (mouse_unsubscribe_int()){
    printf("Error unsubscribing mouse.\n");
    return 1;
  }



  if(kbd_unsubscribe_int() != OK){
    printf("Error unsubscribing KBC.\n");
    return 1;
  } 

  if(rtc_unsubscribe_int() != OK){
    printf("Error unsubscribing RTC.\n");
    return 1;
  } 

  if(ser_unsubscribe_int() != OK){
    printf("Error unsubscribing serial port.\n");
    return 1;
  } 
  return 0;
}
