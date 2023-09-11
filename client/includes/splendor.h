#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"



//-~-~-~-~-~-~-~-~-~-~-~-~ BOARD -~-~-~-~-~-~-~-~-~-~-~-~

void init_board_static(Context *ctx);
void fill_board(Context *ctx);
void board_screen(Context *ctx);
void render_board_screen(Context *ctx);

//-~-~-~-~-~-~-~-~-~-~-~-~  I/0  -~-~-~-~-~-~-~-~-~-~-~-~

void initConnection(Context *context);
void parse_message(char *inc_msg);
void sendMessage(char *message);
void window_events(Context *ctx);

//-~-~-~-~-~-~-~-~-~-~-~-~ PLAYER -~-~-~-~-~-~-~-~-~-~-~-~

void init_main_screen(Context *ctx);
void main_screen(Context *ctx);
void render_main_screen(Context *ctx);

//-~-~-~-~-~-~-~-~-~-~-~-~ CONNECT -~-~-~-~-~-~-~-~-~-~-~-~

void init_connect_screen(Context *context);
void connect_screen(Context *ctx);
void renderConnectScreen(Context *ctx);

//-~-~-~-~-~-~-~-~-~-~-~-~ UTILS -~-~-~-~-~-~-~-~-~-~-~-~

void init_static(Context *ctx, int width, int height);
void fillPotion(Potion *card);
void startTurn(Context *ctx);
void endTurn(Context *ctx);

SDL_Rect scaleAndCenter(double scalar, SDL_Rect parent, SDL_Rect this);

void printRect(SDL_Rect *rect, char * msg);
void get_img_src(SDL_Rect *dst, int imageType, int index);
void generatePotionTexture(SDL_Texture *base, Potion *card, int type);
void draw_dotted_rect(SDL_Point start, int w, int h, int size);

int extract_card_from_input(Context *ctx, Potion *dst, char *input);
int extract_num(char *str, int *number);
char *input(Context *ctx);

void set_resize_context(Context *context);




#endif
