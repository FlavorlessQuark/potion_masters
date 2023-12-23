#ifndef _GAME_GRAPHICS_H
# define _GAME_GRAPHICS_H

#include "game_structs.h"

// ~-~-~-~-~-~-~-~-  render.c ~-~-~-~-~-~-~-~- -~-

void render_connect_screen(Context *ctx);
void renderPlayer(Context *ctx, Player *player);
void renderBoard (Context *ctx);

// ~-~-~-~-~-~-~-~-~- utils.c ~-~-~-~-~-~-~~-~-~-~


// ~-~-~-~-~-~-~-~-~- graphics.c ~-~-~-~-~-~-~~-~-~-~
void init_UI(Context *ctx);


int 	core(void *arg, char *msg);
int 	connect_screen(Context *ctx);
int		main_game(Context *ctx);
int 	title_screen(Context *ctx);

// ~-~-~-~-~-~-~- init.c ~-~-~-~-~-~-~

Context *init();

void init_new_game(Context *ctx);
void init_connectScreen(Context *ctx);

// ~-~-~-~-~-~-~- messages.c ~-~-~-~-~-~-~-
int	send_game_state(Context *ctx, int player);
int execMsg		(Context *ctx, char *msg);
int parse_action(Context *ctx, char * msg);

// ~-~-~-~-~-~-~- utils.c ~-~-~-~-~-~-~-

void copy_potion(Potion *dst, Potion *src);

void start_next_turn(Context *ctx);
void startGame		(Context *ctx);

int can_buy_potion(Player *player, Potion *potion);

int	extract_num		(char *str, int *number);
int generate_potion(Context *ctx, Potion *card, int level);

void draw_player_essences(Context *ctx, Player *player);
SDL_Rect scale_and_center(double scalar, SDL_Rect parent, SDL_Rect this);
void overlay_text(SDL_Texture *dest, SDL_Texture *base, SDL_Rect *baseSrc,  uint32_t color, char *text);
void generate_potion_texture(Context *ctx, Potion *card);
SDL_Texture *create_target_texture(int w, int h);

void cleanup		(Context *ctx);

// ~-~-~-~-~-~-~ connection.c ~-~-~-~-~-~-~

void 	render_connect_screen(Context *ctx);

char 	*recv_from	(char *handle);
int 	send_to		(char *handle, char *msg);

int sortHandles(const void *a, const void *b);

int connect_handles		(Context *ctx, char **handles, int len);
int disconnect_handles	(Context *ctx, char **handles, int len);
// int handle_Connect		(Context *ctx, c_string_vec *new);

#endif
