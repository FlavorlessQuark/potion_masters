#include "graphics.h"

# define UI_PLAYER_LEFT 0
# define UI_BOARD 1
# define UI_PLAYER_RIGHT 2

void init_graphics(Context *ctx)
{
	SDLX_InitDefault();
	SDLX_RectContainer *root;


	ctx->display = SDLX_DisplayGet();
	root = loadConfig("assets/UIconfig");

	prepare_textures(ctx);
	init_board_graphics(ctx);
	if (ctx->players[0].status == READY)
		init_player_graphics(ctx, 0, &root->containers[UI_PLAYER_LEFT].containers[0]);
	if (ctx->players[1].status == READY)
		init_player_graphics(ctx, 1, &root->containers[UI_PLAYER_RIGHT].containers[0]);
	if (ctx->players[2].status == READY)
		init_player_graphics(ctx, 2, &root->containers[UI_PLAYER_LEFT].containers[1]);
	if (ctx->players[3].status == READY)
		init_player_graphics(ctx, 3, &root->containers[UI_PLAYER_RIGHT].containers[1]);
}

void prepare_textures(Context * ctx)
{
	SDL_Surface *surf;

	surf = IMG_Load("assets/cards.png");
	ctx->assets.texPotions = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/buttons.png");
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
	SDLX_RenderResetColour(ctx->display);
	SDL_SetRenderTarget(ctx->display->renderer, ctx->display->background);
	SDL_SetTextureBlendMode(ctx->display->background , SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(ctx->display->renderer, NULL, NULL, NULL);
}

void init_board_graphics()
{

}

void init_row_graphics(Context *ctx, SDLX_RectContainer *container, uint8_t level)
{
	Row *row;

	row = &ctx->board.rows[level];
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		SDLX_SpriteCreate(&row->recipes[i].sprite, 1, NULL);
		row->recipes[i].sprite._dst = container->elems[i + 1]._boundingBox;
		row->recipes[i].sprite.src = NULL;
		row->recipes[i].sprite.texture = SDL_CreateTexture(
			ctx->display->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			row->recipes[i].sprite._dst.w,
			row->recipes[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->recipes[i].sprite.texture , SDL_BLENDMODE_BLEND);
	}

}

void init_player_graphics(Context *ctx, uint8_t id, SDLX_RectContainer *root)
{


	SDLX_SpriteCreate(&ctx->players[id].pointSprite, 1,  ctx->assets.text.tex);

	ctx->players[id].pointSprite._src    = ctx->assets.textSrc;
	ctx->players[id].pointsTag 			 = root->containers[0].elems[1]._boundingBox;
	ctx->players[id].pointSprite._dst    = root->containers[0].elems[1]._boundingBox;
	ctx->players[id].pointSprite._dst.x += root->containers[0].elems[1]._boundingBox.w;
	ctx->players[id].pointSprite._dst.w  = ctx->players[id].pointSprite._dst.h;

	char name[9] = {'P', 'L', 'A', 'Y', 'E', 'R', ' ', (id + 1) + '0', '\0'};
	SDLX_RenderMessage(ctx->display, root->containers[0].elems[0].boundingBox, (SDL_Color){255,255,255,255}, name);
	SDLX_RenderMessage(ctx->display, root->containers[0].elems[1].boundingBox, (SDL_Color){255,255,255,255}, "POINTS : ");

	for (i = 0; i < POTION_TYPES; i++)
	{
		SDLX_SpriteCreate(&ctx->players[id].ressources[i], 1,  ctx->textSheet.tex);
		ctx->players[id].ressources[i]._dst = root->containers[1].containers[i].elems[0]._boundingBox;
		ctx->players[id].ressources[i]._src = ctx->numbers;
		get_img_src(&src, TOK_RECT, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->Tcards, &src,  root->containers[1].containers[i].elems[0].boundingBox);
		get_img_src(&src, TOK_HEX, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->Tcards, &src,  root->containers[1].containers[i].elems[1].boundingBox);
	}



}
