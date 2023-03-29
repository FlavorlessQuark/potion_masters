#include "../includes/splendor.h"

# define SPRITE_TOP (i * 2 + 0)
# define SPRITE_BOT (i * 2 + 1)

void init_status_bar(Context *ctx, SDLX_RectContainer *root)
{
	SDL_Color color = {255,255,255,255};

	SDLX_SpriteCreate(&ctx->UI.name, 1, NULL);
	SDLX_SpriteCreate(&ctx->UI.points, 1, NULL);
	root->elems[0]._boundingBox.h /= 2;
	ctx->UI.name._dst = root->elems[0]._boundingBox;
	ctx->UI.points._dst = root->elems[0]._boundingBox;
	ctx->UI.points._src = ctx->nums;
	ctx->UI.points.texture = ctx->textSheet.tex;
	SDLX_RenderMessage(ctx->display, &root->elems[0]._boundingBox, color, "POINTS :");
	ctx->UI.points._dst.x += ctx->UI.points._dst.w;
	ctx->UI.points._dst.w = ctx->UI.points._dst.h;
	ctx->player.points = 0;
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
		ctx->UI.permanents[i]._dst = scaleAndCenter(0.5,root->containers[i].elems[0]._boundingBox, root->containers[i].elems[0]._boundingBox);
		ctx->UI.tokens[i]._dst = scaleAndCenter(0.5,root->containers[i].elems[1]._boundingBox, root->containers[i].elems[1]._boundingBox);
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
}

void init_reserved_cards(Context *ctx, SDLX_RectContainer *root)
{
	uint8_t buttonOffset;
	SDL_Point start;

	SDL_SetRenderDrawColor(ctx->display->renderer, 255,255,0,255);
	for (int i = 0; i < root->containerCount; i++)
	{
		SDLX_SpriteCreate(&ctx->player.reserved[SPRITE_TOP].sprite, 1, ctx->cardTex);
		SDLX_SpriteCreate(&ctx->player.reserved[SPRITE_BOT].sprite, 1, ctx->cardTex);
		SDLX_ButtonCreate(&ctx->UI.reserved[SPRITE_TOP], ctx->player.reserved[SPRITE_TOP].sprite.dst);
		SDLX_ButtonCreate(&ctx->UI.reserved[SPRITE_BOT], ctx->player.reserved[SPRITE_BOT].sprite.dst);
		ctx->player.reserved[SPRITE_TOP].sprite._dst = root->containers[i].elems[0]._boundingBox;
		ctx->player.reserved[SPRITE_BOT].sprite._dst = root->containers[i].elems[1]._boundingBox;
		ctx->player.reserved[SPRITE_TOP].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctx->player.reserved[SPRITE_TOP].sprite._dst.w, ctx->player.reserved[SPRITE_TOP].sprite._dst.h);
		ctx->player.reserved[SPRITE_BOT].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctx->player.reserved[SPRITE_BOT].sprite._dst.w, ctx->player.reserved[SPRITE_BOT].sprite._dst.h);
		SDL_SetTextureBlendMode(ctx->player.reserved[SPRITE_TOP].sprite.texture , SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(ctx->player.reserved[SPRITE_BOT].sprite.texture , SDL_BLENDMODE_BLEND);
		ctx->player.reserved[SPRITE_TOP].sprite.src = NULL;
		ctx->player.reserved[SPRITE_BOT].sprite.src = NULL;
		start.x = ctx->player.reserved[SPRITE_TOP].sprite._dst.x;
		start.y = ctx->player.reserved[SPRITE_TOP].sprite._dst.y;
		draw_dotted_rect(start,ctx->player.reserved[SPRITE_TOP].sprite._dst.w, ctx->player.reserved[SPRITE_TOP].sprite._dst.h, ctx->player.reserved[SPRITE_TOP].sprite._dst.w / 20);

		start.x = ctx->player.reserved[SPRITE_BOT].sprite._dst.x;
		start.y = ctx->player.reserved[SPRITE_BOT].sprite._dst.y;
		draw_dotted_rect(start,ctx->player.reserved[SPRITE_BOT].sprite._dst.w, ctx->player.reserved[SPRITE_BOT].sprite._dst.h, ctx->player.reserved[SPRITE_BOT].sprite._dst.w / 20);
		// ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[0]._boundingBox;
		// *offset+= 1;
		// ctx->mainscreen->UI[*offset]._dst = root->containers[i].elems[1]._boundingBox;
		// *offset+= 1;
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0,0,0,255);
}\

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
	SDL_SetRenderTarget(ctx->display->renderer, NULL);
	endTurn(ctx);
}



