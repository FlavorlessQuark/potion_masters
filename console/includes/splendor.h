#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

Context *init();
int 	core(void *arg, char *msg);

int 	replaceCard(Row *row, int position);
void	payReserved(Player *player, uint8_t cardId);
int 	replaceCard(Row *row, int position);

extern void 	free_strvec(c_string_vec vec);
c_string_vec 	*get_connections(void);

char 	*recv_from(char *handle);
void 	render_connect_screen(Context *ctx);
int 	send_to(char *handle, char *msg);
int		sendBoardState(Context *ctx, int player);
int 	execMsg(Context *ctx, char *msg);
int 	connect_screen(Context *ctx);



void renderPlayer(PlayerUI *player);
void renderBoard(Context *ctx);

int	extract_num(char *str, int *number);
void findCard(Context *ctx, uint8_t id, uint8_t *row, uint8_t *col);

void cleanup(Context *ctx);
void cleanupUIConfig(SDLX_RectContainer *container);

void startGame(Context *ctx);

#endif
