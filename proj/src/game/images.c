#include "images.h"

void (load_all_images)(){
    xpm_load(MENU_SPRITE, XPM_8_8_8_8, &game_images[MENU_IMG]);

    xpm_load(PLAYER1_SELECTED_SPRITE, XPM_8_8_8_8, &game_images[PLAYER1_SELECTED_IMG]);
    xpm_load(PLAYER2_SELECTED_SPRITE, XPM_8_8_8_8, &game_images[PLAYER2_SELECTED_IMG]);
    xpm_load(ABOUT_SELECTED_SPRITE, XPM_8_8_8_8, &game_images[ABOUT_SELECTED_IMG]);

    xpm_load(WAIT_MENU_SPRITE, XPM_8_8_8_8, &game_images[WAIT_MENU_IMG]);

    xpm_load(NUMBER0_SPRITE, XPM_8_8_8_8, &game_images[NUMBER0_IMG]);
    xpm_load(NUMBER1_SPRITE, XPM_8_8_8_8, &game_images[NUMBER1_IMG]);
    xpm_load(NUMBER2_SPRITE, XPM_8_8_8_8, &game_images[NUMBER2_IMG]);
    xpm_load(NUMBER3_SPRITE, XPM_8_8_8_8, &game_images[NUMBER3_IMG]);
    xpm_load(NUMBER4_SPRITE, XPM_8_8_8_8, &game_images[NUMBER4_IMG]);
    xpm_load(NUMBER5_SPRITE, XPM_8_8_8_8, &game_images[NUMBER5_IMG]);

    xpm_load(LOST_MENU_SPRITE, XPM_8_8_8_8, &game_images[LOST_MENU_IMG]);
    xpm_load(LOST_MENU_PLAYER_SPRITE, XPM_8_8_8_8, &game_images[LOST_MENU_PLAYER_IMG]);
    xpm_load(WON_P1_MENU_SPRITE, XPM_8_8_8_8, &game_images[WON_P1_MENU_IMG]);
    xpm_load(WON_P2_MENU_SPRITE, XPM_8_8_8_8, &game_images[WON_P2_MENU_IMG]);

    xpm_load(POWER1_SPRITE, XPM_8_8_8_8, &game_images[POWER1_IMG]);
    xpm_load(POWER2_SPRITE, XPM_8_8_8_8, &game_images[POWER2_IMG]);

    xpm_load(SUN_SPRITE, XPM_8_8_8_8, &game_images[SUN_IMG]);
    xpm_load(DISCO_SPRITE, XPM_8_8_8_8, &game_images[DISCO_IMG]);

    xpm_load(ABOUT_PAGE_SPRITE, XPM_8_8_8_8, &game_images[ABOUT_PAGE_IMG]);
}
