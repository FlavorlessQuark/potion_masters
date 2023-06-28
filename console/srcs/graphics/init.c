#include "graphics.h"

# define UI_PLAYER_LEFT 0
# define UI_BOARD 1
# define UI_PLAYER_RIGHT 2

void prepare_textures(Context * ctx);
void init_connect_screen_UI(Context *ctx, SDLX_RectContainer *root);
void init_board_UI(Context *ctx, SDLX_RectContainer *container);
void init_row_UI(Row *row, SDLX_RectContainer *container);
void init_player_UI(Context *ctx, uint8_t id, SDLX_RectContainer *root);

void init_UI(Context *ctx)
{
	SDLX_InitDefault();
	SDLX_RectContainer *board_root;
	SDLX_RectContainer *connect_root;


	ctx->display = SDLX_DisplayGet();
	board_root = loadConfig("assets/UI/boardUI");
	connect_root = loadConfig("assets/UI/menuUI");


	prepare_textures(ctx);
	init_connect_screen_UI(ctx, &connect_root->containers[0].containers[0]);
	init_board_UI(ctx, &board_root->containers[UI_BOARD]);
	init_player_UI(ctx, 0, &board_root->containers[UI_PLAYER_LEFT].containers[0]);
	init_player_UI(ctx, 1, &board_root->containers[UI_PLAYER_RIGHT].containers[0]);
	init_player_UI(ctx, 2, &board_root->containers[UI_PLAYER_LEFT].containers[1]);
	init_player_UI(ctx, 3, &board_root->containers[UI_PLAYER_RIGHT].containers[1]);

	SDL_Log("Finished Init");
}

void prepare_textures(Context * ctx)
{
	SDL_Surface *surf;

	SDL_Log("Preparing textures...");
	surf = IMG_Load("assets/Textures/bg_placeholder.jpg");
	ctx->assets.connectBg = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/Textures/bg_placeholder.jpg");
	ctx->assets.mainBg = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/Textures/cards.png");
	ctx->assets.texPotions = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/Textures/buttons.png");
	ctx->assets.texUI = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);


	ctx->display->defaultFont = TTF_OpenFont("assets/underwood.ttf", 40);
	SDLX_TextSheet_Create(&ctx->assets.text, ctx->display->win_w, ctx->display->win_h);
	ctx->assets.textSrc = SDLX_TextSheet_Add(
							&ctx->assets.text,
							"0123456789",
							ctx->display->defaultFont,
							(SDL_Color){255, 255, 255, 255});
	TTF_SizeText(ctx->display->defaultFont, "0", &ctx->assets.textSrc.w, &ctx->assets.textSrc.h);


	ctx->display->background = SDL_CreateTexture(
			ctx->display->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			ctx->display->win_w, ctx->display->win_h
			);
	SDL_Log("Done with textures");
}

void init_connect_screen_UI(Context *ctx, SDLX_RectContainer *root)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		SDLX_SpriteCreate(&ctx->connectscreen.playerSprites[i], 1, ctx->assets.mainBg);
		SDLX_SpriteCreate(&ctx->connectscreen.playerStatus[i], 1, ctx->assets.mainBg);
		ctx->connectscreen.playerSprites[i]._dst = root->containers[i].elems[0]._boundingBox;
		ctx->connectscreen.playerStatus[i]._dst = root->containers[i].elems[1]._boundingBox;

		ctx->connectscreen.playerSprites[i].src = NULL;
		ctx->connectscreen.playerStatus[i].src = NULL;
	}
}

void init_board_UI(Context *ctx, SDLX_RectContainer *container)
{
	for (int i = 0; i < MAX_MASTER_POTIONS; i++)
	{
		SDLX_SpriteCreate(&ctx->board.titles[i].sprite, 1, ctx->assets.texPotions);
		ctx->board.titles[i].sprite._dst = container->containers[0].elems[i]._boundingBox;
		ctx->board.titles[i].sprite.src = NULL;
	}
	for (int i = 0; i < ROW_COUNT; i++)
	{
		init_row_UI(&ctx->board.rows[i], &container->containers[i + 1]);
	}
}

void init_row_UI(Row *row, SDLX_RectContainer *container)
{

	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		SDLX_SpriteCreate(&row->recipes[i].sprite, 1, NULL);
		row->recipes[i].sprite._dst = container->elems[i]._boundingBox;
		row->recipes[i].sprite.src = NULL;
		row->recipes[i].sprite.texture = SDL_CreateTexture(
			SDLX_DisplayGet()->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			row->recipes[i].sprite._dst.w,
			row->recipes[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->recipes[i].sprite.texture , SDL_BLENDMODE_BLEND);
	}

}

void init_player_UI(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	SDLX_SpriteCreate(&ctx->players[id].pointSprite, 1,  ctx->assets.text.tex);

	ctx->players[id].pointSprite._src    = ctx->assets.textSrc;
	ctx->players[id].pointsTag 			 = root->containers[0].elems[0]._boundingBox;
	ctx->players[id].pointSprite._dst    = root->containers[0].elems[1]._boundingBox;
	ctx->players[id].pointSprite._dst.w  = ctx->players[id].pointSprite._dst.h;

	for (int i = 0; i < MAX_POTIONS; i++)
	{
		SDLX_SpriteCreate(&ctx->players[id].owned[i].sprite, 1, ctx->assets.mainBg);
		ctx->players[id].owned[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctx->board.rows[0].recipes[0].sprite._dst.w, ctx->board.rows[0].recipes[0].sprite._dst.h);
		SDL_SetTextureBlendMode(ctx->players[id].owned[i].sprite.texture , SDL_BLENDMODE_BLEND);
	}
	for (int i = 0; i < ESSENCE_TYPES; i++)
	{
		SDLX_SpriteCreate(&ctx->players[id].ressources[i], 1, ctx->assets.text.tex);
		ctx->players[id].ressources[i]._dst = root->containers[1].containers[id % 2].elems[i]._boundingBox;
		ctx->players[id].ressources[i]._src = ctx->assets.textSrc;
	}
}
