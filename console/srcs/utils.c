#include "../includes/splendor.h"

Card *findCard(Context *ctx, char *id, int _id)
{
	int level;
	int i;

	i = 0;
	level = id[0] - '0';
	SDL_Log("Try find %d", _id);
	for (i = 0; i < MAX_ROWCARD; i++)
	{
		SDL_Log("CMP %s[%d] -> %s[%d]",
		ctx->board.rows[level].revealed[i].id, ctx->board.rows[level].revealed[i]._id,
		id, _id);
		if (ctx->board.rows[level].revealed[i]._id == _id)
			return &ctx->board.rows[level].revealed[i];
	}
	return NULL;
}

void nextTurn(Context *ctx)
{
	int i;

	i = (ctx->turn + 1) % MAX_PLAYERS;
	while (i != ctx->turn)
	{
		if (ctx->players[i].status == READY)
		{
			ctx->turn = i;
			break ;
		}
		i = (i + 1) % MAX_PLAYERS;
	}
	send_game_state(ctx, ctx->turn);
}

void startGame(Context *ctx)
{
	ctx->turn = 0;
	initNewGame(ctx);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ctx->players[i].status == READY)
			send_to(ctx->players[i].handle, "s");
	}
	send_game_state(ctx, ctx->turn);
}


int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}

SDL_Rect scaleAndCenter(double scalar, SDL_Rect parent, SDL_Rect this)
{
	SDL_Rect result;

	result.w = this.w * scalar;
	result.h = this.h * scalar;
	result.x = parent.x + ((parent.w / 2) - (result.w / 2));
	result.y = parent.y + ((parent.h / 2) - (result.h / 2));

	return result;
}

void delReserved(Player *player, int cardId)
{
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		if (player->reserved[i]._id == cardId)
		{
			player->reserved[i] = player->reserved[player->reserveCount - 1];
			player->reserveCount--;
			break ;
		}
	}
}

void generateCardTexture(SDL_Texture *base, Card *card, int type)
{
	SDL_Rect src;
	SDL_Rect dst;
	SDL_Rect centereDst;
	SDL_Renderer *renderer;
	SDL_Texture *target;
	char text[2];
	int remainder;

	text[1] = '\0';
	renderer = SDLX_DisplayGet()->renderer;
	target = SDL_GetRenderTarget(renderer);
	get_img_src(&src, CARD, type);

	dst.h = card->sprite.dst->h / 7;
	dst.w = dst.h;
	dst.x = card->sprite.dst->w / 10;
	dst.y = card->sprite.dst->h / 10;


	if (card->sprite.texture == NULL)
	{
		SDL_Log(" !! New card (%d) has no texture", card->_id);
		card->sprite.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, card->sprite._dst.w, card->sprite._dst.h);
		SDL_SetTextureBlendMode(card->sprite.texture , SDL_BLENDMODE_BLEND);
	}


	SDL_SetRenderTarget(renderer, card->sprite.texture);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderCopy(renderer, base, &src, NULL);
	SDL_Log("Texture fro card %s", card->id);
	for (int i = 0; i < CARD_TYPES; i++)
	{
		if (card->cost[i] > 0)
		{
			get_img_src(&src, TOK_HEX, i);

			centereDst = scaleAndCenter(0.5, dst, dst);
			// SDL_Log("Token %d x%d  (%s)", i, card->);
			SDL_RenderCopy(renderer, base, &src, &dst);
			remainder = card->cost[i];
			if (remainder >= 5)
			{
				remainder -= 5;
				SDLX_RenderMessage(SDLX_DisplayGet(), &centereDst,(SDL_Color){0,0,0,0}, "5");
				dst.x += dst.w / 5;
			}
			for (int x = 0; x < remainder; x++)
			{
				SDL_RenderCopy(renderer, base, &src, &dst);
				dst.x += dst.w / 5;
			}
			// SDLX_RenderMessage(SDLX_DisplayGet(), &centereDst,(SDL_Color){255,255,255,255}, text);
			dst.y += card->sprite.dst->h / 5;
		}
		dst.x = card->sprite.dst->w / 10;
	}
	dst.x = card->sprite._dst.w * 0.70;
	dst.y = dst.y = card->sprite.dst->h / 10;
	text[0] = card->points + '0';
	SDLX_RenderMessage(SDLX_DisplayGet(), &dst,(SDL_Color){255,255,255,255}, text);
	SDL_SetRenderTarget(renderer, target);
}

