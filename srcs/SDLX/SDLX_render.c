#include "SDLX/SDLX.h"

typedef struct _internal_Queues
{
	uint32_t count;
	SDLX_RenderQueue *queues;
}	_intern_Queues;

static _intern_Queues queues;

void SDLX_RenderQueuesInit()
{
	uint32_t i;

	i = 0;
	queues.queues = SDL_calloc(DEFAULT_QUEUES_COUNT, sizeof(SDLX_RenderQueue));
	while (i < DEFAULT_QUEUES_COUNT)
	{
		queues.queues[i].sprites = SDL_calloc(DEFAULT_QUEUE_SIZE, sizeof(SDLX_Sprite *));
		queues.queues[i].capacity = DEFAULT_QUEUE_SIZE;
		queues.queues[i].size = 0;
		++i;
	}
	queues.count = DEFAULT_QUEUES_COUNT;
}


void 		SDLX_RenderAll(SDLX_Display *display)
{
	uint32_t i;
	uint32_t n;
	SDLX_RenderQueue *current;

	i = 0;
	while (i < queues.count)
	{
		current = &(queues.queues[i]);
		n = 0;
		// SDL_Log("Queue n %d has %ld", i, current->size);
		while (n < current->size)
		{
			SDL_RenderCopyEx(
				display->renderer,
				current->sprites[n]->texture,
				current->sprites[n]->src,
				current->sprites[n]->dst,
				current->sprites[n]->angle,
				current->sprites[n]->center,
				current->sprites[n]->flip
			);
			++n;
		}
		++i;
	}
}

void 		SDLX_RenderOne(uint32_t id);

void        SDLX_RenderReset(SDLX_Display *display)
{
	SDL_RenderClear(display->renderer);
	SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
	if (display->background)
		SDL_RenderCopy(display->renderer, display->background, NULL, NULL);
}

void		SDLX_RenderQueuePush(SDLX_Sprite *sprite)
{
	SDLX_RenderQueue *current;

	if (sprite->primary_Layer < queues.count)
	{
		current = &(queues.queues[sprite->primary_Layer]);
		if (current->size >= current->capacity)
		{
			current->capacity *= 2;
			current->sprites = realloc(current->sprites, current->capacity * sizeof(SDLX_Sprite *));
		}
		current->sprites[current->size] = sprite;
		current->size++;
	}
}

void		SDLX_RenderQueueFlushAll()
{
	uint32_t i;

	i = 0;
	while (i < queues.count)
	{
		queues.queues[i].size = 0;
		++i;
	}
}

void        SDLX_RenderQueueFlush(uint32_t id)
{
	if (queues.count < id)
		queues.queues[id].size = 0;
}

uint32_t    SDLX_RenderQueueCreate(SDL_bool isSorted);

SDLX_RenderQueue *SDLX_RenderQueueGet(uint32_t id)
{
	if (id < queues.count)
		return &queues.queues[id];

	return NULL;
}

// 0 if renderered, 1 if error

void SDLX_RenderMessageAligned(SDLX_Display *display, int x_align, int y_align, SDL_Color color, char *text)
{
	SDL_Surface *surf;
	SDL_Rect 	dst;

	if (!display->defaultFont)
	{
		SDL_Log("No font");
		return ;
	}
	surf = TTF_RenderText_Solid(display->defaultFont, text, color);
	TTF_SizeText(display->defaultFont, text, &dst.w, &dst.h);
	if (x_align == SDLX_RIGHT_ALIGN)
		dst.x = 0;
	else if (x_align == SDLX_LEFT_ALIGN)
		dst.x = display->win_w - dst.w;
	else if (x_align == SDLX_CENTER_ALIGN)
		dst.x = (display->win_w / 2) - (dst.w / 2);

	if (y_align == SDLX_TOP_ALIGN)
		dst.y = 0;
	else if (y_align == SDLX_BOTTOM_ALIGN)
		dst.y = display->win_h - dst.h;
	else if (y_align == SDLX_CENTER_ALIGN)
		dst.y = (display->win_h / 2) - (dst.h / 2);

	SDL_RenderCopy(display->renderer, SDL_CreateTextureFromSurface(display->renderer, surf),
		NULL, &dst);
	SDL_FreeSurface(surf);
}

void SDLX_RenderMessage(SDLX_Display *display, SDL_Rect *dst, SDL_Color color, char *text)
{
	SDL_Surface *surf;

	surf = TTF_RenderText_Solid(display->defaultFont, text, color);

	SDL_RenderCopy(display->renderer, SDL_CreateTextureFromSurface(display->renderer, surf),
		NULL, dst);
	SDL_FreeSurface(surf);
}
