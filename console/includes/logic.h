#include "splendor_structs.h"

// ~-~-~-~-~-~-~ core_loops.c ~-~-~-~-~-~-~

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

SDL_Rect scale_and_center(double scalar, SDL_Rect parent, SDL_Rect this);

int	extract_num		(char *str, int *number);
int generatePotion(Context *ctx, Potion *card, int level);

void cleanup		(Context *ctx);

SDLX_RectContainer *loadConfig(char *filename);

void get_img_src(SDL_Rect *dst, int imageType, int index);
