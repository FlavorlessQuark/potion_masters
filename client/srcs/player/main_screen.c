#include "../includes/splendor.h"
#include "../includes/table.h"

void set_main_cards_active(Context *ctx, int enabled)
{
	for (int x = 0; x < ctx->player.ownedCount; x++)
	{
		ctx->mainUI.ownedButtons[x].enabled = enabled;
	}
}


void main_screen(Context *ctx)
{
	if (ctx->mainUI.overlay.selected != NULL)
	{
		if (ctx->mainUI.overlay.exit.button.triggered == SDLX_KEYUP)
		{
			ctx->mainUI.overlay.selected = NULL;
			set_main_cards_active(ctx, SDL_TRUE);
		}
		else if (ctx->isTurn && ctx->player.actionsRemaining > 0 && ctx->mainUI.overlay.use.button.triggered == SDLX_KEYUP)
			send_action('u', ctx->mainUI.overlay.position);
		else if (ctx->isTurn && ctx->player.actionsRemaining > 0 && ctx->mainUI.overlay.convert.button.triggered == SDLX_KEYUP)
			send_action('r', ctx->mainUI.overlay.position);
	}
	else {
		if (ctx->isTurn && ctx->mainUI.endTurn.button.triggered == SDLX_KEYUP)
			send_action('e', 0);
		else if (ctx->mainUI.switchScreen.button.triggered == SDLX_KEYUP)
		{
			ctx->state = BOARD;
			set_main_cards_active(ctx, SDL_FALSE);
			set_board_cards_active(ctx, SDL_TRUE);
		}
		else {
			for (int i = 0; i < ctx->player.ownedCount; ++i)
			{
				if (ctx->mainUI.ownedButtons[i].triggered == SDLX_KEYUP)
				{
					char fill[2] = {"00"};
					SDL_itoa(ctx->player.owned[i].fill, fill, 10);
					ctx->mainUI.overlay.selected = &ctx->player.owned[i];
					ctx->mainUI.overlay.position = i;
					overlay_text(ctx->mainUI.overlay.name.texture, NULL, NULL, WHITE, 1, potions_by_id[ctx->player.owned[i].type].name);
					overlay_text(ctx->mainUI.overlay.fillAmount.texture, NULL, NULL, WHITE, 1, fill);
					break ;
				}
			}
		}
	}
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{

	for (int i = 0; i < ctx->player.ownedCount; ++i)
	{
		SDLX_RenderQueuePush(&ctx->player.owned[i].sprite);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.essences[i].dst);
		SDLX_RenderQueuePush(&ctx->mainUI.essences[i]);
	}

	if (ctx->isTurn)
		SDLX_RenderQueuePush(&ctx->mainUI.endTurn.sprite);
	SDLX_RenderQueuePush(&ctx->mainUI.switchScreen.sprite);

	if (ctx->player.isBrewing)
	{
		SDLX_RenderQueuePush(&ctx->player.brewing.sprite);
	}
	if (ctx->mainUI.overlay.selected != NULL)
	{
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.name);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.fillAmount);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.effect);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.exit.sprite);

		if (ctx->isTurn)
		{
			SDLX_RenderQueuePush(&ctx->mainUI.overlay.use.sprite);
			SDLX_RenderQueuePush(&ctx->mainUI.overlay.convert.sprite);
		}
	}

}

