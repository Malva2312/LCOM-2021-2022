#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include "handlers.h"
#include "images.h"

/** @defgroup model Game Model
 * @{
 * 
 * Functions and data structures useful to the construction of the game
 */
#define ENTRIES 3 /*!< Total number of entries on the menu */

/**
 * @brief In the menu, it goes to the next option.
 * If there is no next option, the selection remains unchanged
 * 
 */
void (addMenuEntry)();
/**
 * @brief In the menu, it goes to the previous option.
 * If there is no previous option, the selection remains unchanged
 * 
 */
void (backMenuEntry)();
/**
 * @brief Returns the value of the corrent entry in the menu
 * @return  
 * 
 */
int (getCurrentEntryImg)();

/**
 * @brief Struct that saves all data about one player
 * 
 */
struct Player{
    int x_pos;          /*!< @brief Position of the player on the x-axis */
    int y_pos;          /*!< @brief Position of the player on the y-axis */
    unsigned int color; /*!< @brief Colour with which the player is drawn  */
    int vel;            /*!< @brief Distance, on the y-axis, per unit of time */
    int height;         /*!< @brief Height of the player's paddle */
    int width;          /*!< @brief Width of the player's paddle */
    int score;          /*!< @brief Total points scored in the match */
};
/**
 * @brief Struct that saves all data about the ball
 * 
 */
struct Ball{
    int x_pos;          /*!< @brief Position of the ball on the x-axis */
    int y_pos;          /*!< @brief Position of the ball on the y-axis */
    unsigned int color; /*!< @brief Colour with which the ball is drawn  */
    int vel_x;          /*!< @brief Distance, on the x-axis, per unit of time */
    int vel_y;          /*!< @brief Distance, on the y-axis, per unit of time */
    int height;         /*!< @brief Height of the ball */
    int width;          /*!< @brief Width of the ball */
};
/**
 * @brief Struct that saves all data about game arena
 * 
 */
struct Arena{
    int max_y;          /*!< @brief Maximum value of position on y-axis */
    int min_y;          /*!< @brief Minimum value of position on y-axis */
    int max_x;          /*!< @brief Maximum value of position on x-axis */
    int min_x;          /*!< @brief Minimum value of position on y-axis */
    int height;         /*!< @brief Length of the arena */
    int width;          /*!< @brief Width of the arena */
};
/**
 * @brief Struct that saves all data about the mouse
 * 
 */
struct Mouse{
    int x_pos;          /*!< @brief Position of the mouse on the x-axis */
    int y_pos;          /*!< @brief Position of the mouse on the y-axis */
    unsigned int color; /*!< @brief Colour with which the mouse is drawn  */
    int height;         /*!< @brief Lenght of the mouse */
    int width;          /*!< @brief Width of the mouse */
};

/**
 * @brief Struct that saves all data about the wall (origianted by a power-up)
 * 
 */
struct Wall{
    int x_pos;          /*!< @brief Position of the Wall on the x-axis */
    int y_pos;          /*!< @brief Position of the Wall on the y-axis */
    int height;         /*!< @brief Lenght of the Wall */
    int width;          /*!< @brief Width of the Wall */
    bool active;        /*!< @brief true if the power-up is activated, false otherwise */
    int decreaseRate;   /*!< @brief Rate at which the Wall is reducing in size */
    int timeout;        /*!< @brief Time needed to use the power-up again*/
};
/**
 * @brief Returns a default Player1 
 * @return
 * 
 */
struct Player (getPlayer)();
/**
 * @brief Returns a default Arena
 * @return
 */
struct Arena (getArena)();
/**
 * @brief Returns a default ball 
 * @return
 */
struct Ball (getBall)();
/**
 * @brief Resets the ball and the player(s) after someone has scored a point
 * 
 */
void resetPositions();
/**
 * @brief Resets the ball, the player(s) and the scores at the start of a match
 * 
 */
void (resetGame)();
/**
 * @brief Receives a Player and moves it up (y-axis) 
 * @param player
 */
void (playerUp)();
/**
 * @brief Receives a Player and moves it down (y-axis) 
 * @param player
 */
void (playerDown)();
/**
 * @brief Checks if any player has scored a point and raises his score if so
 * Returns true if someone has scored
 * 
 * @return true 
 * @return false 
 */
bool goal();
/**
 * @brief Calculates and moves the ball to its next position
 * Changes trajectory if collisions are detected
 */
bool (moveBall)();
/**
 *  @brief Moves the computer-controlled player in the case of Single Player mode
 * 
 */
void (movePCPlayer)();
/**
 * @brief Return true if any of the players has won the game
 * @return true
 * @return false
 */
bool (gameWinner)();
/**
 * @brief Changes the position of the mouse cursor depending on the input data (x-axis, y-axis)
 * @param x
 * @param y
 */
void (moveMouse)(int x, int y);
/**
 * @brief Activates the power associated with the left mouse click
 * 
 */
void (mouseActionLeft)();
/**
 * @brief Activates the power associated with the righ mouse click
 * 
 */
void (mouseActionRight)();
/**
 * @brief Reduces the wall (power up) over time
 * 
 */
void (wallDecrease)();

/**@}*/
