#include "../includes/splendor.h"
#include "../includes/table.h"

void set_main_cards_active(Context *ctx, int enabled)
{
	for (int x = 0; x < ctx->player.ownedCount; x++)
	{
		ctx->mainUI.ownedButtons[x].enabled = enabled;
	}
}

#define POTION_PER_ROW (5)
#define HORZ_SEP (10)
#define VERT_SEP (10)

static void build_overlay(Context *ctx, Potion *potion)
{
	char fill[9] = {"Uses:   "};
	SDL_Rect src = {.x = 20, .y = 0, .w = 260, .h = 360};
	SDL_Rect dst;
	char cost[2] = {"00"};
	uint32_t color = BLACK;
	SDL_Rect scaled_bounds;
	SDL_Renderer *renderer;
	SDL_Texture *renderTarget;

	renderer = SDLX_DisplayGet()->renderer;
	renderTarget = SDL_GetRenderTarget(renderer);

	SDL_itoa(potion->fill, fill + 7, 10);
	overlay_text(ctx->mainUI.overlay.name.texture, NULL, NULL, WHITE, 1, potions_by_id[potion->type].name);
	overlay_text(ctx->mainUI.overlay.fillAmount.texture, NULL, NULL, WHITE, 1, fill);
	overlay_text(ctx->mainUI.overlay.effect.texture, NULL, NULL, WHITE, 1, "Effect: None");

	ctx->mainUI.overlay.potion.src = &ctx->mainUI.overlay.potion._src;
	ctx->mainUI.overlay.potion._src.w = 370;
	ctx->mainUI.overlay.potion._src.h = 540;
	ctx->mainUI.overlay.potion._src.y = (potion->type / POTION_PER_ROW) * (ctx->mainUI.overlay.potion._src.h + VERT_SEP);
	ctx->mainUI.overlay.potion._src.x = (potion->type % POTION_PER_ROW) * (ctx->mainUI.overlay.potion._src.w + HORZ_SEP);

	dst = ctx->mainUI.overlay.recyle._dst;
	dst.w = dst.h;
	dst.x = 0;
	dst.y = 0;
	SDL_SetRenderTarget(renderer, ctx->mainUI.overlay.recyle.texture);
	SDL_RenderClear(ctx->display->renderer);
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		if (potions_by_id[potion->type].essences[i])
		{
			SDL_itoa(potions_by_id[potion->type].essences[i], cost, 10);
			SDL_RenderCopy(ctx->display->renderer, ctx->assets.essence, &src, &dst);
			scaled_bounds = scale_and_center(0.55, dst, dst);
			SDLX_RenderMessage(SDLX_DisplayGet(), &scaled_bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, cost);
			dst.x += dst.w + (dst.w  / 5);
		}
		if (i == 1)
		{
			src.x = 20;
			src.y += src.h + 10;
		}
		else
		src.x += src.w;
	}
	SDL_SetRenderTarget(renderer, renderTarget);
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
		else if (ctx->isTurn && ctx->player.actionsRemaining > 0 && ctx->mainUI.overlay.selected->fill > 0 && ctx->mainUI.overlay.use.button.triggered == SDLX_KEYUP)
		{
			send_action('u', ctx->mainUI.overlay.position);
			ctx->mainUI.overlay.selected = NULL;
			set_main_cards_active(ctx, SDL_TRUE);
		}
		else if (ctx->isTurn && ctx->player.actionsRemaining > 0 && ctx->mainUI.overlay.selected->fill > 0 && ctx->mainUI.overlay.convert.button.triggered == SDLX_KEYUP)
		{
			send_action('s', ctx->mainUI.overlay.position);
			ctx->mainUI.overlay.selected = NULL;
			set_main_cards_active(ctx, SDL_TRUE);
		}
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

					ctx->mainUI.overlay.selected = &ctx->player.owned[i];
					ctx->mainUI.overlay.position = i;
					build_overlay(ctx, &ctx->player.owned[i]);
					break ;
				}
			}
		}
	}
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{
	SDL_RenderCopy(ctx->display->renderer, ctx->mainUI.bg, NULL, NULL);
	// SDL_RenderDrawRect(ctx->display->renderer, &ctx->player.brewing.sprite._dst);
	for (int i = 0; i < ctx->player.ownedCount; ++i)
	{
		SDLX_RenderQueuePush(&ctx->player.owned[i].sprite);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		// SDL_RenderDrawRect(ctx->display->renderer, ctx->mainUI.essences[i].dst);
		SDLX_RenderQueuePush(&ctx->mainUI.essences[i]);
	}

	if (ctx->isTurn)
	{
		SDLX_RenderQueuePush(&ctx->mainUI.endTurn.sprite);
		SDLX_RenderQueuePush(&ctx->mainUI.actions);
	}
	SDLX_RenderQueuePush(&ctx->mainUI.switchScreen.sprite);

	if (ctx->player.isBrewing)
	{
		SDLX_RenderQueuePush(&ctx->player.brewing.sprite);
	}
	if (ctx->mainUI.overlay.selected != NULL)
	{
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.bg);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.name);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.potion);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.fillAmount);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.effect);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.exit.sprite);
		SDLX_RenderQueuePush(&ctx->mainUI.overlay.recyle);

		if (ctx->isTurn)
		{
			if (ctx->mainUI.overlay.selected->fill > 0)
			{
				SDLX_RenderQueuePush(&ctx->mainUI.overlay.use.sprite);
				SDLX_RenderQueuePush(&ctx->mainUI.overlay.convert.sprite);
			}
		}
	}
}
