#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

void init(Context *ctx, int width, int height);
char *input(Context *ctx);

void window_events(Context *ctx);

void init_board_screen(Context *ctx);
void init_buy_screen(Context *ctx);
void init_connect_screen(Context *context);
void init_main_screen(Context *ctx);

void initConnection(Context *context);

void board_screen(Context *ctx);
void buy_screen(Context *ctx);
void connect_screen(Context *ctx);
void main_screen(Context *ctx);

void render_board_screen(Context *ctx);
void renderConnectScreen(Context *ctx);
void render_buy_screen(Context *ctx);
void render_main_screen(Context *ctx);


void sendReserve(Context *ctx);
void sendPay(Context *ctx);
void sendTakeTokens(Context *ctx, uint8_t *taken);
void parseMsg(Context *ctx, char *msg);
void fillCard(Card *card, int _id, char *id);

int extract_num(char *str, int *number);
SDLX_RectContainer *parseUI(char *filename);
void delReserved(Context *ctx, int id);

void startTurn(Context *ctx);
void endTurn(Context *ctx);

void printRect(SDL_Rect *rect, char * msg);
void sendMessage(char *message);

#endif
