#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

void init(Context *ctx);

void 	payReserved(Player *player, int position);
char 	*createMsg(Context *ctx);
void 	parseMsg(Context *ctx, char *msg);
char 	*input(Context *ctx);

SDLX_RectContainer *initUI(char *filename);

void window_events(Context *ctx);

void init_board_screen(Context *ctx);
void board_screen(Context *ctx);
void render_board_screen(Context *ctx);

void ConnectScreen(Context *ctx);
void initConnection(Context *context);
void renderConnectScreen(Context *ctx);

void init_main_screen(Context *ctx);
void main_screen(Context *ctx);


void init_buy_screen(Context *ctx);
void buy_screen(Context *ctx);

#endif
