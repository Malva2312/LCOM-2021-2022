#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>	
#include "i8042.h"

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
int enable_mouse();
int disable_mouse();

void (mouse_ih)();
int mouse_get_status_byte();

int mouse_get_value();
int parse_packet();
