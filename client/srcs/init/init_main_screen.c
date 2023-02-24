#include "../includes/splendor.h"

void init_status_bar(Context *ctx, SDLX_RectContainer *root)
{
	SDLX_SpriteCreate(&ctx->UI.name, 1, NULL);
	SDLX_SpriteCreate(&ctx->UI.points, 1, NULL);
	ctx->UI.name._dst = root->elems[0]._boundingBox;
	ctx->UI.points._dst = root->elems[0]._boundingBox;
	// ctx.UI.name._dst = root->elems[0]._boundingBox;
}
void init_token_bar(Context *ctx, SDLX_RectContainer *root)
{
	SDLX_RectContainer container;

	for (int i = 0; i < CARD_TYPES; i++)
	{
		SDLX_SpriteCreate(&ctx->UI.permanents[i], 1, ctx->cardTex);
		SDLX_SpriteCreate(&ctx->UI.tokens[i], 1, ctx->cardTex);
		ctx->UI.permanents[i]._dst = root->containers[i].elems[0]._boundingBox;
		ctx->UI.tokens[i]._dst = root->containers[i].elems[1]._boundingBox;
	}
	ctx->UI.tokens[TOK_COUNT - 1]._dst = root->containers[TOK_COUNT - 1].elems[0]._boundingBox;

	// for (int i = 0; i < root->containerCount; i++)
	// {
	// 	container = root->containers[i];
	// 	for(int x = 0; x < container.elemCount; x++)
	// 	{
	// 		ctx->mainscreen->UI[*offset]._dst = container.elems[x]._boundingBox;
	// 		*offset+= 1;
	// 	}
	// }
}

void init_reserved_cards(Context *ctx, SDLX_RectContainer *root)
{
	uint8_t buttonOffset;

	for (int i = 0; i < root->containerCount; i++)
	{
		SDLX_SpriteCreate(&ctx->player.reserved[i * 2 + 0].sprite, 1, ctx->cardTex);
		SDLX_SpriteCreate(&ctx->player.reserved[i * 2 + 1].sprite, 1, ctx->cardTex);
		SDLX_ButtonCreate(&ctx->UI.reserved[i * 2 + 0], ctx->player.reserved[i * 2 + 0].sprite.dst);
		SDLX_ButtonCreate(&ctx->UI.reserved[i * 2 + 1], ctx->player.reserved[i * 2 + 1].sprite.dst);
		ctx->player.reserved[i * 2 + 0].sprite._dst = root->containers[i].elems[0]._boundingBox;
		ctx->player.reserved[i * 2 + 1].sprite._dst = root->containers[i].elems[1]._boundingBox;
		// ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[0]._boundingBox;
		// *offset+= 1;
		// ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[1]._boundingBox;
		// *offset+= 1;
	}
}

// TODO Bye bye magic numbers
void init_button(Context *ctx, SDLX_RectContainer *root)
{

	SDLX_ButtonCreate(&ctx->UI.switchMode, NULL);
	ctx->UI.switchMode._boundingBox.x =  ctx->display->win_w - 55;
	ctx->UI.switchMode._boundingBox.y = ( ctx->display->win_h / 2) - 25;
	ctx->UI.switchMode._boundingBox.w =  50;
	ctx->UI.switchMode._boundingBox.h =  50;

// 	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.x = ctx->display->win_w - 55;
// 	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.y =( ctx->display->win_h / 2) - 25;
// 	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.w = 50;
// 	ctx->mainscreen->UI[MAIN_SCREEN_SPRITE_COUNT - 2]._dst.h = 50;
}

void init_main_screen(Context *ctx)
{
	SDLX_RectContainer *root;

	root = parseUI("assets/UI/mainUI");

	init_status_bar(ctx, &root->containers[0]);
	SDL_Log("LOAD STATUS SCREEN %d, ", root->containers[1].containerCount);
	init_token_bar(ctx, &root->containers[1].containers[0]);
	SDL_Log("LOAD TOKEN SCREEN");
	init_reserved_cards(ctx, &root->containers[1].containers[1]);
	init_button(ctx, &root->containers[1].containers[2]);
	endTurn(ctx);
}



