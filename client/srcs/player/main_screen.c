#include "../includes/splendor.h"

# define SWITCH_BUTTTON MAIN_SCREEN_SPRITE_COUNT - 2
# define EXTRA_TOKEN MAIN_SCREEN_SPRITE_COUNT - 1

void main_screen(Context *ctx)
{
	if (ctx->mainUI.endTurn.button.triggered == SDLX_KEYDOWN)
		send_action('e', NULL);
	if (ctx->mainUI.switchScreen.button.triggered == SDLX_KEYDOWN)
		ctx->state = BOARD;
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);

	for (int i = 0; i < MAX_POTIONS; ++i)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.owned[i].sprite.dst);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.essences[i].dst);
	}

	// SDL_Log(" ??? %d ", ctx->mainUI.endTurn.button.state);
	if (ctx->mainUI.endTurn.button.triggered == SDLX_KEYHELD)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.endTurn.sprite.dst);

	if (ctx->mainUI.switchScreen.button.triggered == SDLX_KEYHELD)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.switchScreen.button.boundingBox);

	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

