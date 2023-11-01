#include "../includes/splendor.h"
#include "../includes/table.h"

void set_board_cards_active(Context *ctx, int enabled)
{
	for (int x = 0; x < ROW_COUNT; x++)
	{
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			ctx->board.rows[x].cardButton[i].enabled = enabled;
		}
	}
}

static int canBuy;

void board_screen(Context *ctx)
{
	if (ctx->board.overlay.selected != NULL)
	{
		canBuy = 1;
		for (int i = 0; i < ESSENCE_TYPES; ++i)
		{
			if (ctx->player.tokens[i] < ctx->board.overlay.selected->cost[i])
			{
				canBuy = 0;
				break;
			}
		}
		if (ctx->board.overlay.exit.button.triggered == SDLX_KEYUP)
		{
			set_board_cards_active(ctx, SDL_TRUE);
			ctx->board.overlay.selected = NULL;
			ctx->board.overlay.exit.button.triggered = 0;
		}
		if (canBuy && ctx->board.overlay.buy.button.triggered == SDLX_KEYUP)
		{
			set_board_cards_active(ctx, SDL_FALSE);
			set_main_cards_active(ctx, SDL_TRUE);
			ctx->state = PLAYERSTATUS;
			ctx->board.overlay.selected = NULL;
			send_action('b', ctx->board.overlay.position);
		}
	}
	else {
		if (ctx->board.switchScreen.button.triggered == SDLX_KEYUP)
		{
			set_board_cards_active(ctx, SDL_FALSE);
			set_main_cards_active(ctx, SDL_TRUE);
			ctx->state = PLAYERSTATUS;
		}
		for (int x = 0; x < ROW_COUNT; x++)
		{
			for (int i = 0; i < MAX_ROWCARD; i++)
			{
				if (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYUP)
				{
					char cost[2] = {"00"};
					SDL_Rect src = {.x = 20, .y = 0, .w = 260, .h = 360};
					ctx->board.overlay.position = x * MAX_ROWCARD + i;
					ctx->board.overlay.selected = &ctx->board.rows[x].card[i];
					ctx->board.rows[x].cardButton[i].triggered = 0;
					overlay_text(ctx->board.overlay.name.texture, NULL, NULL, WHITE, 1, potions_by_id[ctx->board.overlay.selected->type].name);
					overlay_text(ctx->board.overlay.desc.texture, NULL, NULL, WHITE, 1, "Desc here");
					for (int i = 0; i < ESSENCE_TYPES; ++i)
					{
						SDL_itoa(ctx->board.overlay.selected->cost[i], cost, 10);
						overlay_text(ctx->board.overlay.essences[i].texture, ctx->assets.essence, &src, ((0xFF000000 >> (5 * i)) + 0xFF), 1, cost);
						if (i == 1)
						{
							src.x = 20;
							src.y += src.h + 10;
						}
						else
						src.x += src.w;
					}
					set_board_cards_active(ctx, SDL_FALSE);
					break ;
				}
			}
		}
	}


	render_board_screen(ctx);
}


void render_board_screen(Context *ctx)
{
	SDLX_RenderQueuePush(&ctx->board.switchScreen.sprite);
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.switchScreen.sprite.dst);
	for (int x = 0; x < ROW_COUNT; x++)
	{
		for (int i = 0; i < ctx->board.rows[x].count; i++)
		{
			SDLX_RenderQueuePush(&ctx->board.rows[x].card[i].sprite);
		}
	}
	if (ctx->board.overlay.selected != NULL)
	{
		SDLX_RenderQueuePush(&ctx->board.overlay.background);
		SDLX_RenderQueuePush(&ctx->board.overlay.name);
		SDLX_RenderQueuePush(&ctx->board.overlay.desc);
		SDLX_RenderQueuePush(&ctx->board.overlay.cost);
		SDLX_RenderQueuePush(&ctx->board.overlay.exit.sprite);
		if (canBuy)
			SDLX_RenderQueuePush(&ctx->board.overlay.buy.sprite);
		for (int i = 0; i < ESSENCE_TYPES; ++i)
		{
			SDLX_RenderQueuePush(&ctx->board.overlay.essences[i]);

		}
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.overlay.background.dst);
		if (ctx->board.overlay.exit.button.state == SDLX_KEYHELD)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);

	}


	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
