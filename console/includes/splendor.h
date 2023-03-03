#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

// ~-~-~-~-~-~-~- config_parser.c ~-~-~-~-~-~-~

SDLX_RectContainer *parse_UIConfig(char *filename);

// ~-~-~-~-~-~-~ connection.c ~-~-~-~-~-~-~

c_string_vec 	*get_connections(void);
extern void 	free_strvec(c_string_vec vec);
void 	render_connect_screen(Context *ctx);

char 	*recv_from	(char *handle);
int 	send_to		(char *handle, char *msg);

int sortHandles(const void *a, const void *b);

int connect_handles		(Context *ctx, char **handles, int len);
int disconnect_handles	(Context *ctx, char **handles, int len);
int handle_Connect		(Context *ctx, c_string_vec *new);

// ~-~-~-~-~-~-~ core_loops.c ~-~-~-~-~-~-~

int 	core(void *arg, char *msg);
int 	connect_screen(Context *ctx);
int		main_game(Context *ctx);

// ~-~-~-~-~-~-~- init.c ~-~-~-~-~-~-~

Context *init();

void initBoard	 (Context *ctx, SDLX_RectContainer *root);
void initRowCards(Context *ctx, SDLX_RectContainer *container, int level);
void initPlayer	 (Context *ctx, uint8_t id, SDLX_RectContainer *root);

void init_connectScreen(Context *ctx);

// ~-~-~-~-~-~-~- IO.c ~-~-~-~-~-~-~-
int	sendBoardState(Context *ctx, int player);
int execMsg		(Context *ctx, char *msg);
int execReserve (Context *ctx, uint8_t playerID, char *msg);
int execBuy		(Context *ctx, uint8_t playerID, char *msg);
int execTake	(Context *ctx, uint8_t playerID, char *msg);

// ~-~-~-~-~-~-~- render.c ~-~-~-~-~-~-~-

void renderPlayer(Player *player);
void renderBoard (Context *ctx);


// ~-~-~-~-~-~-~- utils.c ~-~-~-~-~-~-~-

Card *findCard		(Context *ctx, char *id, int _id);

void nextTurn		(Context *ctx);
void startGame		(Context *ctx);

int	extract_num		(char *str, int *number);
int generateCard	(Card *card, int level);

void delReserved 	(Player *player, int cardId);
void print_config	(Context *ctx, SDLX_RectContainer *root);
void cleanupUIConfig(SDLX_RectContainer *container);
void cleanup		(Context *ctx);

SDLX_RectContainer *loadConfig(char *filename);

void startGame(Context *ctx);

#endif
