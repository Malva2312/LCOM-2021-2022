#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t counter = 0;
int timer_hook_id;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if(((TIMER_FREQ / freq) < BIT(0)) || ((TIMER_FREQ / freq) >= BIT(16))){
    printf("Invalid frequency. \n");
    return 1;
  }

  //separar as duas partes da frequencia

  uint8_t lsb, msb;
  uint16_t newfreq = TIMER_FREQ / freq;   

  util_get_LSB(newfreq, &lsb);
  util_get_MSB(newfreq, &msb);


  //enviar a control word

  uint8_t st;
  timer_get_conf(timer, &st);

  st = (st & 0x0f) | TIMER_LSB_MSB;

  switch (timer)
  {
  case 0:
    st = st | TIMER_SEL0;
    break;
  case 1:
    st = st | TIMER_SEL1;
    break;
  case 2:
    st = st | TIMER_SEL2;
    break;
  default:
    printf("Invalid timer(timer_set_frequency)");
    return 1;
  }

  sys_outb(TIMER_CTRL,st);

  //enviar a nova frequencia
  sys_outb(GET_TIMER(timer),lsb);
  sys_outb(GET_TIMER(timer),msb);

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  timer_hook_id = *bit_no;
  return sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer < 0 || timer > 2) {
    printf("Invalid timer(timer_get_conf)");
    return 1;
  }
  
  uint8_t byte = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  sys_outb(TIMER_CTRL,byte);

  util_sys_inb(GET_TIMER(timer),st);

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
  union timer_status_field_val val;

  switch (field)
  {
  case tsf_all:
    val.byte = st;
    break;
  case tsf_mode:
    val.count_mode = (st&TIMER_GET_COUNTING_MODE)>>1;
    if(val.count_mode == 6 || val.count_mode == 7){
      val.count_mode -= 4;
    }
    break;
  case tsf_initial:
    switch (TIMER_LSB_MSB & st)
    {
    case TIMER_LSB:
      val.in_mode = LSB_only;
      break;
    case TIMER_MSB:
      val.in_mode = MSB_only;
      break;
    case TIMER_LSB_MSB:
      val.in_mode = MSB_after_LSB;
      break;
    default:
      val.in_mode = INVAL_val;
      break;
    }
    break;
  case tsf_base:
    val.bcd = (st&TIMER_BCD) == TIMER_BCD;
    break;
  default:
    return 1;
  }

  return timer_print_config(timer,field,val);; 
}
