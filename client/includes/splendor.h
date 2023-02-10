#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

void init(Context *ctx);
char *input(Context *ctx);

void window_events(Context *ctx);

void init_board_screen(Context *ctx);
void init_buy_screen(Context *ctx);
void init_connect_screen(Context *context);
void init_main_screen(Context *ctx);

void initConnection(Context *context);

void board_screen(Context *ctx);
void buy_screen(Context *ctx);
void ConnectScreen(Context *ctx);
void main_screen(Context *ctx);

void render_board_screen(Context *ctx);
void renderConnectScreen(Context *ctx);
void render_buy_screen(Context *ctx);
void render_main_screen(Context *ctx);


void composeReserve(Context *ctx);
void composePay(Context *ctx);
void composeTakeTokens(Context *ctx);
void parseMsg(Context *ctx, char *msg);

int extract_num(char *str, int *number);
SDLX_RectContainer *parseUI(char *filename);

#endif
