#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <i8042.h>
#include <kbc.h>
#include <stdbool.h>
#include <stdint.h>	

extern int counter;
extern uint8_t data;
extern uint8_t scanconde[2];
extern int size;
extern bool makecode, kbd_valid, full_code_ready;
extern int cnt;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (kbd_test_scan)() {
    message msg;
    int ipc_status;
    int r;
    //subscribe kbc interrupts
    uint8_t bit_no = 1;
    if(kbd_subscribe_int(&bit_no)) return 1;

    while (data != ESC)
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
            if(msg.m_notify.interrupts & BIT(bit_no)){ //ve se o bit do irq esta ativo na interrupção
                kbc_ih();
                if(kbd_valid & full_code_ready) kbd_print_scancode(makecode, size, scanconde);
            }
            break;
         
         default:
             break;
         }
        
     }
     
    }
    kbd_unsubscribe_int();
    kbd_print_no_sysinb(cnt);
    return 0;
}

int (kbd_test_poll)() {
    while (data != ESC)
    {
        kbc_ih();
        if(kbd_valid&& full_code_ready){
            kbd_print_scancode(makecode, size, scanconde);
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    kbc_restore_interrupts();
    return 0;
}

int (kbd_test_timed_scan)(uint8_t idle) { // test whether if program is able to handle interrupts from more than one device
    /*your program should exit not only when the user releases the ESC key, 
  but also if it does not receive a scancode for a number of seconds equal to its argument, idle.*/
    uint8_t timer_bit_no = 0;
    if(timer_subscribe_int(&timer_bit_no)){
        printf("Error subscribing to timer 0");
        return 1;
    }
    uint8_t kbc_bit_no = 1;
    if(kbd_subscribe_int(&kbc_bit_no)){
        printf("Error subscribing to kbc");
        return 1;
    }

    int r;
    int ipc_status;
    message msg;
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
                if(msg.m_notify.interrupts & BIT(timer_bit_no)){
                    timer_int_handler();
                    if(counter/60 >= idle){
                        timer_unsubscribe_int();
                        kbd_unsubscribe_int();
                        return 0;
                    }
                }
                if(msg.m_notify.interrupts & BIT(kbc_bit_no)){
                    kbc_ih();
                    if(full_code_ready && kbd_valid) kbd_print_scancode(makecode, size, scanconde);
                    counter = 0;
                }
                break;
            
            default:
                break;
            }
        }
    }
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    return 0;
}
