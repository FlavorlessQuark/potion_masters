#include "../includes/splendor.h"

void render_main_screen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255,0,0,255);
	for (int i = 0; i < MAIN_SCREEN_SPRITE_COUNT; i++)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[i].dst);

	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0,0,0,255);
}

void main_screen(Context *ctx)
{
	render_main_screen(ctx);
}

void init_status_bar(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	for (int i = 0; i < root->elemCount; i++)
	{
		// SDL_Log(" Box %d (%d, %d) %d", i,root->elems[i]._boundingBox.w, root->elems[i]._boundingBox.h, offset );
		ctx->mainscreen->UI[*offset]._dst = root->elems[i]._boundingBox;
		// SDLX_SpritePrint(&ctx->mainscreen->UI[i + *offset]);
		*offset+= 1;
	}
}
void init_token_bar(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	SDLX_RectContainer container;

	for (int i = 0; i < root->containerCount; i++)
	{
		container = root->containers[i];
		for(int x = 0; x < container.elemCount; x++)
		{
			ctx->mainscreen->UI[*offset]._dst = container.elems[x]._boundingBox;
			*offset+= 1;
		}
	}
}

void init_reserved_cards(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	for (int i = 0; i < root->containerCount; i++)
	{
		ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[0]._boundingBox;
		*offset+= 1;
		ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[1]._boundingBox;
		*offset+= 1;
	}
}

void init_button(Context *ctx, int *offset, SDLX_RectContainer *root)
{
	ctx->mainscreen->UI[*offset]._dst = root->elems[0]._boundingBox;
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
	}
	root = initUI("assets/UI/mainUI");
	offset = 0;

	init_status_bar(ctx, &offset, &root->containers[0]);
	init_token_bar(ctx, &offset, &root->containers[1].containers[0]);
	init_reserved_cards(ctx, &offset, &root->containers[1].containers[1]);
	init_button(ctx, &offset, &root->containers[1].containers[2]);
}



