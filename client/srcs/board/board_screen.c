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

#define POTION_PER_ROW (5)
#define HORZ_SEP (10)
#define VERT_SEP (10)

static void build_overlay(Context *ctx, Potion *potion)
{
	SDL_Rect src = {.x = 20, .y = 0, .w = 260, .h = 360};
	SDL_Rect dst;
	SDL_Rect rdst ;
	SDL_Rect potion_src;
	char cost[2] = {"00"};
	uint32_t color = BLACK;
	SDL_Rect scaled_bounds;

	potion_src.w = 370;
	potion_src.h = 540;
	potion_src.y = (potion->type / POTION_PER_ROW) * (potion_src.h + VERT_SEP);
	potion_src.x = (potion->type % POTION_PER_ROW) * (potion_src.w + HORZ_SEP);


	SDL_Renderer *renderer;
	SDL_Texture *renderTarget;

	renderer = SDLX_DisplayGet()->renderer;

	renderTarget = SDL_GetRenderTarget(renderer);
	// SDL_RenderCopy(renderer, from, src, dst);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 0);
	SDL_SetRenderTarget(renderer, ctx->board.overlay.ecost.texture);
	SDL_RenderClear(ctx->display->renderer);
	SDL_SetRenderTarget(renderer, ctx->board.overlay.rcost.texture);
	SDL_RenderClear(ctx->display->renderer);
	SDL_SetRenderTarget(renderer, ctx->board.overlay.pcost.texture);
	SDL_RenderClear(ctx->display->renderer);
	SDL_SetRenderTarget(renderer, ctx->board.overlay.potion.texture);
	SDL_RenderClear(ctx->display->renderer);

	overlay_text(ctx->board.overlay.name.texture, NULL, NULL, BLACK, 1, potions_by_id[ctx->board.overlay.selected->type].name);
	overlay_text(ctx->board.overlay.effect.texture, NULL, NULL, BLACK, 1, "Effect: None");
	overlay_texture(ctx->board.overlay.potion.texture, ctx->assets.potions, NULL, &potion_src );

	dst = ctx->board.overlay.ecost._dst;
	rdst = ctx->board.overlay.rcost._dst;
	SDL_SetRenderTarget(renderer, ctx->board.overlay.ecost.texture);
	// SDL_GetRendererOutputSize(renderer, &dst.w, & dst.h);
	dst.x =  0;
	dst.y =  0;
	dst.w = dst.h;
	rdst.x =  0;
	rdst.y =  0;
	rdst.w = dst.h;
	// SDL_Log("DST %d %d", dst.x ,dst.y);
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		if (potion->cost[i])
		{
			SDL_SetRenderTarget(renderer, ctx->board.overlay.ecost.texture);
			SDL_itoa(potion->cost[i], cost, 10);
			SDL_RenderCopy(ctx->display->renderer, ctx->assets.essence, &src, &dst);
			scaled_bounds = scale_and_center(0.55, dst, dst);
			SDLX_RenderMessage(SDLX_DisplayGet(), &scaled_bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, cost);
			// overlay_text(ctx->board.overlay.ecost.texture, ctx->assets.essence, &src, BLACK, 1, cost);
			dst.x += dst.w + (dst.w/ 5);
		}
		if (potions_by_id[potion->type].essences[i])
		{
			SDL_SetRenderTarget(renderer, ctx->board.overlay.rcost.texture);
			SDL_itoa(potions_by_id[potion->type].essences[i], cost, 10);
			SDL_RenderCopy(ctx->display->renderer, ctx->assets.essence, &src, &rdst);
			scaled_bounds = scale_and_center(0.55, rdst, rdst);
			SDLX_RenderMessage(SDLX_DisplayGet(), &scaled_bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, cost);
			// overlay_text(ctx->board.overlay.ecost.texture, ctx->assets.essence, &src, BLACK, 1, cost);
			rdst.x += rdst.w + (rdst.w/ 5);
		}
		if (i == 1)
		{
			src.x = 20;
			src.y += src.h + 10;
		}
		else
		src.x += src.w;
	}
	dst = ctx->board.overlay.pcost._dst;
	dst.w = dst.h;
	dst.x = 0;
	dst.y = 0;
	SDL_SetRenderTarget(renderer, ctx->board.overlay.pcost.texture);

	for (int i = 0; i < potions_by_id[potion->type].cost_count; ++i)
	{
		potion_src.y = (potions_by_id[potion->type].potion_cost[i] / POTION_PER_ROW) * (potion_src.h + VERT_SEP);
		potion_src.x = (potions_by_id[potion->type].potion_cost[i] % POTION_PER_ROW) * (potion_src.w + HORZ_SEP);
		SDL_RenderCopy(ctx->display->renderer, ctx->assets.potions, &potion_src, &dst);
		dst.x += dst.w + (dst.w / 5);
	}

	SDL_SetRenderTarget(renderer, renderTarget);
}

void board_screen(Context *ctx)
{
	if (ctx->board.overlay.selected != NULL)
	{
		canBuy = can_buy_potion(ctx, ctx->board.overlay.selected);
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
					ctx->board.overlay.position = x * MAX_ROWCARD + i;
					ctx->board.overlay.selected = &ctx->board.rows[x].card[i];
					ctx->board.rows[x].cardButton[i].triggered = 0;
					build_overlay(ctx, &ctx->board.rows[x].card[i]);
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
	SDL_RenderCopy(ctx->display->renderer, ctx->assets.board_bg, NULL, NULL);
	SDLX_RenderQueuePush(&ctx->board.switchScreen.sprite);
	for (int x = 0; x < ctx->board.masterPotions.count; x++)
	{
		SDLX_RenderQueuePush(&ctx->board.masterPotions.card[x].sprite);
	}
	for (int x = 0; x < ROW_COUNT; x++)
	{
		for (int i = 0; i < ctx->board.rows[x].count; i++)
		{
			SDLX_RenderQueuePush(&ctx->board.rows[x].card[i].sprite);
		}
	}
	if (ctx->board.overlay.selected != NULL)
	{
		SDLX_RenderQueuePush(&ctx->board.overlay.bg);
		SDLX_RenderQueuePush(&ctx->board.overlay.potion);
		SDLX_RenderQueuePush(&ctx->board.overlay.name);
		SDLX_RenderQueuePush(&ctx->board.overlay.effect);
		SDLX_RenderQueuePush(&ctx->board.overlay.cost);
		SDLX_RenderQueuePush(&ctx->board.overlay.ecost);
		SDLX_RenderQueuePush(&ctx->board.overlay.pcost);
		SDLX_RenderQueuePush(&ctx->board.overlay.rcost);
		SDLX_RenderQueuePush(&ctx->board.overlay.recycle);
		SDLX_RenderQueuePush(&ctx->board.overlay.exit.sprite);
		if (canBuy)
			SDLX_RenderQueuePush(&ctx->board.overlay.buy.sprite);
		// for (int i = 0; i < ESSENCE_TYPES; ++i)
		// {
		// 	SDLX_RenderQueuePush(&ctx->board.overlay.essences[i]);

		// }
	}


	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
