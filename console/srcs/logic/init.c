#include "../includes/splendor.h"
#include <time.h>

# define TOP_ROW_COUNT 10
# define MID_ROW_COUNT 10
# define BOT_ROW_COUNT 10

# define ROW_CARD_COUNT 4

# define UI_PLAYER_LEFT 0
# define UI_BOARD 1
# define UI_PLAYER_RIGHT 2

# define TOKEN_ROW 0
# define CARD_ROW 1
# define TITLE_ROW 2
# define TOP_CARD_ROW 0
# define MID_CARD_ROW 1
# define BOT_CARD_ROW 2


Context *init()
{
	Context *ctx;
	SDL_Surface *surf;

	srand(time(NULL));
	SDLX_InitDefault();

	ctx = SDL_calloc(1, sizeof(Context));
	init_graphics(ctx);
	init_connectScreen(ctx);
	ctx->state = TITLE;
	ctx->playerCount = 0;




	return ctx;
}

void initNewGame(Context *ctx)
{
	SDLX_RectContainer *root;

	root = loadConfig("assets/UIconfig");


	initBoard (ctx, &root->containers[UI_BOARD]);

	SDL_Log("Board was init");
	if (ctx->players[0].status == READY)
		initPlayer(ctx, 0, &root->containers[UI_PLAYER_LEFT].containers[0]);
	if (ctx->players[1].status == READY)
		initPlayer(ctx, 1, &root->containers[UI_PLAYER_RIGHT].containers[0]);
	if (ctx->players[2].status == READY)
		initPlayer(ctx, 2, &root->containers[UI_PLAYER_LEFT].containers[1]);
	if (ctx->players[3].status == READY)
		initPlayer(ctx, 3, &root->containers[UI_PLAYER_RIGHT].containers[1]);

	SDL_SetRenderTarget(ctx->display->renderer, NULL);
	SDL_SetRenderDrawBlendMode(ctx->display->renderer, SDL_BLENDMODE_BLEND);
	cleanupUIConfig(root);
	SDL_free(root);
}

void initBoard(Context *ctx, SDLX_RectContainer *root)
{
	Potion *cards;
	SDL_Rect src;

	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;

	initRowPotions(ctx, &root->containers[CARD_ROW].containers[TOP_CARD_ROW], TOP_ROW);
	initRowPotions(ctx, &root->containers[CARD_ROW].containers[MID_CARD_ROW], MID_ROW);
	initRowPotions(ctx, &root->containers[CARD_ROW].containers[BOT_CARD_ROW], BOT_ROW);

	ctx->board.tokens[ESSENCE_TYPES - 1] = 5;
	for (int i = 0; i < MAX_MASTER_POTIONS; i++)
	{
		SDLX_SpriteCreate(&ctx->board.titleUI[i], 1, NULL);
		ctx->board.titleUI[i]._dst = root->containers[TITLE_ROW].elems[i]._boundingBox;
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

void initRowPotions(Context *ctx, SDLX_RectContainer *container, int level)
{
	Row *row;

	row = &ctx->board.rows[level];
	row->recipeCount = MAX_ROWCARD;
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		generatePotion(ctx->assets.texPotions, &row->recipes[i], level);
	}

}

void initPlayer(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	int i;

	SDL_Rect src;

	memset(ctx->players[id].owned, 0, 5 * sizeof(uint8_t));
	memset(ctx->players[id].tokens, 0, 5 *sizeof(uint8_t));
	ctx->players[id].status = DISCONNECTED;
	ctx->players[id].reserveCount = 0;


	// ctx->players[id].pointSprite._dst.h = ctx->numbers.h;


	// TTF_SizeText(ctx->display->defaultFont, name, &root->containers[0].elems[0].boundingBox->w, &root->containers[0].elems[0].boundingBox->h);
	// TTF_SizeText(ctx->display->defaultFont, "POINTS : ", &root->containers[0].elems[1].boundingBox->w, &root->containers[0].elems[1].boundingBox->h);


	for (i = 0; i < POTION_TYPES; i++)
	{
		ctx->players[id].owned[i] = 0;
		ctx->players[id].tokens[i] = 0;

		SDLX_SpriteCreate(&ctx->players[id].ressources[i], 1,  ctx->textSheet.tex);
		SDLX_SpriteCreate(&ctx->players[id].permanents[i], 1,  ctx->textSheet.tex);
		ctx->players[id].ressources[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;
		ctx->players[id].permanents[i]._dst = root->containers[1].containers[i].elems[1]._boundingBox;
		ctx->players[id].ressources[i]._src = ctx->numbers;
		ctx->players[id].permanents[i]._src = ctx->numbers;
		get_img_src(&src, TOK_RECT, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->Tcards, &src,  root->containers[1].containers[i].elems[0].boundingBox);
		get_img_src(&src, TOK_HEX, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->Tcards, &src,  root->containers[1].containers[i].elems[1].boundingBox);
	}

	// src = (SDL_Rect){.h = 53, .w = CARD_W / 2,
	// 					 .x = (SEP_X + 5) + (CARD_W / 2 + SEP_X) * i, .y =  (CARD_H * 2) + SEP_Y * 3 + 35};
	// SDLX_SpriteCreate(&ctx->players[id].ressources[i], 1, ctx->textSheet.tex);
	// ctx->players[id].ressources[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;
	// ctx->players[id].ressources[i]._src = ctx->numbers;
	// SDL_RenderCopy(ctx->display->renderer, ctx->Tcards, &src,  root->containers[1].containers[i].elems[0].boundingBox);

	for (i = 0; i < MAX_RESERVE; i++)
	{
		SDLX_SpriteCreate(&ctx->players[id].reserved[i].sprite, 1, ctx->Tcards);
		ctx->players[id].reserved[i].sprite._dst = root->containers[2].elems[i]._boundingBox;
		ctx->players[id].reserved[i].sprite.src = NULL;
		ctx->players[id].reserved[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctx->board.rows[0].revealed[0].sprite._dst.w, ctx->board.rows[0].revealed[0].sprite._dst.h);
		SDL_SetTextureBlendMode(ctx->players[id].reserved[i].sprite.texture , SDL_BLENDMODE_BLEND);
	}
}
# define START 3000
void init_connectScreen(Context *ctx)
{
	SDLX_RectContainer *root;
	SDL_Surface *surf;

	root = loadConfig("assets/startUI");

	// ctx->connectscreen.buttons = IMG_Load("assets/buttons.png");
	SDL_Log("Container %d", root->containerCount);
	ctx->connectscreen.counter = START;
	surf = IMG_Load("assets/PMTitle.png");
	ctx->display->background = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	ctx->display->bgColor = (SDL_Color){54, 60, 66,255};
	SDL_FreeSurface(surf);

	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[0], 1 , NULL);
	ctx->connectscreen.playerSprites[0]._dst = root->containers[0].elems[0]._boundingBox;
	ctx->connectscreen.playerSprites[0]._src = (SDL_Rect){.x = 0, .y = 0, .w = 500, .h = 700};
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[1], 1 , NULL);
	ctx->connectscreen.playerSprites[1]._dst = root->containers[0].elems[1]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[2], 1 , NULL);
	ctx->connectscreen.playerSprites[2]._dst = root->containers[1].elems[0]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[3], 1 , NULL);
	ctx->connectscreen.playerSprites[3]._dst = root->containers[1].elems[1]._boundingBox;
}
