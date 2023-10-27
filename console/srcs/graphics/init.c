#include "graphics.h"

# define UI_PLAYER_LEFT 0
# define UI_BOARD 1
# define UI_PLAYER_RIGHT 2

void prepare_textures(Context * ctx);
void init_connect_screen_UI(Context *ctx, SDLX_RectContainer *root);
void init_board_UI(Context *ctx, SDLX_RectContainer *container);
void init_row_UI(Row *row, SDLX_RectContainer *container);
void init_left_player_UI(Context *ctx, uint8_t id, SDLX_RectContainer *root);
void init_right_player_UI(Context *ctx, uint8_t id, SDLX_RectContainer *root);

void init_UI(Context *ctx)
{
	SDLX_InitDefault();
	SDLX_RectContainer *board_root;
	SDLX_RectContainer *connect_root;


	ctx->display = SDLX_DisplayGet();
	board_root = loadConfig("assets/UI/boardUI");
	connect_root = loadConfig("assets/UI/menuUI");


	prepare_textures(ctx);

	SDL_Log("Init connect screen...");
	init_connect_screen_UI(ctx, &connect_root->containers[0].containers[0]);

	SDL_Log("Init board...");
	init_board_UI(ctx, &board_root->containers[UI_BOARD]);
	SDL_Log("Init P1...");
	init_left_player_UI(ctx, 0, &board_root->containers[UI_PLAYER_LEFT].containers[0]);
	SDL_Log("Init P2...");
	init_right_player_UI(ctx, 1, &board_root->containers[UI_PLAYER_RIGHT].containers[0]);
	SDL_Log("Init P3...");
	init_left_player_UI(ctx, 2, &board_root->containers[UI_PLAYER_LEFT].containers[1]);
	SDL_Log("Init P4...");
	init_right_player_UI(ctx, 3, &board_root->containers[UI_PLAYER_RIGHT].containers[1]);

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

	surf = IMG_Load("assets/Textures/ui_p1.png");
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
	SDL_Rect src = {.x = 625, .y = 470, .w = 500, .h = 187};


	SDLX_SpriteCreate(&ctx->connectscreen.bg, 1, ctx->assets.texUI);
	SDLX_SpriteCreate(&ctx->connectscreen.timer, 1, create_target_texture(ctx->display->win_h *0.6,ctx->display->win_h / 4));
	ctx->connectscreen.timer._dst = (SDL_Rect){
		.x = 0,
		.y = ctx->display->win_h / 4,
		.w = ctx->display->win_h *0.6,
		.h = ctx->display->win_h / 3};
	ctx->connectscreen.timer.src = NULL;
	ctx->connectscreen.bg._src = (SDL_Rect){.x = 0, .y = 0, .w = 470, .h = 470};
	ctx->connectscreen.bg._dst = root->self._boundingBox;
	root = &root->containers[0].containers[0];
	for (int i = 0; i < MAX_PLAYERS; i++)
	{

		SDLX_SpriteCreate(&ctx->connectscreen.playerName[i], 1,
			create_target_texture(500, 187)
			);
		SDLX_SpriteCreate(&ctx->connectscreen.playerStatus[i], 1, ctx->assets.texUI);
		ctx->connectscreen.playerStatus[i]._dst = root->containers[i].elems[0]._boundingBox;
		ctx->connectscreen.playerName[i]._dst = root->containers[i].elems[1]._boundingBox;

		overlay_text(ctx->connectscreen.playerName[i].texture,ctx->assets.texUI, &src, 0x000000FF,"");
		ctx->connectscreen.playerName[i].src = NULL;
		ctx->connectscreen.playerStatus[i]._src = (SDL_Rect){.x = 490, .y = 155, .w = 150, .h = 180};
		// ctx->connectscreen.playerName[i].src = NULL;
		// ctx->connectscreen.playerStatus[i].src = NULL;
	}
}

void init_board_UI(Context *ctx, SDLX_RectContainer *container)
{
	for (int i = 0; i < MAX_MASTER_POTIONS; i++)
	{
		SDLX_SpriteCreate(&ctx->board.titles[i].sprite, 1,  create_target_texture(
			container->containers[0].elems[i]._boundingBox.w,
			container->containers[0].elems[i]._boundingBox.h));
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
		row->recipes[i].sprite.texture = create_target_texture(container->elems[i]._boundingBox.w, container->elems[i]._boundingBox.h);
	}

}

