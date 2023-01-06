#pragma once

#include <lcom/lcf.h>
#include <stdio.h>

#include "../sprites/menu/menu.h"
#include "../sprites/menu/player1_selected.h"
#include "../sprites/menu/player2_selected.h"
#include "../sprites/menu/about_selected.h"
#include "../sprites/numbers/0.h"
#include "../sprites/numbers/1.h"
#include "../sprites/numbers/2.h"
#include "../sprites/numbers/3.h"
#include "../sprites/numbers/4.h"
#include "../sprites/numbers/5.h"
#include "../sprites/end/Lost_menu.h"
#include "../sprites/end/Won_p1_menu.h"
#include "../sprites/end/Won_p2_menu.h"
#include "../sprites/end/Lost_menu_player.h"
#include "../sprites/wait_menu.h"
#include "../sprites/powerups/power1.h"
#include "../sprites/powerups/power2.h"
#include "../sprites/sun.h"
#include "../sprites/discoball.h"
#include "../sprites/about.h"

/** @defgroup images Game Images
 * @{
 *
 * Constants for programming the i8254 Timer.
 */


#define NUM_IMAGES 20 /*!< @brief Total number od images to be load */

/**
 * @brief Enumerates each image in the game 
 * 
 */
enum game_image_t{
    MENU_IMG,               /*!< @brief Menu image and layout */
    PLAYER1_SELECTED_IMG,   /*!< @brief Player1 option selected in menu */
    PLAYER2_SELECTED_IMG,   /*!< @brief Player1 Vs Player2 option selected in menu */
    ABOUT_SELECTED_IMG,     /*!< @brief About option selected in menu */
    NUMBER0_IMG,            /*!< @brief Number "0" for score display */
    NUMBER1_IMG,            /*!< @brief Number "1" for score display */
    NUMBER2_IMG,            /*!< @brief Number "2" for score display */
    NUMBER3_IMG,            /*!< @brief Number "3" for score display */
    NUMBER4_IMG,            /*!< @brief Number "4" for score display */
    NUMBER5_IMG,            /*!< @brief Number "5" for score display */
    LOST_MENU_IMG,          /*!< @brief Message that indicates that the player has lost */
    LOST_MENU_PLAYER_IMG,
    WON_P1_MENU_IMG,        /*!< @brief Message that indicates that the Player1 has won */
    WON_P2_MENU_IMG,        /*!< @brief Message that indicates that the Player2 has won */
    POWER1_IMG,             /*!< @brief Power-Up-1 Icon */
    POWER2_IMG,             /*!< @brief Power-Up-2 Icon */
    WAIT_MENU_IMG,          /*!< @brief Wait menu image and layout */
    SUN_IMG,                /*!< @brief Sun image for a given RTC time */
    DISCO_IMG,              /*!< @brief Disco image for a given RTC time */
    ABOUT_PAGE_IMG          /*!< @brief About page image */
};

/**
 * @brief Images data cointainer
 * 
 */
xpm_image_t game_images[NUM_IMAGES];

/**
 * @brief Load all images into the game
 * 
 */
void (load_all_images)();

/**@}*/
