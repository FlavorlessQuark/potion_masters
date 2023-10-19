#include "../includes/splendor.h"

void main_screen(Context *ctx)
{
	if (ctx->mainUI.overlay.selected != NULL)
	{
		if (ctx->mainUI.overlay.exit.button.triggered == SDLX_KEYDOWN)
			ctx->mainUI.overlay.selected = NULL;
		else if (ctx->mainUI.overlay.use.button.triggered == SDLX_KEYDOWN)
			send_action('u', NULL);
		else if (ctx->mainUI.overlay.convert.button.triggered == SDLX_KEYDOWN)
			send_action('r', NULL);
	}
	else {
		if (ctx->mainUI.endTurn.button.triggered == SDLX_KEYDOWN)
			send_action('e', NULL);
		else if (ctx->mainUI.switchScreen.button.triggered == SDLX_KEYDOWN)
			ctx->state = BOARD;
		else {
			for (int i = 0; i < MAX_POTIONS; ++i)
			{
				if (ctx->mainUI.ownedButtons[i].triggered == SDLX_KEYHELD)
					ctx->mainUI.overlay.selected = &ctx->mainUI.owned[i];
			}
		}
	}
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);

	for (int i = 0; i < MAX_POTIONS; ++i)
	{
		if ( ctx->mainUI.ownedButtons[i].triggered == SDLX_KEYHELD)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.owned[i].sprite.dst);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.essences[i].dst);
		SDLX_RenderQueuePush(&ctx->mainUI.essences[i]);
	}

	// SDL_Log(" ??? %d ", ctx->mainUI.endTurn.button.state);
	// if (ctx->mainUI.endTurn.button.triggered == SDLX_KEYHELD)
	// 		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
	// 	else
	// 		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	// SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.endTurn.sprite.dst);

	SDLX_RenderQueuePush(&ctx->mainUI.endTurn.sprite);

	// if (ctx->mainUI.switchScreen.button.triggered == SDLX_KEYHELD)
	// 		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
	// 	else
	// 		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	// SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.switchScreen.button.boundingBox);
	SDLX_RenderQueuePush(&ctx->mainUI.switchScreen.sprite);


	if (ctx->mainUI.overlay.selected != NULL)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.exit.sprite.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.convert.sprite.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.use.sprite.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.name.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.potion.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.effect.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.overlay.fillAmount.dst);


		SDLX_RenderQueuePush(&ctx->mainUI.overlay.name);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.fillAmount);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.effect);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.exit.sprite);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.use.sprite);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.convert.sprite);
	}

	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

