#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard and Mouse and KBC.
 */

#define DELAY_US    20000 /**< @brief Delay between commands*/

#define KBC_IRQ     1 /**< @brief KBC interrupt line*/
#define MOUSE_IRQ   12 /**< @brief mouse interrupt line*/

#define KBC_TWO_BYTE 0xE0 /**< @brief First byte of a two byte key*/

#define KBC_ESC_KEY 0x81 /**< @brief ESC key scancode*/

#define KBC_OUT_BUF 0x60 /**< @brief Output buffer of kbc*/
#define KBC_ARG_CMD 0x60 /**< @brief Argumment commands for kbc*/
#define KBC_ST_REG  0x64 /**< @brief Status register of kbc*/
#define KBC_CMD_REG 0x64 /**< @brief Command register of kbc*/


//  Status register
#define KBC_PAR_ERR     BIT(7)
#define KBC_TO_ERR      BIT(6)
#define KBC_ST_AUX      BIT(5)
#define KBC_ST_INH      BIT(4)
#define KBC_ST_IBF      BIT(1)
#define KBC_ST_OBF      BIT(0)


//  Keyboard-Related KBC Commands
/*  
    Written using KBC_CMD_REG
    Arguments are passed using 0x60 (KBC_ARG_CMD)
    Return values are passed in the KBC_OUT_BUF
*/
#define KBC_READ_CMD        0x20
#define KBC_WRITE_CMD       0x60
#define CHECK_KBC           0xAA    //0x55 if OK, 0xFC if error
#define CHECK_KB_INTERFACE  0xAB    //0 if OK
#define DISABLE_KBI         0xAD
#define ENABLE_KBI          0xAE

//  Mouse related KBC Commands
#define DISABLE_MOUSE           0xA7
#define ENABLE_MOUSE            0xA8
#define CHECK_MOUSE_INTERFACE   0xA9    //0 if OK
#define WRITE_BYTE_TO_MOUSE     0xD4    //byte/args to KBC_ARG_CMD


//  KBC "Command Byte"
    /*  
        First, issue a command (KBC_RCB / KBC_RCB) to KBC_CMD_REG
        It's value must be read/written to 0x60 (KBC_OUT_BUF)
    */
#define KBC_DIS_MOUSE       BIT(5)
#define KBC_DIS_KBI         BIT(4)
#define KBC_ENA_OBF_MOUSE   BIT(1)
#define KBC_ENA_OBF_KB      BIT(0)

// Scancodes for keyboard keys
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_ENTER 0x1C
#define KEY_W   0x11
#define KEY_A   0x1e
#define KEY_S   0x1F
#define KEY_D   0x20

//********* MOUSE ***************

// Data from mouse
#define MOUSE_Y_OVFL        BIT(7)
#define MOUSE_X_OVFL        BIT(6)
#define MOUSE_Y_SIGN        BIT(5)
#define MOUSE_X_SIGN        BIT(4)
#define MOUSE_ALWAYS_1      BIT(3)  //MISSING NAME
#define MOUSE_MIDDLE_BUTT   BIT(2)
#define MOUSE_RIGHT_BUTT    BIT(1)
#define MOUSE_LEFT_BUTT     BIT(0)

//Arguments to the mouse commands
    /*
    Sent as args after a new call to command 0xD4 (WRITE_BYTE_TO_MOUSE)
    */
#define MCM_RESET               0xFF
#define MCM_RESEND              0xFE
#define MCM_SET_DEFAULTS        0xF6
#define MCM_DISABLE_DATA_REP    0xF5
#define MCM_ENABLE_DATA_REP     0xF4
#define MCM_SET_SMPL_RATE       0xF3
#define MCM_SET_REM_MODE        0xF0
#define MCM_READ_DATA           0xEB
#define MCM_SET_STREAM_MODE     0xEA
#define MCM_STATUS_REQUEST      0xE9
#define MCM_SET_RESOLUTION      0xE8
#define MCM_SET_SCAL_2          0xE7
#define MCM_SET_SCAL_1          0xE6

// acknowledgement is read from KBC_OUT_BUF within 25ms
// (for multi-byte responses, the time between bytes should be lower than 20 ms)

#define MCM_ACK     0xFA    /**< @brief Response when everything is OK*/
#define MCM_NACK    0xFE    /**< @brief First invalid byte*/
#define MCM_ERROR   0xFC    /**< @brief Second consecutive invalid byte*/


#endif
