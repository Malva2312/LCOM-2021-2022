#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include <lcom/utils.h>
#include "serialport.h"

int ser_hook_id = 1;
uint8_t read_data = 0x00;
bool other_ready = false;

int (ser_init)() {
    if(sys_outb(SER_PORT + SER_IER, SER_ERBFI)) {
        printf("Error writing to IER from serial port\n");
        return 1;
    }    
    while (!ser_read_data()) printf("Read trash\n");
    
    ser_transmit_data(SER_INIT);
    return 0;
}

int (ser_read_lsr_status)(uint8_t* status) {
    if (util_sys_inb(SER_PORT + SER_LSR, status) != OK) {
        printf("Error reading LSR from serial port\n");
        return 1;
    }
    if (*status & (SER_OVERRUN_ERR | SER_PARITY_ERR | SER_FRAME_ERR)) {
        printf("Overrun, parity or framing error from serial port\n");
        return 1;
    }
    return 0;
}

int (ser_read_data)() {
    uint8_t status;
    ser_read_lsr_status(&status);

    if (!(status & SER_DATA_READY)){
        read_data = 0;
        printf("Data not ready!\n");
        return 1;
    }
    while (status & SER_DATA_READY) {
        if (util_sys_inb(SER_PORT + SER_RBR, &read_data)) {
            printf("Error reading recieved data from serial port!\n");
            return 1;    
        }
        printf("Read data: %d\n", read_data);
        if (read_data == SER_GOAL_1 || read_data == SER_GOAL_2) return 0;
        if (read_data == (SER_WINNER + 1) || read_data == (SER_WINNER + 2) || read_data == (SER_WINNER + 3)) return 0;
        ser_read_lsr_status(&status);
    }
    return 0;
}

int (ser_transmit_data)(uint8_t data) {
    uint8_t status;
    while (true) {
        ser_read_lsr_status(&status);
        if (status & (SER_UART_READY | SER_THR_EMPTY)){
            if (sys_outb(SER_PORT + SER_THR,data)) {
                printf("Error reading recieved data from serial port!\n");
                return 1;    
            }
            printf("Transmitted %d\n", data);
            return 0;
        }
        tickdelay(micros_to_ticks(5));
    }
}

int (ser_check_connection)() {
    if (read_data == SER_INIT){
        other_ready = true;
        ser_transmit_data(SER_START);
        return SER_INIT;
    }
    if (read_data == SER_START)
        return SER_START;
    return 0;
}

int (ser_subscribe_int)(uint8_t *bit_no) {
  ser_hook_id = *bit_no;
  return sys_irqsetpolicy(SER_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&ser_hook_id);
}

int (ser_unsubscribe_int)() {
  return sys_irqrmpolicy(&ser_hook_id);
}

