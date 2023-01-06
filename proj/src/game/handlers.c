#include "handlers.h"

extern uint8_t scancode[2];
extern struct packet pp;
extern int menu_entries[];
extern int counter;
extern int actionLeftTimeout;
extern struct Player player;
extern struct Player player2;
extern uint8_t read_data;
extern int player1_initial_x;
extern int player2_initial_x;
extern struct Wall wall;
extern int winner;

state_t state = MENU;

void (mainHandler)(int device){
    switch (state){
        case MENU: {
            menuHandler(device);
            break;
        }
        case ONEPGAME: {
            gameOnePlayerHandler(device);
            break;
        }
        case WAITING: {
            waitingHandler(device);
            break;
        }
        case TWOPGAME: {
            gameTwoPlayersHandler(device);
            break;
        }
        case ABOUT: {
            aboutHandler(device);
            break;
        }
        case ENDGAME: {
            endGameHandler(device);
            break;
        }
    }
}

void (menuHandler)(int device){
    switch (device){
        case KEYBOARD: {
            if(scancode[0] == KBC_TWO_BYTE && scancode[1] == KEY_UP){
                backMenuEntry();
            }
            else if(scancode[0] == KBC_TWO_BYTE && scancode[1] == KEY_DOWN){
                addMenuEntry();
            }
            else if(scancode[0] == KEY_ENTER){
                if(getCurrentEntryImg() == PLAYER1_SELECTED_IMG){
                    state = ONEPGAME;
                    clear_buffer();
                    refresh_buffer();
                }
                else if(getCurrentEntryImg() == PLAYER2_SELECTED_IMG){
                    printf("2player\n");
                    ser_init();
                    state = WAITING;
                    printf("state: %d\n", state);
                    clear_buffer();
                    drawWaitMenu();
                    refresh_buffer();
                }
                else if(getCurrentEntryImg() == ABOUT_SELECTED_IMG){
                    state = ABOUT;
                    clear_buffer();
                    refresh_buffer();
                }
            }
            break;
        }
        case TIMER: {
            if(counter % REFRESH_RATE == 0){
                drawMenu();
                refresh_buffer();
            }
            break;
        }
        case MOUSE: {
            break;
        }
        default:
            break;
    }
}

void (gameOnePlayerHandler)(int device){
    switch (device){
        case KEYBOARD: {
            switch (scancode[0])
            {
            case KEY_W:
                playerUp(&player);
                break;
            case KEY_S:
                playerDown(&player);
                break;
            default:
                break;
            }
            break;
        }
        case TIMER: {
            if(counter % REFRESH_RATE == 0){
                if(wall.active){
                    if (counter % wall.decreaseRate == 0){
                        wallDecrease();
                    }
                }
                else{
                    if(wall.timeout>0){
                        wall.timeout--;
                    }
                }

                if(actionLeftTimeout > 0){
                    actionLeftTimeout--;
                }

                if(moveBall()){
                    if(gameWinner()){
                        resetGame();
                        state = ENDGAME;
                    }
                }
                movePCPlayer();
                drawGame();
                drawArena();
                drawMouse();
                drawWall();
                
                refresh_buffer();
            }
            break;
        }
        case MOUSE:{
            moveMouse(pp.delta_x, pp.delta_y);

            if(pp.lb && actionLeftTimeout == 0){
                mouseActionLeft();
            }

            if(pp.rb && !wall.active && wall.timeout == 0){
                mouseActionRight();
            }
            break;
        }
        default:
            break;
    }
}
void (waitingHandler)(int device){
    unsigned int ser_state;
    switch (device)
    {
    case SERIALPORT:
        ser_state = ser_check_connection();
        printf("state: %d\n",ser_state);
        if (!ser_state) break;
        state = TWOPGAME;
        if (ser_state == SER_INIT){
            int player1_initial_aux = player1_initial_x;
            player1_initial_x = player2_initial_x;
            player2_initial_x = player1_initial_aux;
            unsigned int aux_color = player.color;
            player.color = player2.color;
            player2.color = aux_color;
            resetPositions();
        }
        break;
    /*case TIMER:
        if(counter % 120 == 0)
            ser_transmit_data(SER_INIT);
        break;*/
    default:
        break;
    }
}

void (gameTwoPlayersHandler)(int device){
    switch (device){
        case KEYBOARD: {
            switch (scancode[0])
            {
            case KEY_W:
                playerUp(&player);
                ser_transmit_data((uint8_t) (player.y_pos / player.vel));
                break;
            case KEY_S:
                playerDown(&player);
                printf("befor pos: %d\n",player.y_pos);
                ser_transmit_data((uint8_t) (player.y_pos / player.vel));
                break;
            default:
                break;
            }
            break;
        }
        case TIMER: {
            if(counter % REFRESH_RATE == 0){
                if(moveBall()){
                    if(gameWinner()){
                        resetGame();
                        state = ENDGAME;
                    }
                }
                drawGame();
                drawArena();
                
                refresh_buffer();
            }
            break;
        }
        case SERIALPORT: {
            if (read_data == SER_GOAL_1) {
                player.score++;
                resetPositions();
                break;
            }
            if (read_data == SER_GOAL_2) {
                player2.score++;
                resetPositions();
                break;
            }
            if (read_data == (SER_WINNER + 1) || read_data == (SER_WINNER + 2) || read_data == (SER_WINNER + 3)) {
                winner = ((int) read_data) - SER_WINNER;
                resetGame();
                state = ENDGAME;
                break;
            }
            if ((((int) read_data) * player2.vel) > 600) break;
            player2.y_pos = ((int) read_data) * player2.vel;
            printf("after pos2: %d\n",player2.y_pos);
            printf("after pos1: %d\n",player.y_pos);
            break;
        }
        default:
            break;
    }
}
void (aboutHandler)(int device){
    switch (device)
    {
    case KEYBOARD:{
        if(scancode[0] == KEY_ENTER){
            state = MENU;
        }
        break;
    }
    case TIMER: {
        if(counter % REFRESH_RATE == 0){
            drawAbout();
            refresh_buffer();
        }
    }
    default:
        break;
    }
    return;
}

void (endGameHandler)(int device){
    switch (device)
    {
    case KEYBOARD:{
        if(scancode[0] == KEY_ENTER){
            state = MENU;
        }
        break;
    }
    case TIMER: {
        if(counter % REFRESH_RATE == 0){
            drawEndGame();
            refresh_buffer();
        }
    }
    default:
        break;
    }
    return;
}
