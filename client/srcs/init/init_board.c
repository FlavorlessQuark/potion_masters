#include "../includes/splendor.h"

# define TOKEN_ROW 0
# define TOP_CARD_ROW 1
# define MID_CARD_ROW 2
# define BOT_CARD_ROW 3
# define ROW_CARD_COUNT 4

# define CARDS 0
# define TOKENS 1
# define SWITCH 2

void initTokens(Context *ctx, SDLX_RectContainer *root)
{
	SDL_Rect src;

	for (int i = 0; i < TOK_COUNT; i++)
	{
		SDLX_SpriteCreate(&ctx->board.tokenCount[i], 1, ctx->textSheet.tex);
		SDLX_SpriteCreate(&ctx->board.tokenTaken[i], 1, ctx->textSheet.tex);
		SDLX_ButtonCreate(&ctx->board.tokenButton[i], ctx->board.tokenCount[i].dst);
		ctx->board.tokenCount[i]._src = ctx->nums;
		ctx->board.tokenTaken[i]._src = ctx->nums;
		root->elems[i]._boundingBox.w = root->elems[i]._boundingBox.h;

		src = (SDL_Rect){.h = 53, .w = CARD_W / 2,
						 .x = (SEP_X + 5) + (CARD_W / 2 + SEP_X) * i, .y =  (CARD_H * 2) + SEP_Y * 3 + 35};
		SDL_RenderCopy(ctx->display->renderer, ctx->cardTex, &src, &root->elems[i]._boundingBox);

		ctx->board.tokenCount[i]._dst = root->elems[i]._boundingBox;
		ctx->board.tokenTaken[i]._dst = root->elems[i]._boundingBox;
		ctx->board.tokenTaken[i]._dst.x += ctx->board.tokenTaken[i]._dst.w;
	}
	for (int i = TOK_COUNT; i < TOK_COUNT + 2; i++)
	{
		root->elems[i]._boundingBox.w = root->elems[i]._boundingBox.h;
		SDLX_ButtonCreate(&ctx->board.tokenButton[i], NULL);
		ctx->board.tokenButton[i]._boundingBox = root->elems[i]._boundingBox;
	}
	SDLX_RenderMessage(ctx->display, &root->elems[TOK_COUNT]._boundingBox, (SDL_Color){255,255,255}, "X");
	SDLX_RenderMessage(ctx->display, &root->elems[TOK_COUNT + 1]._boundingBox, (SDL_Color){255,255,255}, "V");

}

void initRowCards(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	//Parse texture data here
	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon.texture = ctx->cardTex;
	row->rowIcon.src->x = SEP_X;
	row->rowIcon.src->y = SEP_Y;
	row->rowIcon.src->h = CARD_H;
	row->rowIcon.src->w = CARD_W;
	row->rowIcon._dst = root->elems[0]._boundingBox;
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		SDLX_SpriteCreate(&row->revealed[i].sprite, 1, ctx->cardTex);
		row->cardButton[i]._boundingBox = root->elems[i + 1]._boundingBox;
		row->revealed[i].sprite._dst = root->elems[i + 1]._boundingBox;
		row->revealed[i].sprite._src.h = CARD_H;
		row->revealed[i].sprite._src.w = CARD_W;
		for (int n = 0; n < CARD_TYPES; n++)
		{
			SDLX_SpriteCreate(&row->revealed[i].costSprite[n], 1, ctx->textSheet.tex);
			row->revealed[i].costSprite[n].dst->x = row->revealed[i].sprite.dst->x + row->revealed[i].sprite.dst->w / 10;
			row->revealed[i].costSprite[n].dst->y = row->revealed[i].sprite.dst->y + (row->revealed[i].sprite.dst->h / 4 * n);
			row->revealed[i].costSprite[n].dst->w = row->revealed[i].sprite.dst->w / 10;
			row->revealed[i].costSprite[n].dst->h = row->revealed[i].sprite.dst->h / 5;
			row->revealed[i].costSprite[n]._src = ctx->nums;

		}
	}

}

void init_board_screen(Context *ctx)
{
	SDLX_RectContainer *root;

	root = parseUI("assets/UI/boardUI");

	ctx->board.bg = SDL_CreateTexture(ctx->display->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		ctx->display->win_w, ctx->display->win_h
		);

	SDL_SetRenderTarget(ctx->display->renderer, ctx->board.bg);

	initTokens(ctx, &root->containers[TOKENS]);
	initRowCards(ctx, &ctx->board.rows[TOP_ROW], &root->containers[CARDS].containers[TOP_ROW]);
	initRowCards(ctx, &ctx->board.rows[MID_ROW], &root->containers[CARDS].containers[MID_ROW]);
	initRowCards(ctx, &ctx->board.rows[BOT_ROW], &root->containers[CARDS].containers[BOT_ROW]);
	SDLX_ButtonCreate(&ctx->board.switchMode, NULL);
	ctx->board.switchMode._boundingBox.x = root->containers[SWITCH].elems[0]._boundingBox.x;
	ctx->board.switchMode._boundingBox.y = root->containers[SWITCH].elems[0]._boundingBox.y;
	ctx->board.switchMode._boundingBox.w = root->containers[SWITCH].elems[0]._boundingBox.w;
	ctx->board.switchMode._boundingBox.h = root->containers[SWITCH].elems[0]._boundingBox.h;

	SDL_SetRenderTarget(ctx->display->renderer, NULL);

}
