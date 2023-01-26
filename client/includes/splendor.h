#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

void init(Context *ctx);

void 	payReserved(Player *player, int position);
char 	*createMsg(Context *ctx);
void 	parseMsg(Context *ctx, char *msg);
char 	*input(Context *ctx);

void window_events(Context *ctx);
void initConnection(Context *context);

void ConnectScreen(Context *ctx);
SDLX_RectContainer *initUI(char *filename);
void init_main_screen(Context *ctx);
void renderConnectScreen(Context *ctx);
void main_screen(Context *ctx);
void renderBoard(Context *ctx);
void renderPlayer(Context *ctx);
void renderBuyMenu(Context *ctx);
void renderSwitchButtons(Context *ctx);

#endif
