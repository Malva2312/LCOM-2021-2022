#include <i8042.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>	
void (kbc_ih)();
int kbd_get_status();
int kbd_read_value();

int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe_int();

void kbc_restore_interrupts();
int kbc_read_cmd(uint8_t *command);
int kbc_write_cmd(uint8_t command);

