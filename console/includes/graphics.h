#include "splendor_structs.h"

// ~-~-~-~-~-~-~- UI/config_parser.c ~-~-~-~-~-~-~

SDLX_RectContainer *parse_UIConfig(char *filename);

// ~-~-~-~-~-~-~-~-~- UI/utils.c ~-~-~-~-~-~-~~-~-
void print_config	(SDL_Renderer *renderer, SDLX_RectContainer *root);
void cleanupUIConfig(SDLX_RectContainer *container);
SDLX_RectContainer *loadConfig(char *filename);

// ~-~-~-~-~-~-~-~-  render.c ~-~-~-~-~-~-~-~- -~-

void render_connect_screen(Context *ctx);
void renderPlayer(Context *ctx, Player *player);
void renderBoard (Context *ctx);

// ~-~-~-~-~-~-~-~-~- utils.c ~-~-~-~-~-~-~~-~-~-~

SDL_Rect scale_and_center(double scalar, SDL_Rect parent, SDL_Rect this);
void overlay_text(SDL_Texture *dest, SDL_Texture *base, SDL_Rect *baseSrc,  uint32_t color, char *text);
void generatePotionTexture(SDL_Texture *base, Potion *card, int type);
SDL_Texture *create_target_texture(int w, int h);
// ~-~-~-~-~-~-~-~-~- graphics.c ~-~-~-~-~-~-~~-~-~-~
void init_UI(Context *ctx);
