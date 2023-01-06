#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

#define DELAY_US    20000

#define KBD_IRQ 0x01

#define ESC 0x81

#define TWO_BYTE 0xE0

#define KBC_ST_REG 0x64 // for reading the KBC state
#define KBC_CMD_REG 0x64 //write commands to the KBC
#define KBC_IN_BUF 0x60 // used for sending commands to the keyboard (KBD commands)
#define KBC_OUT_BUF 0x60 // used for receiving scancodes

//status register (to read it)

#define KBC_PAR_ERR BIT(7) //parity error
#define KBC_TO_ERR BIT(6) //timeout error
#define KBC_AUX BIT(5) //mouse data
#define KBC_INH BIT(4) //if 0, kbd is inhibited
#define KBC_IBF BIT(1) //input buffer full - dont write command
#define KBC_OBF BIT(0) //Output buffer full - data available for reading

//KBC commands
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60
#define KBC_CHECK 0xAA
#define CHECK_KB_INTERFACE  0xAB    //0 if OK
#define DISABLE_KBI 0xAD
#define ENABLE_KBI  0xAE

#define DISABLE_MOUSE 0xA7
#define ENABLE_MOUSE 0xA8
#define CHECK_MOUSE_INTERFACE 0xA9
#define WRITE_BYTE_TO_MOUSE 0xD4 //byte/args to KBC_IN_BUF

// --------------------MOUSE-----------------------------------

#define MOUSE_IRQ 12

#define MCM_DISABLE_DATA_REP    0xF5
#define MCM_ENABLE_DATA_REP     0xF4

#define MCM_NACK    0xFE    //if recieved retry sending the command (all steps from start)

#define MOUSE_Y_OVFL        BIT(7)
#define MOUSE_X_OVFL        BIT(6)
#define MOUSE_Y_SIGN        BIT(5)
#define MOUSE_X_SIGN        BIT(4)
#define MOUSE_ALWAYS_1      BIT(3)
#define MOUSE_MIDDLE_BUTT   BIT(2)
#define MOUSE_RIGHT_BUTT    BIT(1)
#define MOUSE_LEFT_BUTT     BIT(0)

#endif
