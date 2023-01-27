#include "../includes/splendor.h"


void render_buy_screen(Context *ctx)
{

	SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->buyscreen->buyButton.triggered == SDLX_KEYHELD),
					255 * (ctx->buyscreen->buyButton.state == SDLX_FOCUS_STAY),
					255,
					255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen->buyButton.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->buyscreen->reserveButton.triggered == SDLX_KEYHELD),
					255 * (ctx->buyscreen->reserveButton.state == SDLX_FOCUS_STAY),
					255,
					255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen->reserveButton.boundingBox);


	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 255, 255);
SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen->showSelected.dst);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 255, 255);
	SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->buyscreen->exit.triggered == SDLX_KEYHELD),
					255 ,
					0,
					255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen->exit.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

void buy_screen(Context *ctx)
{
	// if (ctx->buyscreen->exit.triggered == SDLX_KEYUP)
	// 	ctx->state = 0;
	render_buy_screen(ctx);
}


void init_buy_screen(Context *ctx)
{
	SDLX_RectContainer *root;
	SDL_Rect exit;

	exit.x = ctx->display->win_w - 100;
	exit.y = 50;
	exit.w = 50;
	exit.h = 50;
	if (!ctx->buyscreen)
	{
		ctx->buyscreen = calloc(1, sizeof(BuyScreen));
		SDLX_ButtonCreate(&ctx->buyscreen->buyButton, NULL);
		SDLX_ButtonCreate(&ctx->buyscreen->reserveButton, NULL);
		SDLX_ButtonCreate(&ctx->buyscreen->exit, NULL);
		SDLX_SpriteCreate(&ctx->buyscreen->showSelected, 1, NULL);
	}
	root = initUI("assets/UI/buyUI");
	ctx->buyscreen->showSelected._dst = root->containers[0].elems[0]._boundingBox;
	ctx->buyscreen->showSelected._dst.x += 100;
	ctx->buyscreen->buyButton._boundingBox = root->containers[1].elems[0]._boundingBox;
	ctx->buyscreen->buyButton._boundingBox.x += 100;
	ctx->buyscreen->reserveButton._boundingBox = root->containers[1].elems[1]._boundingBox;
	ctx->buyscreen->reserveButton._boundingBox.x += 100;
	ctx->buyscreen->exit._boundingBox = exit;
}



