#include "../includes/splendor.h"
#include "../includes/table.h"

void start_next_turn(Context *ctx)
{
	ctx->players[ctx->turn].actionsRemaining = 0;
	ctx->turn = (ctx->turn + 1)  % ctx->playerCount;
	ctx->players[ctx->turn].actionsRemaining = MAX_ACTIONS;
	if (ctx->players[ctx->turn].isBrewing)
	{
		int count = ctx->players[ctx->turn].potionCount;
		ctx->players[ctx->turn].isBrewing = SDL_FALSE;
		copy_potion(&ctx->players[ctx->turn].owned[count], &ctx->players[ctx->turn].brewing);
		SDL_Log("Copying to potion n%d, %s from %s",count, ctx->players[ctx->turn].owned[count].id, ctx->players[ctx->turn].brewing.id);
		overlay_text(ctx->players[ctx->turn].owned[count].sprite.texture, NULL, NULL, 0xFFFFFFFF,
			potions_by_id[ctx->players[ctx->turn].owned[count].type].name
		);
		ctx->players[ctx->turn].potionCount++;
	}
}

void startGame(Context *ctx)
{
	ctx->turn = -1;
	init_new_game(ctx);
	start_next_turn(ctx);
}

/*
	 Potions ids: <type>|<current_fill>|<cost_count>:<cost>
	Type:          integer
	Current_fill : integer
	Cost: [<identifier><type>:<count>,pas aux ]
		identifier:p or e
		type:      integer
 */

int generatePotion(Context *ctx, Potion *card, int level)
{
	int type;
	int count;
	int offset;
	int i;

	offset = 0;
	type = (rand() % potions_by_tier[level].count) + potions_by_tier[level].entries[0].id;
	card->type = type;
	card->fill = MAX_FILL;

	i = 0;
	uint8_t essences[ESSENCE_TYPES] = {0};
	while (i < count)
	{
		int amount;
		int tok;

		tok = rand() % ESSENCE_TYPES;
		amount = rand() % MAX(1, count - i) + 1;
		essences[tok] += amount;
		i += (amount + 1);
	}

	card->cost[0] = potions_by_id[type].essences[0];
	card->cost[1] = potions_by_id[type].essences[1];
	card->cost[2] = potions_by_id[type].essences[2];
	card->cost[3] = potions_by_id[type].essences[3];
	SDL_snprintf(card->id, CARD_ID_LEN + 1, "%02d|%d|%02d,%02d,%02d,%02d", type, MAX_FILL,
		potions_by_id[type].essences[0],
		potions_by_id[type].essences[1],
		potions_by_id[type].essences[2],
		potions_by_id[type].essences[3]
	 );

	SDL_Log("Generate  %s %d vs %d",
		card->id, strlen(card->id), CARD_ID_LEN
	);

	overlay_text(card->sprite.texture, NULL, NULL, 0xFFFFFFFF, potions_by_id[type].name);

		SDL_Renderer *renderer;
		SDL_Texture *renderTarget;
		SDL_Rect bounds = {0};
		uint32_t color = 0x000000FF;
		char cost[2] = {"00"};

		renderer = SDLX_DisplayGet()->renderer;
		renderTarget = SDL_GetRenderTarget(renderer);

		SDL_SetRenderTarget(renderer, card->sprite.texture);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		// SDL_RenderClear(renderer);
		SDL_GetRendererOutputSize(renderer, &bounds.w, &bounds.h);

		bounds.y = bounds.h - (bounds.h / 5);
		bounds.w /= 5;
		bounds.h /= 5;

		SDL_Rect src = {.x = 20, .y = 0, .w = 260, .h = 360};
		for (int i = 0; i < ESSENCE_TYPES; ++i)
		{
			SDL_itoa(potions_by_id[type].essences[i], cost, 10);
			SDL_RenderCopy(ctx->display->renderer, ctx->assets.essences,&src, &bounds);
			SDLX_RenderMessage(SDLX_DisplayGet(), &bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, cost);
			bounds.x += bounds.w;
			if (i == 1)
			{
				src.x = 20;
				src.y += src.h + 10;
			}
			else
				src.x += src.w;

		}
		SDL_SetRenderTarget(renderer, renderTarget);

	return 1;
}

void copy_potion(Potion *dst, Potion *src)
{
	dst->fill = src->fill;
	dst->type = src->type;
	SDL_memcpy(dst->id ,src->id, CARD_ID_LEN);
	SDL_memcpy(dst->cost ,src->cost, ESSENCE_TYPES * sizeof(int));

}
