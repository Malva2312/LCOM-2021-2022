#ifndef _LCOM_VIDEO_MACROS_H_
#define _LCOM_VIDEO_MACROS_H_

#include <lcom/lcf.h>

/** @defgroup video_macros video_macros
 * @{
 *
 * Constants for programming the video card in graphic mode.
 */

#define VIDEO_MODE 0x115

//Call functions - AH
#define VBE_AH_CALL_FUNCTION 0x4F

//VBE functions - AL
#define VBE_AL_SET_VBE_MODE 0x02
#define VBE_AL_RETURN_VBE_MODE_INFO 0x01
#define VBE_AL_RETURN_VBE_CONTROLLER_INFO 0x02

//(Un)successful call return codes -AH
#define VBE_AH_FUN_SUCCESS 0x00
#define VBE_AH_FUN_CALL_FAILED 0x01
#define VBE_AH_FUN_NOT_SUPPORTED 0x02
#define VBE_AH_FUN_INVALID_MODE 0x03

//INTNO
#define VBE_INTNO_BIOS_VIDEO_SERVICES 0x10

//Graphic modes - BX in SET VBE MODE, CX in RETRUN MODE INFO
#define VBE_RES_1024x768 0x105
#define VBE_RES_640x480 0x110
#define VBE_RES_800x600 0x115
#define VBE_RES_1280x1024 0x11A
#define VBE_RES_1152x864 0x14C

//BX for mode - BIT 14 must be set
#define VBE_BX_SET_MODE BIT(14)

#define GET_N_BYTE(w, n) ((w >> (8*n)) & 0xFF) /**< Get nth byte of a word */


#endif /*_LCOM_VIDEO_MACROS_H_ */
