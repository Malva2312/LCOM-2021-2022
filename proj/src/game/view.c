#include "view.h"

#include "../rtc/rtc.h"
/*MENU VIEW*/

extern int menu_entries[];
extern bool darkmode;

void (drawMenu)(){

    draw_sprite(0, 0, game_images[MENU_IMG]);
    draw_sprite(0, 0, game_images[getCurrentEntryImg()]);
    if(!darkmode){
        draw_sprite(0, 0, game_images[SUN_IMG]);
    }
    else{
        draw_sprite(0, 0, game_images[DISCO_IMG]);
    }

}

void (drawWaitMenu)() {
    draw_sprite(0, 0, game_images[WAIT_MENU_IMG]);
}

/*GAME_VIEW*/
extern struct Player player;
extern struct Player player2;
extern struct Player PCplayer;
extern struct Arena arena;
extern struct Ball ball;
extern struct Mouse mouse;
extern struct Wall wall;
extern state_t state;
extern int actionLeftTimeout;

enum game_image_t getNumberImg(int score){ 
    switch (score)
    {
    case 0:
        return NUMBER0_IMG;
        break;
    case 1:
        return NUMBER1_IMG;
        break;
    case 2:
        return NUMBER2_IMG;
        break;
    case 3:
        return NUMBER3_IMG;
        break;
    case 4:
        return NUMBER4_IMG;
        break;
    default:
        return NUMBER5_IMG;
        break;
    }
}

void (drawBoard)(){
    if (state==TWOPGAME)
    {
        if (player.x_pos < player2.x_pos) {
            draw_sprite(450, 10, game_images[getNumberImg(player2.score)]);
            draw_sprite(300, 10, game_images[getNumberImg(player.score)]);
        } else {
            draw_sprite(300, 10, game_images[getNumberImg(player2.score)]);
            draw_sprite(450, 10, game_images[getNumberImg(player.score)]);
        }
        
    }
    else{
        draw_sprite(450, 10, game_images[getNumberImg(PCplayer.score)]);
        draw_sprite(300, 10, game_images[getNumberImg(player.score)]);
        if(actionLeftTimeout == 0){
            draw_sprite(630, 40, game_images[POWER1_IMG]);
        }
        if(!wall.active){
            draw_sprite(690, 40, game_images[POWER2_IMG]);
        }
    }
}

void (drawPlayer)(struct Player *p){
    draw_rectangle(p->color, p->x_pos, p->y_pos, p->width, p->height);
}

void (drawBall)(){
    draw_rectangle(ball.color, ball.x_pos, ball.y_pos, ball.width, ball.height);
}

void (drawGame)(){
    draw_rectangle(0, 0, 0, 800, 600);
    drawPlayer(&player);
    if (state==TWOPGAME)
    {
       drawPlayer(&player2);
    }
    else{
        drawPlayer(&PCplayer);
    }
    
    drawBall();
}

void (drawArena)(){
    drawBoard();
    //bar of score
    vg_draw_hline((arena.width/2)+arena.min_x-15, 50, 30, 0xffffff);

    vg_draw_hline(arena.min_x, arena.min_y, arena.width, 0xffffff);
    vg_draw_vline(arena.min_x, arena.min_y, 160, 0xffffff);
    vg_draw_vline(arena.min_x, arena.min_y + 240, 160, 0xffffff);

    for (int i = 0; i <= arena.height; i+=30)
    {
        vg_draw_vline(400, 100+i, 10, 0xffffff);
    }

    vg_draw_hline(arena.min_x, arena.max_y, arena.width, 0xffffff);
    vg_draw_vline(arena.max_x, arena.min_y, 160, 0xffffff);
    vg_draw_vline(arena.max_x, arena.min_y + 240, 160, 0xffffff);
}

void(drawMouse)(){
    draw_rectangle(mouse.color, mouse.x_pos, mouse.y_pos, mouse.width, mouse.height);
}

void(drawWall)(){
    if(wall.active){
        draw_rectangle(0xffffff, wall.x_pos, wall.y_pos, wall.width, wall.height);
    }
}

/*ENG GAME VIEW*/
extern int winner;

void (drawEndGame)(){
    if(winner == 1){
        draw_sprite(0, 0, game_images[WON_P1_MENU_IMG]);
    }
    else if(winner == 2){
        draw_sprite(0, 0, game_images[LOST_MENU_IMG]);
    }
    else if(winner == 3){
        draw_sprite(0, 0, game_images[WON_P2_MENU_IMG]);
    }
}

/*ABOUT VIEW*/

void(drawAbout)(){
    draw_sprite(0, 0, game_images[ABOUT_PAGE_IMG]);
}
