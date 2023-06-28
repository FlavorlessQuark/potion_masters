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
int execReserve (Context *ctx, uint8_t playerID, char *msg);
int execBuy		(Context *ctx, uint8_t playerID, char *msg);
int execTake	(Context *ctx, uint8_t playerID, char *msg);

// ~-~-~-~-~-~-~- utils.c ~-~-~-~-~-~-~-

Potion *findPotion		(Context *ctx, char *id, int _id);

void nextTurn		(Context *ctx);
void startGame		(Context *ctx);

SDL_Rect scaleAndCenter(double scalar, SDL_Rect parent, SDL_Rect this);

int	extract_num		(char *str, int *number);
int generatePotion	(SDL_Texture *base, Potion *card, int level);

void delReserved 	(Player *player, int cardId);

void cleanup		(Context *ctx);

SDLX_RectContainer *loadConfig(char *filename);

void startGame(Context *ctx);
void get_img_src(SDL_Rect *dst, int imageType, int index);
