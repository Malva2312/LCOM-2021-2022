#pragma once

#include <lcom/lcf.h>
#include <stdio.h>

#include "model.h"
#include "images.h"
#include "../video/video.h"
#include "handlers.h"

struct Player;

/** @defgroup view Game View
 * @{
 * 
 * Functions responsible for the output of the game on screen
 */
/**
 * @brief Calls the functions responsible for drawing the menu
 * 
 */
void (drawMenu)();
/**
 * @brief Calls the function responsible for drawing the waiting menu, while waiting other player
 * 
 */
void (drawWaitMenu)();
/**
 * @brief Calls the functions responsable for drawing the score board
 * 
 */
void (drawBoard)();
/**
 * @brief Receives a Player pointer and draws the Player's paddle in the arena according to the pointer data
 * @param p player pointer that containt data about the player
 */
void (drawPlayer)(struct Player *p);
/**
 * @brief Draws the Ball in the arena according to Ball's data
 * 
 */
void (drawBall)();
/**
 * @brief Calls the functions needed to draw the match
 * 
 */
void (drawGame)();
/**
 * @brief Draw the Arena and its boundaries
 * 
 */
void (drawArena)();
/**
 * @brief Draws the mouse cursor according to its position
 * 
 */
void(drawMouse)();
/**
 * @brief Draw the wall originated by a power-up
 * 
 */
void(drawWall)();
/**
 * @brief Draws an end-of-game message depending on whether the player wins or loses
 * 
 */
void (drawEndGame)();
/**
 * @brief Draws about menu 
 * 
 */
void (drawAbout)();
/**@}*/
