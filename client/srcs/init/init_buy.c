#include "../includes/splendor.h"

void init_buy_screen(Context *ctx)
{
	SDLX_RectContainer *root;
	SDL_Rect exit;
	SDL_Rect src;

	exit.x = ctx->display->win_w - 75;
	exit.y = 25;
	exit.w = 50;
	exit.h = 50;

	SDLX_ButtonCreate(&ctx->buyscreen.buyButton, NULL);
	SDLX_ButtonCreate(&ctx->buyscreen.reserveButton, NULL);
	SDLX_ButtonCreate(&ctx->buyscreen.exit, NULL);
	SDLX_SpriteCreate(&ctx->buyscreen.showSelected, 1, NULL);
	SDLX_SpriteCreate(&ctx->buyscreen.reserveSprite, 1, ctx->textSheet.tex);
	SDLX_SpriteCreate(&ctx->buyscreen.buySprite, 1, ctx->textSheet.tex);

	root = parseUI("assets/UI/buyUI");
	ctx->buyscreen.reserveSprite._src = SDLX_TextSheet_Add(&ctx->textSheet, "RESERVE", ctx->display->defaultFont, (SDL_Color){0, 255, 0, 255});
	ctx->buyscreen.buySprite._src = SDLX_TextSheet_Add(&ctx->textSheet, "BUY", ctx->display->defaultFont, (SDL_Color){0, 0, 255, 255});


	ctx->buyscreen.bg = SDL_CreateTexture(ctx->display->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		ctx->display->win_w, ctx->display->win_h
	);
	SDL_SetRenderTarget(ctx->display->renderer, ctx->buyscreen.bg);
	ctx->buyscreen.showSelected._dst = root->containers[0].elems[0]._boundingBox;
	ctx->buyscreen.buyButton._boundingBox = root->containers[1].elems[0]._boundingBox;
	ctx->buyscreen.buySprite._dst = root->containers[1].elems[0]._boundingBox;
	ctx->buyscreen.reserveButton._boundingBox = root->containers[1].elems[1]._boundingBox;
	ctx->buyscreen.reserveSprite._dst = root->containers[1].elems[1]._boundingBox;
	ctx->buyscreen.exit._boundingBox = exit;

	ctx->buyscreen.showSelected.src = NULL;
	SDLX_RenderMessage(ctx->display, &ctx->buyscreen.exit._boundingBox, (SDL_Color){255, 0, 0, 255}, "X");
	for (int n = 0; n < POTION_TYPES; n++)
	{
		get_img_src(&src, TOK_HEX, n);
		SDLX_SpriteCreate(&ctx->buyscreen.costSprite[n], 1, ctx->textSheet.tex);
		ctx->buyscreen.costSprite[n]._dst.x = ctx->buyscreen.showSelected._dst.x + (ctx->buyscreen.showSelected._dst.w / 10);
		ctx->buyscreen.costSprite[n]._dst.y = ctx->buyscreen.showSelected._dst.y + (ctx->buyscreen.showSelected._dst.h / 4 * n);
		ctx->buyscreen.costSprite[n]._dst.w = ctx->buyscreen.showSelected._dst.w / 10;
		ctx->buyscreen.costSprite[n]._dst.h = ctx->buyscreen.showSelected._dst.h / 5;
		SDL_RenderCopy(ctx->display->renderer, ctx->cardTex, &src, &ctx->buyscreen.costSprite[n]._dst);
		ctx->buyscreen.costSprite[n]._src = ctx->nums;
	}
	SDL_SetRenderTarget(ctx->display->renderer, NULL);
}
