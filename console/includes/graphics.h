#include "splendor_structs.h"

// ~-~-~-~-~-~-~- config_parser.c ~-~-~-~-~-~-~

SDLX_RectContainer *parse_UIConfig(char *filename);

// ~-~-~-~-~-~-~-~-~- utils.c ~-~-~-~-~-~-~~-~-~-~-
void print_config	(Context *ctx, SDLX_RectContainer *root);
void cleanupUIConfig(SDLX_RectContainer *container);

// ~-~-~-~-~-~-~- render.c ~-~-~-~-~-~-~-

void render_connect_screen(Context *ctx);
void renderPlayer(Context *ctx, Player *player);
void renderBoard (Context *ctx);
