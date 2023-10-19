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
void parse_message(Context *ctx, char *inc_msg);
void sendMessage(char *message);
void window_events(Context *ctx);
void send_action(char action, Potion *potion);

//-~-~-~-~-~-~-~-~-~-~-~-~ PLAYER -~-~-~-~-~-~-~-~-~-~-~-~

void init_main_screen(Context *ctx);
void main_screen(Context *ctx);
void fill_main_screen(Context *ctx);
void render_main_screen(Context *ctx);

//-~-~-~-~-~-~-~-~-~-~-~-~ CONNECT -~-~-~-~-~-~-~-~-~-~-~-~

void init_connect_screen(Context *context);
void connect_screen(Context *ctx);
void fill_connect_screen(Context *ctx);
void renderConnectScreen(Context *ctx);

//-~-~-~-~-~-~-~-~-~-~-~-~ UTILS -~-~-~-~-~-~-~-~-~-~-~-~

void init_static(Context *ctx, int width, int height);
void startTurn(Context *ctx);
void endTurn(Context *ctx);

void overlay_text( SDL_Texture *dest,SDL_Texture *base, SDL_Rect *baseSrc, uint32_t color, char *text);
SDL_Rect scale_and_center(double scalar, SDL_Rect parent, SDL_Rect this);
SDL_Texture *create_target_texture(int w, int h);

void printRect(SDL_Rect *rect, char * msg);
void get_img_src(SDL_Rect *dst, int imageType, int index);
void generatePotionTexture(SDL_Texture *base, Potion *card, int type);

char *input(Context *ctx);
void reset_game(Context *ctx);
void set_resize_context(Context *context);




#endif
