#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_structs.h"

Context *init();
int 	replaceCard(Row *row, int position);
int 	core(void *arg, char *msg);
int 	execMsg(Context *ctx, char *msg);
void payReserved(Player *player, int position);
int replaceCard(Row *row, int position);
extern void 	free_strvec(c_string_vec vec);
c_string_vec 	*getConnections(void);
char 	*recvFrom(int handle);
void 			renderConnectScreen(Context *ctx, uint8_t ready[MAX_PLAYERS]);
int 			sendTo(char *msg, int handle);
int		 sendBoardState(Context *ctx, int player);
void cleanup(Context *ctx);
void cleanupUIConfig(SDLX_RectContainer *container);
#endif
