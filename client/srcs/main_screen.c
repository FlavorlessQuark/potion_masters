#include "../includes/splendor.h"

void render_main_screen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255,0,0,255);
	for (int i = 0; i < MAIN_SCREEN_SPRITE_COUNT - (MAX_RESERVE + 2); i++)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[i].dst);
	}
	SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 1].dst);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2].dst);
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->mainscreen->reserved[i].state == SDLX_FOCUS_STAY),
					255 * (ctx->mainscreen->reserved[i].triggered == SDLX_KEYHELD),
					255,
					255);
		if (ctx->mainscreen->reserved[i].triggered == SDLX_KEYDOWN)
		{
			ctx->state = 2;
			ctx->buyscreen->cardOrigin = 1;
		}
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->reserved[i].boundingBox);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0,0,0,255);
}

void main_screen(Context *ctx)
{
	if (ctx->mainscreen->switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 1;
	render_main_screen(ctx);
}

void init_status_bar(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	for (int i = 0; i < root->elemCount; i++)
	{
		ctx->mainscreen->UI[*offset]._dst = root->elems[i]._boundingBox;
		*offset+= 1;
	}
}
void init_token_bar(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	SDLX_RectContainer container;

	SDL_Log("ROOT HAS %d CHILDREN", root->containerCount);
	for (int i = 0; i < root->containerCount; i++)
	{
		container = root->containers[i];
		SDL_Log("BOX HAS %d CHILDREN", root->containers[i].elemCount);
		for(int x = 0; x < container.elemCount; x++)
		{
			SDL_Log(" Box %d (%d, %d) %d", x, container.elems[x]._boundingBox.w,  container.elems[x]._boundingBox.h, *offset );
			ctx->mainscreen->UI[*offset]._dst = container.elems[x]._boundingBox;
			*offset+= 1;
		}
	}
}

void init_reserved_cards(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	uint8_t buttonOffset;

	for (int i = 0; i < root->containerCount; i++)
	{
		ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[0]._boundingBox;
		*offset+= 1;
		ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[1]._boundingBox;
		*offset+= 1;
	}
}

// TODO Bye bye magic numbers
void init_button(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 1]._dst.x = root->self._boundingBox.x + ( root->self._boundingBox.w  / 2) - 50;
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 1]._dst.y = root->self._boundingBox.y + ( root->self._boundingBox.h  / 2) - 25;
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 1]._dst.w = 100;
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 1]._dst.h = 50;

	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.x = ctx->display->win_w - 55;
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.y =( ctx->display->win_h / 2) - 25;
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.w = 50;
	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.h = 50;
}

void init_main_screen(Context *ctx)
{
	SDLX_RectContainer *root;
	int offset;

	if (!ctx->mainscreen)
	{
		ctx->mainscreen = calloc(1, sizeof(MainScreen));
		for (int i = 0; i < MAIN_SCREEN_SPRITE_COUNT; i++)
			SDLX_SpriteCreate(&ctx->mainscreen->UI[i], 1, NULL);
		for (int i = 0; i < MAX_RESERVE; i++)
			SDLX_ButtonCreate(&ctx->mainscreen->reserved[i], &ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - (MAX_RESERVE + 2) + i]._dst);
		SDLX_ButtonCreate(&ctx->mainscreen->switchMode, &ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst);

	}
	root = initUI("assets/UI/mainUI");
	offset = 0;

	init_status_bar(ctx, &offset, &root->containers[0]);
	SDL_Log("LOAD STATUS SCREEN %d, ", root->containers[1].containerCount);
	init_token_bar(ctx, &offset, &root->containers[1].containers[0]);
	SDL_Log("LOAD TOKEN SCREEN");
	init_reserved_cards(ctx, &offset, &root->containers[1].containers[1]);
	init_button(ctx, &offset, &root->containers[1].containers[2]);
}



