#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

/** @defgroup serialport Serial Port
 * @{
 * 
 * Serial Port
*/

/******** CONSTANTS ********/

#define SER_IRQ     4           /*!< @brief IRQ line of the serial port */
#define SER_PORT    0x3F8       /*!< @brief Address of the host pipe */

#define SER_INIT    2           /*!< @brief Message signaling being ready for connection */
#define SER_START   3           /*!< @brief Message signaling to start game execution */

#define SER_GOAL_1  40          /*!< @brief Message signaling goal from player 1 */
#define SER_GOAL_2  41          /*!< @brief Message signaling goal from player 2 */

#define SER_WINNER  100         /*!< @brief Winner number are transferred with this number added */
#define SER_RBR 0               /*!< @brief Byte offset of the Reciever Buffer Register */
#define SER_THR 0               /*!< @brief Byte offset of the Transmitter Holding Register */

//---LSR----
#define SER_LSR         5       /*!< @brief Byte offset of the Line Status Register */
#define SER_DATA_READY  BIT(0)  /*!< @brief Bit in LSR signaling data available and ready for reading */
#define SER_OVERRUN_ERR BIT(1)  /*!< @brief Bit in LSR signaling overrun error */
#define SER_PARITY_ERR  BIT(2)  /*!< @brief Bit in LSR signaling parity error */
#define SER_FRAME_ERR   BIT(3)  /*!< @brief Bit in LSR signaling framing error */
#define SER_UART_READY  BIT(5)  /*!< @brief Bit in LSR signaling transmitter register ready for recieving data */
#define SER_THR_EMPTY   BIT(6)  /*!< @brief Bit in LSR signaling transmitter register empty */

//---IER----
#define SER_IER     1           /*!< @brief Byte offset of the Interrupt Enable Register */
#define SER_ERBFI   BIT(0)      /*!< @brief Bit in LSR signaling to enable recieved data available interrupts*/

/******** FUNCTIONS ********/

/**
 * @brief Enables interrupts for recieving data, reads leftover data in the Receiver Buffer Register and transmits a initial message saying it is ready to connect
 * @return 1 - error | 0 - success
 */
int (ser_init)();
/**
 * @brief Reads the Line Status Register to check for errors and available data
 * @return 1 - error | 0 - success
 */
int (ser_read_lsr_status)(uint8_t* status);
/**
 * @brief If data is available it will read all entries in the buffer until there are none left(to maintain it updated)
 * @return 1 - error | 0 - success
 */
int (ser_read_data)();
/**
 * @brief Sends the given data to the Transmitter Holding Register
 * @return 1 - error | 0 - success
 */
int (ser_transmit_data)(uint8_t data);
/**
 * @brief Used in the beginning of the connection to determine what to do next
 * @return 1 - error | 0 - success
 */
int (ser_check_connection)();
/**
 * @brief Unubscribes to interrupts from the Serial Port
 * @return 1 - error | 0 - success
 */
int (ser_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unubscribes to interrupts from the Serial Port
 * @return 1 - error | 0 - success
 */
int (ser_unsubscribe_int)();

/**@}*/
