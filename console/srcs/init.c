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
	ctx->display = SDLX_DisplayGet();
	init_connectScreen(ctx);
	ctx->board.remainingTitles = MAX_TITLES;
	ctx->state = TITLE;
	ctx->playerCount = 0;
	ctx->display->defaultFont = TTF_OpenFont("assets/underwood.ttf", 40);
	SDLX_TextSheet_Create(&ctx->textSheet, ctx->display->win_w, ctx->display->win_h);
	ctx->numbers = SDLX_TextSheet_Add(&ctx->textSheet, "0123456789101112131415", ctx->display->defaultFont, (SDL_Color){255, 255, 255, 255});

	TTF_SizeText(ctx->display->defaultFont, "0", &ctx->numbers.w, &ctx->numbers.h);

	surf = IMG_Load("assets/cards.png");
	ctx->Tcards = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);


	surf = IMG_Load("assets/buttons.png");
	ctx->Tbuttons = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);
	return ctx;
}

void initNewGame(Context *ctx)
{
	SDLX_RectContainer *root;

	root = loadConfig("assets/UIconfig");

	ctx->display->background = SDL_CreateTexture(
			ctx->display->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			ctx->display->win_w, ctx->display->win_h
			);

	SDLX_RenderResetColour(ctx->display);
	SDL_SetRenderTarget(ctx->display->renderer, ctx->display->background);
	SDL_SetTextureBlendMode(ctx->display->background , SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(ctx->display->renderer, NULL, NULL, NULL);
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
	Card *cards;
	SDL_Rect src;

	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;

	initRowCards(ctx, &root->containers[CARD_ROW].containers[TOP_CARD_ROW], TOP_ROW);
	initRowCards(ctx, &root->containers[CARD_ROW].containers[MID_CARD_ROW], MID_ROW);
	initRowCards(ctx, &root->containers[CARD_ROW].containers[BOT_CARD_ROW], BOT_ROW);


	for (int i = 0; i < TOK_COUNT; i++)
	{
		ctx->board.tokens[i] = 7;
		SDLX_SpriteCreate(&ctx->board.tokenUI[i], 1, NULL);
		ctx->board.tokenUI[i]._dst = root->containers[TOKEN_ROW].elems[i]._boundingBox;
		ctx->board.tokenUI[i].texture = ctx->Tcards;
		get_img_src(&src, TOK_HEX, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->Tcards, &src, root->containers[TOKEN_ROW].elems[i].boundingBox);

		ctx->board.tokenUI[i]._src = ctx->numbers;
		ctx->board.tokenUI[i]._dst.w /= 2;
		ctx->board.tokenUI[i]._dst.h /= 2;
		ctx->board.tokenUI[i]._dst.x += ctx->board.tokenUI[i]._dst.w / 2;
		ctx->board.tokenUI[i]._dst.y += ctx->board.tokenUI[i]._dst.h / 2;
		ctx->board.tokenUI[i].texture = ctx->textSheet.tex;
	}
	ctx->board.tokens[TOK_COUNT - 1] = 5;
	for (int i = 0; i < MAX_TITLES; i++)
	{
		SDLX_SpriteCreate(&ctx->board.titleUI[i], 1, NULL);
		ctx->board.titleUI[i]._dst = root->containers[TITLE_ROW].elems[i]._boundingBox;
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

void initRowCards(Context *ctx, SDLX_RectContainer *container, int level)
{
	Row *row;

	row = &ctx->board.rows[level];

	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon._dst = container->elems[0]._boundingBox;
	row->rowIcon.texture = ctx->Tcards;
	row->revealedCount = MAX_ROWCARD;
	get_img_src(&row->rowIcon._src, CARD_BACK, level);
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		SDLX_SpriteCreate(&row->revealed[i].sprite, 1, NULL);
		row->revealed[i].sprite._dst = container->elems[i + 1]._boundingBox;
		row->revealed[i].sprite.src = NULL;
		row->revealed[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, row->revealed[i].sprite._dst.w, row->revealed[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->revealed[i].sprite.texture , SDL_BLENDMODE_BLEND);
		generateCard(ctx->Tcards, &row->revealed[i], level);
	}

}

void initPlayer(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	int i;
	char name[9] = {'P', 'L', 'A', 'Y', 'E', 'R', ' ', (id + 1) + '0', '\0'};
	SDL_Rect src;

	memset(ctx->players[id].owned, 0, 5 * sizeof(uint8_t));
	memset(ctx->players[id].tokens, 0, 5 *sizeof(uint8_t));
	ctx->players[id].status = CONNECTED;
	ctx->players[id].reserveCount = 0;

	SDLX_SpriteCreate(&ctx->players[id].pointSprite, 1,  ctx->textSheet.tex);

	ctx->players[id].pointsTag = root->containers[0].elems[1]._boundingBox;
	ctx->players[id].pointSprite._dst = root->containers[0].elems[1]._boundingBox;
	ctx->players[id].pointSprite._dst.x += root->containers[0].elems[1]._boundingBox.w;
	ctx->players[id].pointSprite._dst.w = ctx->players[id].pointSprite._dst.h;
	// ctx->players[id].pointSprite._dst.h = ctx->numbers.h;
	ctx->players[id].pointSprite._src = ctx->numbers;

	// TTF_SizeText(ctx->display->defaultFont, name, &root->containers[0].elems[0].boundingBox->w, &root->containers[0].elems[0].boundingBox->h);
	// TTF_SizeText(ctx->display->defaultFont, "POINTS : ", &root->containers[0].elems[1].boundingBox->w, &root->containers[0].elems[1].boundingBox->h);
	SDLX_RenderMessage(ctx->display, root->containers[0].elems[0].boundingBox, (SDL_Color){255,255,255,255}, name);
	SDLX_RenderMessage(ctx->display, root->containers[0].elems[1].boundingBox, (SDL_Color){255,255,255,255}, "POINTS : ");

	for (i = 0; i < CARD_TYPES; i++)
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
# define OFFSET 40
# define HEIGHT 640
# define WIDTH 600
# define RATIO (HEIGHT / WIDTH)
void init_connectScreen(Context *ctx)
{
	SDLX_RectContainer *root;
	SDL_Surface *surf;
	SDL_Texture *texture;

	root = loadConfig("assets/startUI");

	// ctx->connectscreen.buttons = IMG_Load("assets/buttons.png");
	SDL_Log("Container %d", root->containerCount);
	ctx->connectscreen.counter = START;
	surf = IMG_Load("assets/PMTitle.png");
	ctx->display->background = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);
	surf = IMG_Load("assets/buttons.png");
	texture = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);
	ctx->display->bgColor = (SDL_Color){54, 60, 66,255};

	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[0], 1 , texture);
	ctx->connectscreen.playerSprites[0]._dst = root->containers[0].elems[0]._boundingBox;
	ctx->connectscreen.playerSprites[0]._src = (SDL_Rect){.x = OFFSET, .y = OFFSET, .w = WIDTH, .h = HEIGHT};

	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[1], 1 , texture);
	ctx->connectscreen.playerSprites[1]._dst = root->containers[0].elems[1]._boundingBox;
	ctx->connectscreen.playerSprites[1]._src = (SDL_Rect){.x = OFFSET, .y = OFFSET, .w = WIDTH, .h = HEIGHT};

	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[2], 1 , texture);
	ctx->connectscreen.playerSprites[2]._dst = root->containers[1].elems[0]._boundingBox;
	ctx->connectscreen.playerSprites[2]._src = (SDL_Rect){.x = OFFSET, .y = OFFSET, .w = WIDTH, .h = HEIGHT};

	SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[3], 1 , texture);
	ctx->connectscreen.playerSprites[3]._dst = root->containers[1].elems[1]._boundingBox;
	ctx->connectscreen.playerSprites[3]._src = (SDL_Rect){.x = OFFSET, .y = OFFSET, .w = WIDTH, .h = HEIGHT};
}
