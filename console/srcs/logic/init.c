#include "../includes/splendor.h"
#include <time.h>

# define TOP_ROW 0
# define MID_ROW 1
# define BOT_ROW 2

# define TOP_ROW_COUNT 10
# define MID_ROW_COUNT 10
# define BOT_ROW_COUNT 0

# define ROW_CARD_COUNT 4

void init_new_game(Context *ctx);
void fill_board(Context *ctx);
void fill_player(Context *ctx, uint8_t id);

Context *init()
{
	Context *ctx;
	SDL_Surface *surf;

	srand(time(NULL));

	ctx = SDL_calloc(1, sizeof(Context));
	init_UI(ctx);
	init_connectScreen(ctx);
	init_new_game(ctx);
	// ctx->state = CONNECT_SCREEN;
	ctx->state = PLAYING;
	ctx->playerCount = 0;

	return ctx;
}

void init_new_game(Context *ctx)
{
	SDLX_RenderResetColour(ctx->display);
	SDL_SetRenderTarget(ctx->display->renderer, ctx->display->background);
	SDL_SetTextureBlendMode(ctx->display->background , SDL_BLENDMODE_BLEND);

	fill_board(ctx);

	// if (ctx->players[0].status == READY)
		fill_player(ctx, 0);
	// if (ctx->players[1].status == READY)
		fill_player(ctx, 1);
	// if (ctx->players[2].status == READY)
		fill_player(ctx, 2);
	// if (ctx->players[3].status == READY)
		fill_player(ctx, 3);

	SDL_Log("Init new game");
	SDL_SetRenderTarget(ctx->display->renderer, NULL);
}

void initRowPotions(Context *ctx, int level)
{
	Row *row;

	row = &ctx->board.rows[level];
	row->recipeCount = MAX_ROWCARD;
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		generatePotion(ctx->assets.texPotions, &row->recipes[i], level);
	}

}

void fill_board(Context *ctx)
{
	SDL_Rect src;

	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;

	ctx->board.rows[TOP_ROW].recipeCount = ROW_CARD_COUNT;
	ctx->board.rows[MID_ROW].recipeCount = ROW_CARD_COUNT;
	ctx->board.rows[BOT_ROW].recipeCount = ROW_CARD_COUNT;

	for (int i = 0; i < ROW_COUNT; i++)
		initRowPotions(ctx, i);
}



void fill_player(Context *ctx, uint8_t id)
{
	int i;

	SDL_Rect src;
	char name[9] = {'P', 'L', 'A', 'Y', 'E', 'R', ' ', (id + 1) + '0', '\0'};

	memset(ctx->players[id].owned, 0, 5 * sizeof(uint8_t));
	memset(ctx->players[id].tokens, 0, 5 *sizeof(uint8_t));
	ctx->players[id].potionCount = 0;
	ctx->players[id].status = DISCONNECTED;
	ctx->players[id].points = 0;

	// TTF_SizeText(ctx->display->defaultFont, name, &ctx->players[id].pointsTag.w, &ctx->players[id].pointsTag.h);

	SDLX_RenderMessage(ctx->display, &ctx->players[id].pointsTag, (SDL_Color){255,255,255,255}, name);
	// SDLX_RenderMessage(ctx->display,ctx->players[id].pointSprite., (SDL_Color){255,255,255,255}, "POINTS : ");
	ctx->players[id].pointSprite._dst.h = ctx->assets.textSrc.h;

	// src = (SDL_Rect){.h = 53, .w = CARD_W / 2,
	// 					 .x = (SEP_X + 5) + (CARD_W / 2 + SEP_X) * i, .y =  (CARD_H * 2) + SEP_Y * 3 + 35};
	// SDLX_SpriteCreate(&ctx->players[id].ressources[i], 1, ctx->textSheet.tex);
	// ctx->players[id].ressources[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;

	for (i = 0; i < ESSENCE_TYPES; i++)
	{
		ctx->players[id].tokens[i] = 0;
		SDL_RenderCopy(ctx->display->renderer, ctx->assets.mainBg, NULL,  ctx->players[id].ressources[i].dst);
		// TTF_SizeText(ctx->display->defaultFont, "00", &ctx->players[id].ressources[i]._dst.w, &ctx->players[id].ressources[i]._dst.h);
		ctx->players[id].ressources[i]._src = ctx->assets.textSrc;
	}
}

# define START 3000
void init_connectScreen(Context *ctx)
{
	ctx->connectscreen.counter = START;
}
