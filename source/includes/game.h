#ifndef SPLENDOR_H
# define SPLENDOR_H

#include "game_structs.h"
#include "macros.h"

int main_menu(Context *ctx);

void init_board_static(Context *ctx);
void board_screen(Context *ctx);

void init_main_screen(Context *ctx);
void main_screen(Context *ctx);

int title_screen(Context *ctx);


int init_lobby();
int lobby_screen();


void init_game(Context *ctx, int width, int height);

#endif