int generateCard(SDL_Texture *base, Card *card, int level)
{

	int type;
	int variation;
	int arr[TOK_COUNT - 1] = {0,1,2,3};
	int singleMax = 3 + (2 * level);
	int totalMax = 6 + (2 * level);
	int length = TOK_COUNT - 1;
	int cost;
	int i;

	type = rand() % CARD_TYPES;
	variation = 0;


	while (totalMax > 0 && length > 0)
	{
		i = rand() % length;
		cost = rand() % MIN(singleMax, totalMax);
		totalMax -= cost;
		card->cost[arr[i]] = cost;
		arr[i] = arr[length - 1];
		length -= 1;
	}

	if (level == 0)
		card->points = (rand() % 100) > 75;
	else if (level == 1)
		card->points = (rand() % 2);
	else
		card->points = (rand() % 4);
	card->id[0] = level + '0';
	card->id[1] = type + '0';
	card->id[2] = variation + '0';
	card->id[3] = card->cost[TOK_A] + '0';
	card->id[4] = card->cost[TOK_B] + '0';
	card->id[5] = card->cost[TOK_C] + '0';
	card->id[6] = card->cost[TOK_D] + '0';
	card->id[7] = card->points + '0';
	card->id[8] = '\0';



	extract_num(card->id, &card->_id);
	generateCardTexture(base, card, type);
	// SDL_Log("Generate %s  (%d)",
	// 	card->id,
	// 	card->_id
	// );
	SDL_Log("Generate %s  (%d) | Src (%d,%d) Cost: %d %d %d %d, points %d",
		card->id,
		card->_id,
		card->sprite._src.x, card->sprite._src.y,
		card->cost[0],
		card->cost[1],
		card->cost[2],
		card->cost[3],
		card->points
	);

	return 1;
}


void print_config(Context *ctx, SDLX_RectContainer *root)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);
	SDL_RenderDrawRect(ctx->display->renderer, root->self.boundingBox);

	for (int i = 0; i < root->containerCount; i++)
		print_config(ctx, &root->containers[i]);

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 255,255);
	for (int x = 0; x < root->elemCount; x++)
	{
		SDL_RenderDrawRect(ctx->display->renderer, root->elems[x].boundingBox);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0,0,255);
}

SDLX_RectContainer *loadConfig(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}


void cleanupUIConfig(SDLX_RectContainer *container)
{
	for (int i = 0; i < container->containerCount; i++)
		cleanupUIConfig(&container->containers[i]);
	if (container->containerCount > 0)
		SDL_free(container->containers);
	if (container->elemCount > 0)
		SDL_free(container->elems);
}

void cleanup(Context *ctx)
{
	SDL_free(ctx);
}

void get_img_src(SDL_Rect *dst, int imageType, int index)
{
	if (index < 0 || index >= CARD_TYPES)
		return ;
	if (imageType == CARD_BACK)
	{
		dst->w = CARD_W;
		dst->h = CARD_H;
		dst->x = OFF_X + (index * (CARD_W + SEP_X));
		dst->y = OFF_Y;
	}
	else if (imageType == CARD)
	{
		dst->w = CARD_W;
		dst->h = CARD_H;
		dst->x = OFF_X + (index * (CARD_W + SEP_X));
		dst->y = OFF_Y + CARD_H + SEP_Y;
	}
	else if (imageType == TOK_HEX)
	{
		dst->w = TOK_W;
		dst->h = TOK_H;
		dst->x = OFF_X + (CARD_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_OFF_X;
		dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
	}
	else if (imageType == TOK_RECT)
	{
		dst->w = TOK_W;
		dst->h = TOK_H;
		dst->x = OFF_X + (CARD_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_W + (TOK_OFF_X * 2);
		dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
	}
}
