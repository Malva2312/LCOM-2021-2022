#include <minix/sysutil.h>
#include <lcom/utils.h>
#include <kbc.h>

int hook_id = 1;
bool makecode, full_code_ready=false, kbd_valid = false;
uint8_t data;
int size = 0;
uint8_t scanconde[2];

void (kbc_ih)() {
    if(kbd_get_status()) return;
    if(kbd_read_value()) return;
}
int (kbd_get_status)(){ //read the status register and check if there was some communications error
    uint8_t status;
    kbd_valid = false;
    util_sys_inb(KBC_ST_REG, &status);
    if(status & (KBC_PAR_ERR | KBC_TO_ERR | KBC_AUX)){
        kbd_valid = false;
        printf("error with parity or timeout or mouse");
        return 1;
    }
    if(status & KBC_OBF){ //to see if it is full (with data)
        kbd_valid = true;
        return 0;
    }
    return 1;
}

int (kbd_read_value)(){ //read the scancode byte from the out buffer
    if(full_code_ready){
        full_code_ready = false;
        size = 0;
    }
    if(util_sys_inb(KBC_OUT_BUF, &data)){
        printf("error reading buffer");
        return 1;
    }
    scanconde[size] = data;
    size++;
    if(size == 1 && (data == TWO_BYTE)){
        full_code_ready = false;
    } else{
        full_code_ready = true;
        makecode = ((data & 0x80) == 0x80) ? false : true;
    }
    return 0;
}
int kbc_write_cmd(uint8_t command){
    if(sys_outb(KBC_CMD_REG, KBC_WRITE_CMD)) return 1;
    if(sys_outb(KBC_IN_BUF, command)) return 1;
    return 0;
}
int kbc_read_cmd(uint8_t* command){
    if(sys_outb(KBC_CMD_REG, KBC_READ_CMD)) return 1;
    if(util_sys_inb(KBC_OUT_BUF, command)) return 1;
    return 0;
}
void kbc_restore_interrupts(){
    uint8_t command;
    kbc_read_cmd(&command);
    command = command | BIT(0);
    kbc_write_cmd(command);
}
int (kbd_subscribe_int)(uint8_t *bit_no){
    hook_id = *bit_no;
    return sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}
int (kbd_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id);
}
