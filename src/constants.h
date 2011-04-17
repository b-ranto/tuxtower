#ifndef __CONSTANTS__
#define __CONSTANTS__
#define NORM_UP 27 // Normal jump "speed"
#define BONUS_UP 32 // Superjump "speed", grows 
#define FRAME_WAIT 25 // How often draw a screen(in miliseconds) *
#define LIMIT_ADD 7 // How often should change the level *
#define BACK_X 50 // Width of background(X)
#define BACK_Y 50 // Height of background(Y)
#define TYP_X ((float)0.4) // X-normalization for different resolution *
#define TYP_Y 48 // Y-normalization for different resolution *
#define NASOBIC_1 50 // (X-)Speed while jumping or standing on the platform *
#define NASOBIC_2 40 // (X-)Speed when falling *
#define NEW_LEVEL 100 // How much to add when reaching new level
#define VYREZ_X 50 // (X-)place around main screen
#define VYREZ_Y 0 // (Y-)place around main screen
#define BAR_X 50 // Width of sidebar
#define BAR_Y 279 // Height of sidebar
#define MENU_ITEMS 2 // Number of menu items
#define JUMP_MAX (((NORM_UP-6)/2)*(NORM_UP-6)-20)
#define STR_MAX 1024
#endif
