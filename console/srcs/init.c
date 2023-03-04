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
	SDLX_RectContainer *root;

	srand(time(NULL));
	SDLX_InitDefault();

	ctx = SDL_calloc(1, sizeof(Context));
	ctx->display = SDLX_DisplayGet();
	ctx->board.remainingTitles = MAX_TITLES;
	ctx->state = CONNECT_SCREEN;
	ctx->playerCount = 0;

	surf = IMG_Load("assets/cards.png");
	ctx->Tcards = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/buttons.png");
	ctx->Tbuttons = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	root = loadConfig("assets/UIconfig");
	init_connectScreen(ctx);
	initBoard (ctx, &root->containers[UI_BOARD]);
	initPlayer(ctx, 0, &root->containers[UI_PLAYER_LEFT].containers[0]);
	initPlayer(ctx, 1, &root->containers[UI_PLAYER_RIGHT].containers[0]);
	initPlayer(ctx, 2, &root->containers[UI_PLAYER_LEFT].containers[1]);
	initPlayer(ctx, 3, &root->containers[UI_PLAYER_RIGHT].containers[1]);

	cleanupUIConfig(root);
	SDL_free(root);
	return ctx;
}


void initBoard(Context *ctx, SDLX_RectContainer *root)
{
	Card *cards;

	int yOff = 13;
	int xOff = 13;
	int height = 233;
	int width = 136;

	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;

	initRowCards(ctx, &root->containers[CARD_ROW].containers[TOP_CARD_ROW], TOP_ROW);
	initRowCards(ctx, &root->containers[CARD_ROW].containers[MID_CARD_ROW], MID_ROW);
	initRowCards(ctx, &root->containers[CARD_ROW].containers[BOT_CARD_ROW], BOT_ROW);


	for (int i = 0; i < TOK_COUNT; i++)
	{
		SDLX_SpriteCreate(&ctx->board.tokenUI[i], 1, NULL);
		ctx->board.tokenUI[i]._dst = root->containers[TOKEN_ROW].elems[i]._boundingBox;
		ctx->board.tokenUI[i].texture = ctx->Tcards;
		ctx->board.tokenUI[i]._src.h = 53;
		ctx->board.tokenUI[i]._src.w = CARD_W / 2;
		ctx->board.tokenUI[i]._src.y = (CARD_H * 2) + SEP_Y * 3 + 35;
		ctx->board.tokenUI[i]._src.x = (SEP_X + 5) + (CARD_W / 2 + SEP_X) * i;
	}
	for (int i = 0; i < MAX_TITLES; i++)
	{
		SDLX_SpriteCreate(&ctx->board.titleUI[i], 1, NULL);
		ctx->board.titleUI[i]._dst = root->containers[TITLE_ROW].elems[i]._boundingBox;
	}
}

void initRowCards(Context *ctx, SDLX_RectContainer *container, int level)
{
	Row *row;

	row = &ctx->board.rows[level];

	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon._dst = container->elems[0]._boundingBox;
	row->rowIcon.texture = ctx->Tcards;
	row->rowIcon.src->x = SEP_X;
	row->rowIcon.src->y = SEP_Y;
	row->rowIcon.src->h = CARD_H;
	row->rowIcon.src->w = CARD_W;

	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		SDLX_SpriteCreate(&row->revealed[i].sprite, 1, NULL);
		row->revealed[i].sprite._dst = container->elems[i + 1]._boundingBox;
		row->revealed[i].sprite.texture = ctx->Tcards;
		row->revealed[i].sprite._src.h = CARD_H;
		row->revealed[i].sprite._src.w = CARD_W;
		generateCard(&row->revealed[i], level);
	}

}

void initPlayer(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	int i;

	memset(ctx->players[id].owned, 0, 5 * sizeof(uint8_t));
	memset(ctx->players[id].tokens, 0, 5 *sizeof(uint8_t));
	ctx->players[id].status = DISCONNECTED;
	ctx->players[id].reserveCount = 0;

	ctx->players[id].nameTag = root->containers[0].elems[0]._boundingBox;
	ctx->players[id].pointsTag = root->containers[0].elems[1]._boundingBox;

	for (i = 0; i < CARD_TYPES; i++)
	{
		SDLX_SpriteCreate(&ctx->players[id].ressourceIcon[i], 1,  ctx->Tcards);
		SDLX_SpriteCreate(&ctx->players[id].permanentIcon[i], 1,  ctx->Tcards);
		ctx->players[id].ressourceIcon[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;
		ctx->players[id].permanentIcon[i]._dst = root->containers[1].containers[i].elems[1]._boundingBox;
	}
	SDLX_SpriteCreate(&ctx->players[id].ressourceIcon[i], 1, ctx->Tcards);
	ctx->players[id].ressourceIcon[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;

	for (i = 0; i < MAX_RESERVE; i++)
	{
		SDLX_SpriteCreate(&ctx->players[id].reserved[i].sprite, 1, ctx->Tcards);
		ctx->players[id].reserved[i].sprite._dst = root->containers[2].elems[i]._boundingBox;
	}
}

void init_connectScreen(Context *ctx)
{
	SDLX_RectContainer *root;

	root = loadConfig("assets/startUI");

	SDL_Log("Container %d", root->containerCount);
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[0], 1 , NULL);
	ctx->connectscreen.playerSprites[0]._dst = root->containers[0].elems[0]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[1], 1 , NULL);
	ctx->connectscreen.playerSprites[1]._dst = root->containers[0].elems[1]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[2], 1 , NULL);
	ctx->connectscreen.playerSprites[2]._dst = root->containers[1].elems[0]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[3], 1 , NULL);
	ctx->connectscreen.playerSprites[3]._dst = root->containers[1].elems[1]._boundingBox;
}
