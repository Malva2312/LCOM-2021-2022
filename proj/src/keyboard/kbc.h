#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>	

#include "i8042.h"

/** @defgroup kbc Keybord
 * @{
 * 
 * Lab3 - Keybord Fuctions
*/

/**
 * @brief Control of keyboard interruptions
 * 
 */
void (kbc_ih)(void);
/**
 * @brief Get status byte for keyboard
 * @return 0 if OK, 1 otherwise
 * 
 */
int (kbd_get_status_byte)();
/**
 * @brief Read a value from the kbc output buffer
 * @return 0 if OK, 1 otherwise
 */
int (kbc_read_value)();
/**
 * @brief Issue a command to the kbc
 * @return 0 if OK, 1 otherwise
 */
void (kbc_issue_command)(uint8_t cmd);
/**
 * @brief Write a command to the kbc
 * 
 */
int (kbc_write_cmd)(uint8_t cmd);
/**
 * @brief Read a command from the kbc
 * @return 0 if OK, 1 otherwise
 */
int (kbc_read_cmd)(uint8_t* cmd);
/**
 * @brief Restore keyboard interrupts
 * 
 */
void (kbc_restore_interrupts)();
/**
 * @brief Subscribe interrupts from keyboard
 * @return 0 if OK, 1 otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribe interrupts from keyboard
 * @return 0 if OK, 1 otherwise
 */
int (kbd_unsubscribe_int)();

/*---------------MOUSE--------------*/

/**
 * @brief Subscribe mouse interrupts
 * @return 0 if OK, 1 otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribe mouse interrupts
 * @return 0 if OK, 1 otherwise
 */
int (mouse_unsubscribe_int)();
/**
 * @brief Enable mouse interrupts
 * @return 0 if OK, 1 otherwise
 */
int (enable_mouse)();
/**
 * @brief Disable mouse interrupts
 * @return 0 if OK, 1 otherwise
 */
int (disable_mouse)();
/**
 * @brief Write a command to a port
 * @return 0 if OK, 1 otherwise
 */
int (mouse_write_to_port)(uint8_t port, uint8_t cmd);
int (mouse_write_cmd)(uint8_t cmd);
/**
 * @brief Enable data reporting
 * @return 0 if OK, 1 otherwise
 */
int (mouse_enbl_data_reporting)();
/**
 * @brief Disable mouse data reporting
 * @return 0 if OK, 1 otherwise
 */
int (mouse_disable_data_reporting)();
/**
 * @brief Control of mouse interrupts
 * @return 0 if OK, 1 otherwise
 */
void (mouse_ih)(void);
/**
 * @brief Read a value from the mouse, to build the mouse packet
 * @return 0 if OK, 1 otherwise
 */
int (mouse_read_value)();
/**
 * @brief Parse a packet with mouse information into a struct
 * @return 0 if OK, 1 otherwise
 */
int (parse_packet)();

/**@}*/
