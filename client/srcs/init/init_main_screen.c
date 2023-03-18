#include "../includes/splendor.h"

void init_status_bar(Context *ctx, SDLX_RectContainer *root)
{
	SDL_Color color = {255,255,255,255};

	SDLX_SpriteCreate(&ctx->UI.name, 1, NULL);
	SDLX_SpriteCreate(&ctx->UI.points, 1, NULL);
	ctx->UI.name._dst = root->elems[0]._boundingBox;
	ctx->UI.points._dst = root->elems[0]._boundingBox;

	root->elems[0]._boundingBox.h /= 2;
	SDLX_RenderMessage(ctx->display, &root->elems[0]._boundingBox, color, "POINTS :");
	// ctx.UI.name._dst = root->elems[0]._boundingBox;
}
void init_token_bar(Context *ctx, SDLX_RectContainer *root)
{
	SDLX_RectContainer container;
	SDL_Rect src;
	int i;

	for (i = 0; i < CARD_TYPES; i++)
	{
		SDLX_SpriteCreate(&ctx->UI.permanents[i], 1, ctx->textSheet.tex);
		SDLX_SpriteCreate(&ctx->UI.tokens[i], 1, ctx->textSheet.tex);
		ctx->UI.permanents[i]._dst = root->containers[i].elems[0]._boundingBox;
		ctx->UI.tokens[i]._dst = root->containers[i].elems[1]._boundingBox;
		ctx->UI.permanents[i]._src = ctx->nums;
		ctx->UI.tokens[i]._src = ctx->nums;

		get_img_src(&src, TOK_HEX, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->cardTex, &src, &root->containers[i].elems[0]._boundingBox);
		get_img_src(&src, TOK_RECT, i);
		SDL_RenderCopy(ctx->display->renderer, ctx->cardTex, &src, &root->containers[i].elems[1]._boundingBox);
	}
	src = (SDL_Rect){.h = 53, .w = CARD_W / 2,
						 .x = (SEP_X + 5) + (CARD_W / 2 + SEP_X) * i, .y =  (CARD_H * 2) + SEP_Y * 3 + 35};
	SDL_RenderCopy(ctx->display->renderer, ctx->cardTex, &src, &root->containers[i].elems[0]._boundingBox);
	SDLX_SpriteCreate(&ctx->UI.tokens[i], 1, ctx->textSheet.tex);
	ctx->UI.tokens[i]._dst = root->containers[i].elems[0]._boundingBox;
	ctx->UI.tokens[i]._src = ctx->nums;
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
		ctx->player.reserved[i * 2 + 0].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctx->player.reserved[i * 2 + 0].sprite._dst.w, ctx->player.reserved[i * 2 + 0].sprite._dst.h);
		ctx->player.reserved[i * 2 + 1].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctx->player.reserved[i * 2 + 1].sprite._dst.w, ctx->player.reserved[i * 2 + 1].sprite._dst.h);
		SDL_SetTextureBlendMode(ctx->player.reserved[i * 2 + 0].sprite.texture , SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(ctx->player.reserved[i * 2 + 1].sprite.texture , SDL_BLENDMODE_BLEND);
		ctx->player.reserved[i * 2 + 0].sprite.src = NULL;
		ctx->player.reserved[i * 2 + 1].sprite.src = NULL;
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

	ctx->UI.bg = SDL_CreateTexture(ctx->display->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		ctx->display->win_w, ctx->display->win_h
		);

	SDL_SetRenderTarget(ctx->display->renderer, ctx->UI.bg);

	init_status_bar(ctx, &root->containers[0]);
	// SDL_Log("LOAD STATUS SCREEN %d, ", root->containers[1].containerCount);
	init_token_bar(ctx, &root->containers[1].containers[0]);
	// SDL_Log("LOAD TOKEN SCREEN");
	init_reserved_cards(ctx, &root->containers[1].containers[1]);
	init_button(ctx, &root->containers[1].containers[2]);
	SDL_SetRenderTarget(ctx->display->renderer, NULL);
	endTurn(ctx);
}