void init_left_player_UI(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	char name[10] = {"Player 0"};
	SDL_Rect src = {.x = 625, .y = 470, .w = 500, .h = 187};

	name[7] = id + 1 + '0';

	// SDLX_SpriteCreate(&ctx->players[id].brewing.sprite, 1,
	for (int i = 0; i < 3; ++i)
	{
		for (int n = 0; n < 3; ++n)
		{
			SDLX_SpriteCreate(&ctx->players[id].owned[i * (3) + n].sprite, 1, create_target_texture(
				root->containers[1].containers[1].containers[0].containers[i].elems[n]._boundingBox.w,
				root->containers[1].containers[1].containers[0].containers[i].elems[n]._boundingBox.h
				)
			);
			overlay_text(ctx->players[id].owned[i * (3) + n].sprite.texture, NULL, NULL, 0xFFFFFFFF,"none");
			ctx->players[id].owned[i * (3) + n].sprite._dst = root->containers[1].containers[1].containers[0].containers[i].elems[n]._boundingBox;
			ctx->players[id].owned[i * (3) + n].sprite.src = NULL;
		}
	}
	name[7] = id + 1 + '0';
	SDLX_SpriteCreate(&ctx->players[id].name, 1, create_target_texture(
		root->containers[0].elems[0]._boundingBox.w,
		root->containers[0].elems[0]._boundingBox.h
		));
	overlay_text(ctx->players[id].name.texture, ctx->assets.texUI, &src, 0x000000FF,name);
	ctx->players[id].name._dst = root->containers[0].elems[0]._boundingBox;
	ctx->players[id].name.src = NULL;

	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->players[id].essences[i], 1, create_target_texture(
				root->containers[1].containers[0].elems[i]._boundingBox.w,
				root->containers[1].containers[0].elems[i]._boundingBox.h
			));
		// overlay_text(ctx->players[id].essences[i].texture,NULL, NULL, ((0xFF000000 >> (5 * i)) + 0xFF),"0");
		ctx->players[id].essences[i]._dst = root->containers[1].containers[0].elems[i]._boundingBox;
		ctx->players[id].essences[i].src = NULL;
	}
	SDL_Log("Done!");
}


void init_right_player_UI(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{
	char name[10] = {"Player 0"};
	SDL_Rect src = {.x = 625, .y = 470, .w = 500, .h = 187};

	name[7] = id + 1 + '0';

	// SDLX_SpriteCreate(&ctx->players[id].brewing.sprite, 1,
	// );

	for (int i = 0; i < 3; ++i)
	{
		for (int n = 0; n < 3; ++n)
		{
			SDLX_SpriteCreate(&ctx->players[id].owned[i * (3) + n].sprite, 1, create_target_texture(
				root->containers[1].containers[0].containers[0].containers[i].elems[n]._boundingBox.w,
				root->containers[1].containers[0].containers[0].containers[i].elems[n]._boundingBox.h
				)
			);
			overlay_text(ctx->players[id].owned[i * (3) + n].sprite.texture, NULL, NULL, 0xFFFFFFFF,"none");
			ctx->players[id].owned[i * (3) + n].sprite._dst = root->containers[1].containers[0].containers[0].containers[i].elems[n]._boundingBox;
			ctx->players[id].owned[i * (3) + n].sprite.src = NULL;
		}
	}
	SDLX_SpriteCreate(&ctx->players[id].name, 1, create_target_texture(
		root->containers[0].elems[1]._boundingBox.w,
		root->containers[0].elems[1]._boundingBox.h
		));
	overlay_text(ctx->players[id].name.texture, ctx->assets.texUI, &src, 0x000000FF,name);
	ctx->players[id].name._dst = root->containers[0].elems[1]._boundingBox;
	ctx->players[id].name.src = NULL;

	// SDL_Log("A");
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->players[id].essences[i], 1, create_target_texture(
				root->containers[1].containers[1].elems[i]._boundingBox.w,
				root->containers[1].containers[1].elems[i]._boundingBox.h
			));
		overlay_text(ctx->players[id].essences[i].texture,NULL, NULL, ((0xFF000000 >> (5 * i)) + 0xFF),"0");
		ctx->players[id].essences[i]._dst = 	root->containers[1].containers[1].elems[i]._boundingBox;
		ctx->players[id].essences[i].src = NULL;
	}
	SDL_Log("Done!");
}
