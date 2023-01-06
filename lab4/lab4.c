// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"

extern bool mouse_packet_ready;
extern struct packet pp;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  //enabling/disabling the mouse interface or enabling/disabling interrupt generation upon reception of a byte from the mous
    
    //"driver" must first write command 0xD4 to port 0x64 of KBC, and afterwards the byte (command or argument) to port 0x60
    // mouse will send back an acknowledgment in the output buffer, and it must be read from port 0x60
    //if the mouse command elicits a response, KBC will put in the output buffer; and should be read from port 0x60

    //if the byte the driver is expecting is the first one, and bit 3 of the byte received must be 1

    uint8_t mouse_bit_no = 2;
    if(mouse_subscribe_int(&mouse_bit_no)){
        printf("Error subscribing to kbc.\n");
        return 1;
    }
    if(disable_mouse()){
        printf("Error disabling mouse.\n");
        return 1;
    }
    if(mouse_enable_data_reporting()){ //do this in the project
        printf("Error enabiling data reporting on mouse.\n");
        return 1;
    }   
    if(enable_mouse()){
        printf("Error enabling mouse.\n");
        return 1;
    }
    message msg;
    int ipc_status;
    int r;

    while (cnt > 0)
    {
    /* Get a request message. */
    //driver receive
     if((r=driver_receive(ANY,&msg,&ipc_status)) != 0){
        printf("driver receive failed");
        continue;
     }
     if(is_ipc_notify(ipc_status)){ /* received notification */
         switch (_ENDPOINT_P(msg.m_source))
         {
         case HARDWARE: /* hardware interrupt notification */	
            if(msg.m_notify.interrupts & BIT(mouse_bit_no)){ //ve se o bit do irq esta ativo na interrupção
                mouse_ih();
                if(mouse_packet_ready){
                    mouse_print_packet(&pp);
                    cnt--;
                }
            }
            break;
         
         default:
             break;
         }
     }
    }

  if (disable_mouse()){
      printf("Error disabling mouse.\n");
      return 1;
  }

  //TODO: WRITE COMMAND TO DISABLE MOUSE

  if (enable_mouse()){
      printf("Error disabling mouse.\n");
      return 1;
  }

  if (mouse_unsubscribe_int()){
    printf("Error unsubscribing mouse.\n");
    return 1;
  }


  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
