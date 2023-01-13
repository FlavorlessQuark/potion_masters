#ifndef SPLENDOR_H
# define SPLENDOR_H
# include "splendor_strutcs.h"

Context *init();
int replaceCard(Row *row, int position);
int core(void *arg, char *msg);
void payrReserved(Player *player, int position);
int decodeMsg(Context *ctx, char *msg);
char *input(Context *ctx);
#endif
