#include <minix/sysutil.h>
#include <lcom/utils.h>
#include <mouse.h>

int mouse_hook_id = 2;
bool mouse_packet_ready = false, mouse_valid = false;
struct packet pp;
int mouse_packet_size = 0;
uint8_t data, mouse_packet[3];


int (mouse_subscribe_int)(uint8_t *bit_no){
    mouse_hook_id = *bit_no;
    return sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}
int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

int enable_mouse(){
    return sys_irqenable(&mouse_hook_id);
}
int disable_mouse(){
    return sys_irqdisable(&mouse_hook_id);
}

void (mouse_ih)() {
    mouse_get_status_byte();
    mouse_get_value();
}

int mouse_get_status_byte(){
    uint8_t status;

    if(util_sys_inb(KBC_ST_REG, &status)){
        mouse_valid = false;
        return 1;
    }
        
    if(status & (KBC_PAR_ERR | KBC_TO_ERR)){
        mouse_valid = false;
        return 1;
    }
    if(!(status & KBC_AUX)){
        mouse_valid = false;
        return 1;
    }
    if(status & KBC_OBF){
        mouse_valid = true;
        return 0;
    }
    return 1;
}


int mouse_get_value(){
    if(mouse_packet_ready){
        mouse_packet_ready = false;
        mouse_packet_size = 0;
    }
    if(util_sys_inb(KBC_OUT_BUF, &data)){
        return 1;
    }
    mouse_packet[mouse_packet_size]=data;
    mouse_packet_size++;

    if(mouse_packet_size==3){
        mouse_packet_ready=true;
        parse_packet();
    }
    return 0;
}

int parse_packet(){
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
    } else{
        pp.delta_x = mouse_packet[1];
    }
    if(mouse_packet[0] & MOUSE_Y_SIGN){
        pp.delta_y = mouse_packet[2] - 256;
    } else{
        pp.delta_y = mouse_packet[2];
    }
    return 0;   
}

