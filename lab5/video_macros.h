#ifndef _LCOM_VIDEO_MACROS_H_
#define _LCOM_VIDEO_MACROS_H_

#include <lcom/lcf.h>

/** @defgroup video_macros video_macros
 * @{
 *
 * Constants for programming the video card in graphic mode.
 */

#define VBE_INT 0x10

//call function (in real mode) - AH
#define AH_CALL_FUN 0x4f //When invoking a VBE function, the AH register must be set to 0x4F

//Unsuccessful call return codes - AH
#define AH_FUN_SUCCESS 0x00
#define AH_FUN_CALL_FAILED 0x01
#define AH_FUN_NOT_SUPPORTED 0x02
#define AH_FUN_INVALID_MODE 0x03

//VBE functions - AL
#define AL_SET_VBE_MODE 0x02
#define AL_RETURN_VBE_MODE_INFO 0x01
#define AL_RETURN_VBE_CONTROLLER_INFO 0x02

//Setting the Graphics Mode
#define VBE_RES_1024x768 0x105
#define VBE_RES_640x480 0x110
#define VBE_RES_800x600 0x115
#define VBE_RES_1280x1024 0x11A
#define VBE_RES_1152x864 0x14C


//BX for mode - BIT 14 of the BX register must be set
#define BX_SET_MODE BIT(14)

#define GET_N_BYTE(w, n) ((w >> (8*n)) & 0xFF)

#endif /*_LCOM_VIDEO_MACROS_H_ */
