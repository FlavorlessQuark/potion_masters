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


void initPlayers(Context *ctx)
{
	// card->alignment = SDLX
	char *handles;
	int offset;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		memset(ctx->players[i].owned, 0, 5 * sizeof(uint8_t));
		memset(ctx->players[i].tokens, 0, 5 *sizeof(uint8_t));
		ctx->players[i].status = DISCONNECTED;
		ctx->players[i].reserveCount = 0;
	}
}

void initRowCards(Row *row, SDLX_RectContainer *container, SDL_Texture *tex, int level)
{
	//Parse file data here
	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon._dst = container->elems[0]._boundingBox;
	row->rowIcon.texture = tex;
	row->rowIcon.src->x = SEP_X;
	row->rowIcon.src->y = SEP_Y;
	row->rowIcon.src->h = CARD_H;
	row->rowIcon.src->w = CARD_W;

	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		SDLX_SpriteCreate(&row->revealed[i].sprite, 1, NULL);
		row->revealed[i].sprite._dst = container->elems[i + 1]._boundingBox;
		row->revealed[i].sprite.texture = tex;
		row->revealed[i].sprite._src.h = CARD_H;
		row->revealed[i].sprite._src.w = CARD_W;
		generateCard(&row->revealed[i], level);
	}

}

void initCards(Context *ctx, SDLX_RectContainer *root)
{
	Card *cards;
	SDL_Surface *surf;
	SDL_Texture *tex;

	int yOff = 13;
	int xOff = 13;
	int height = 233;
	int width = 136;

	surf = IMG_Load("assets/cardData/Cards.png");
	tex = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);


	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;

	// Init textures here

	initRowCards(&ctx->board.rows[TOP_ROW], &root->containers[CARD_ROW].containers[TOP_CARD_ROW], tex, TOP_ROW);
	initRowCards(&ctx->board.rows[MID_ROW], &root->containers[CARD_ROW].containers[MID_CARD_ROW], tex, MID_ROW);
	initRowCards(&ctx->board.rows[BOT_ROW], &root->containers[CARD_ROW].containers[BOT_CARD_ROW], tex, BOT_ROW);


	for (int i = 0; i < TOK_COUNT; i++)
	{
		SDLX_SpriteCreate(&ctx->board.tokenUI[i], 1, NULL);
		ctx->board.tokenUI[i]._dst = root->containers[TOKEN_ROW].elems[i]._boundingBox;
		ctx->board.tokenUI[i].texture = tex;
		ctx->board.tokenUI[i]._src.h = 53;
		ctx->board.tokenUI[i]._src.w = CARD_W / 2;
		// ctx->board.tokenUI[i]._src.y = SEP_Y;
		ctx->board.tokenUI[i]._src.y = (CARD_H * 2) + SEP_Y * 3 + 35;
		ctx->board.tokenUI[i]._src.x = (SEP_X + 5) + (CARD_W / 2 + SEP_X) * i;
	}
	for (int i = 0; i < MAX_TITLES; i++)
	{
		SDLX_SpriteCreate(&ctx->board.titleUI[i], 1, NULL);
		ctx->board.titleUI[i]._dst = root->containers[TITLE_ROW].elems[i]._boundingBox;
	}
}

SDLX_RectContainer *initUI(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}

void initPlayerUI(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	int i;

	ctx->board.playerUI[id].nameTag = root->containers[0].elems[0]._boundingBox;
	ctx->board.playerUI[id].pointsTag = root->containers[0].elems[1]._boundingBox;

	for (i = 0; i < CARD_TYPES; i++)
	{
		SDLX_SpriteCreate(&ctx->board.playerUI[id].ressourceIcon[i], 1,  NULL);
		SDLX_SpriteCreate(&ctx->board.playerUI[id].permanentIcon[i], 1,  NULL);
		ctx->board.playerUI[id].ressourceIcon[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;
		ctx->board.playerUI[id].permanentIcon[i]._dst = root->containers[1].containers[i].elems[1]._boundingBox;
	}
	SDLX_SpriteCreate(&ctx->board.playerUI[id].ressourceIcon[i], 1, NULL);
	ctx->board.playerUI[id].ressourceIcon[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;

	for (i = 0; i < MAX_RESERVE; i++)
	{
		SDLX_SpriteCreate(&ctx->board.playerUI[id].reservedIcon[i], 1, NULL);
		ctx->board.playerUI[id].reservedIcon[i]._dst = root->containers[2].elems[i]._boundingBox;
	}
}

void print_config(Context *ctx, SDLX_RectContainer *root)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);
	SDL_RenderDrawRect(ctx->display->renderer, root->self.boundingBox);

	for (int i = 0; i < root->containerCount; i++)
		print_config(ctx, &root->containers[i]);

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 255,255);
	for (int x = 0; x < root->elemCount; x++)
	{
		SDL_RenderDrawRect(ctx->display->renderer, root->elems[x].boundingBox);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0,0,255);
}

void init_connectScreen(Context *ctx)
{
	SDLX_RectContainer *root;

	root = initUI("assets/startUI");

	SDL_Log("Container %d", root->containerCount);
	// ctx->connectscreen.status = root->containers[0].elems[0]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[0], 1 , NULL);
	ctx->connectscreen.playerSprites[0]._dst = root->containers[0].elems[0]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[1], 1 , NULL);
	ctx->connectscreen.playerSprites[1]._dst = root->containers[0].elems[1]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[2], 1 , NULL);
	ctx->connectscreen.playerSprites[2]._dst = root->containers[1].elems[0]._boundingBox;
	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[3], 1 , NULL);
	ctx->connectscreen.playerSprites[3]._dst = root->containers[1].elems[1]._boundingBox;
}

Context *init()
{
	Context *ctx;

	SDLX_RectContainer *root;

	srand(time(NULL));
	SDLX_InitDefault();

	ctx = SDL_calloc(1, sizeof(Context));
	ctx->display = SDLX_DisplayGet();
	ctx->board.remainingTitles = MAX_TITLES;
	ctx->state = CONNECT_SCREEN;
	ctx->playerCount = 0;
	root = initUI("assets/UIconfig");

	initPlayers(ctx);
	initCards(ctx, &root->containers[UI_BOARD]);

	initPlayerUI(ctx, 0, &root->containers[UI_PLAYER_LEFT].containers[0]);
	initPlayerUI(ctx, 1, &root->containers[UI_PLAYER_RIGHT].containers[0]);
	initPlayerUI(ctx, 2, &root->containers[UI_PLAYER_LEFT].containers[1]);
	initPlayerUI(ctx, 3, &root->containers[UI_PLAYER_RIGHT].containers[1]);
	init_connectScreen(ctx);

	cleanupUIConfig(root);
	SDL_free(root);
	return ctx;
}
