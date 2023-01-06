#include "kbc.h"
#include <minix/sysutil.h>

#include "kbc.h"
#include <minix/sysutil.h>


int kbc_hook_id = 1;
uint8_t data = 0;
bool makecode, kbd_valid = false, full_code_ready = false;
int size = 0;
uint8_t scancode[2];

void (kbc_ih)(void) {
    if (kbd_get_status_byte()) return;

    if (kbc_read_value()) return;
}

int (kbd_get_status_byte)() {
    uint8_t status;
    kbd_valid = false;
    if (util_sys_inb(KBC_ST_REG, &status)) {
        printf("Error reading status register");
        return 1;
    }
    if (status & (KBC_PAR_ERR | KBC_TO_ERR | KBC_ST_AUX)) {
        kbd_valid = false;
        printf("ERROR parity, timeout or mouse");
        return 1;
    }
    if (status & KBC_ST_OBF) {
        kbd_valid = true;
        return 0;
    }
    return 1;
}

int (kbc_read_value)(){
    
    if(full_code_ready) {   //reset das variaveis
        full_code_ready = false;
        size = 0;
    }
    
    if(util_sys_inb(KBC_OUT_BUF, &data)){
        printf("Error reading out buffer");
        return 1;
    }
    if(!kbd_valid) return 1;

    scancode[size] = data;
    size++;
    if(size == 1 && (data == KBC_TWO_BYTE)) {   //quando ainda nao esta pronto
        full_code_ready = false;
    } else {    //quando ja esta pronto
        makecode = ((data & 0x80) == 0x80) ? false : true;
        full_code_ready = true;
    }
    return 0;

}

int (kbc_read_cmd)(uint8_t* cmd) {
    if (sys_outb(KBC_CMD_REG,KBC_READ_CMD)) return 1;
    if (util_sys_inb(KBC_OUT_BUF,cmd)) return 1;
    return 0;
}

int (kbc_write_cmd)(uint8_t cmd) {
    if(sys_outb(KBC_CMD_REG,KBC_WRITE_CMD)) return 1;
    if(sys_outb(KBC_ARG_CMD,cmd)) return 1;
    return 0;
}

void (kbc_restore_interrupts)() {
    //not my code
    printf("restoring keyboard \n");
    uint8_t command;
    kbc_read_cmd(&command);
    command |= BIT(0);
    kbc_write_cmd(command);
    printf("done \n");
}

int (kbd_subscribe_int)(uint8_t *bit_no) {
  kbc_hook_id = *bit_no;
  return sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbc_hook_id);
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbc_hook_id);
}

/*****************MOUSE*******************/


int mouse_hook_id = 2, mouse_packet_size = 0;
bool mouse_valid = false, mouse_packet_ready = false;
uint8_t mouse_packet[3];
struct packet pp;

int (mouse_subscribe_int)(uint8_t *bit_no){
  mouse_hook_id = *bit_no;
  return sys_irqsetpolicy(MOUSE_IRQ ,IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

int (enable_mouse)() {
  return sys_irqenable(&mouse_hook_id);
}

int (disable_mouse)() {
  return sys_irqdisable(&mouse_hook_id);
}

int (mouse_get_status_byte)(uint8_t *status) {
    mouse_valid = false;
    if (util_sys_inb(KBC_ST_REG, status)) {
        printf("Error reading status register.\n");
        return 1;
    }
    if (*status & (KBC_PAR_ERR | KBC_TO_ERR)) {
        mouse_valid = false;
        printf("ERROR parity, timeout.\n");
        return 1;
    }
    if (!(*status & KBC_ST_AUX) && (*status & KBC_ST_OBF)) {
        mouse_valid = false;
        printf("Error, data from keyboard.\n");
        return 0;
    }
    return 0;
}

void (mouse_ih)(){
    uint8_t status;
    if(mouse_get_status_byte(&status)) return;

    if (!(status & KBC_ST_OBF)) {
        mouse_valid = false;
        printf("No data to read from OBF.\n");
        return;
    }
    mouse_valid = true;

    if(mouse_read_value()) return;
}

int (mouse_read_value)(){

    if(mouse_packet_ready){
        mouse_packet_ready = false;
        mouse_packet_size = 0;
    }
    
    if(util_sys_inb(KBC_OUT_BUF, &data)){
        printf("Error reading out buffer");
        return 1;
    }

    if(mouse_packet_size == 0){
        if(!(data & MOUSE_ALWAYS_1)){
            printf("The bit(3) is not set to 1.\n");
            return 1;
        }
    }

    mouse_packet[mouse_packet_size] = data;
    mouse_packet_size++;

    if(mouse_packet_size == 3){
        mouse_packet_ready = true;
        parse_packet();
    }

    return 0;
}



int (parse_packet)(){
    pp.bytes[0] = mouse_packet[0];
    pp.bytes[1] = mouse_packet[1];
    pp.bytes[2] = mouse_packet[2];

    pp.x_ov = mouse_packet[0] & MOUSE_X_OVFL;
    pp.y_ov = mouse_packet[0] & MOUSE_Y_OVFL;
    pp.lb = mouse_packet[0] & MOUSE_LEFT_BUTT;
    pp.rb = mouse_packet[0] & MOUSE_RIGHT_BUTT;
    pp.mb = mouse_packet[0] & MOUSE_MIDDLE_BUTT;
    

    if(mouse_packet[0] & MOUSE_X_SIGN){
        pp.delta_x = mouse_packet[1] - 256;
    }
    else{
        pp.delta_x = mouse_packet[1];
    }

    if(mouse_packet[0] & MOUSE_Y_SIGN){
        pp.delta_y = mouse_packet[2] - 256;
    }
    else{
        pp.delta_y = mouse_packet[2];
    }
    return 0;
}

int (mouse_enbl_data_reporting)(void) {
    mouse_write_cmd(MCM_SET_STREAM_MODE);
    mouse_write_cmd(MCM_ENABLE_DATA_REP);
    return 0;
}

int (mouse_disable_data_reporting)() {
    mouse_write_cmd(MCM_SET_STREAM_MODE);
    mouse_write_cmd(MCM_DISABLE_DATA_REP);
    return 0;
}

int (mouse_write_to_port)(uint8_t port, uint8_t cmd) {
    uint8_t status;
    if(mouse_get_status_byte(&status)){
        return 1;
    }
    if(status & KBC_ST_IBF){
        //input buffer full
        return -1;
    }
    if (sys_outb(port, cmd)) {
        return 1;
    }
    return 0;
}

int (mouse_write_cmd)(uint8_t command){
    uint8_t response;
    int full;
    do{
        full = mouse_write_to_port(KBC_CMD_REG,WRITE_BYTE_TO_MOUSE);
        if (full == 1) return 1;
        if (full == -1) continue;

        full = mouse_write_to_port(KBC_ARG_CMD,command);
        if (full == 1) return 1;
        if (full == -1) continue;
        
        tickdelay(micros_to_ticks(DELAY_US));

        if (util_sys_inb(KBC_OUT_BUF, &response)) {
            return 1;
        }

        if(response == MCM_ERROR){
            return 1;
        }
    }while (response != MCM_ACK);

    return 0;
}
