#include "SDLX/SDLX.h"

static SDLX_Time _intern_time;

void		SDLX_SpriteCreate(SDLX_Sprite *dest, uint32_t layer, SDL_Texture *texture)
{
	SDL_memset(dest, 0, sizeof(SDLX_Sprite));
	if (dest == NULL)
		return ;
	dest->texture = texture;
	dest->primary_Layer = layer;
	dest->src = &(dest->_src);
	dest->dst = &(dest->_dst);
	dest->angle = 0;
	dest->center = NULL;
	dest->flip = SDL_FLIP_NONE;
}

SDL_Texture	*SDLX_TextureLoad(char *path, SDLX_Display *display)
{
	return 	SDL_CreateTextureFromSurface(display->renderer, IMG_Load(path));
}

int SDLX_PointInCircle(SDL_Point point, SDLX_Circle circle)
{
	return (
		point.x <= circle.center.x + circle.radius &&
		point.x >= circle.center.x - circle.radius &&
		point.y <= circle.center.y + circle.radius &&
		point.y >= circle.center.y - circle.radius
	);
}

int SDLX_TimedLoop(int (*game_loop)(void *), void *args)
{
	Uint32 	current;
	int		i;

	static double consumable;

	current = SDL_GetTicks();
	consumable += current - _intern_time.last_frame;
	consumable = MAX(consumable, UPDATE_LEN_MS);
	i = 0;
	while (consumable > UPDATE_LEN_MS && i < MAX_UPDATE_PER_FRAME)
	{
		// Maybe a preferable approach would be to time the length of the update
		// and either :
		//			- Average it and use an N update / sec
		//			- Deduct it from consumbale, set it as delta time
		_intern_time.delta_time = MIN(consumable, UPDATE_LEN_MS) / UPDATE_LEN_MS;
		game_loop(args);
		consumable -= UPDATE_LEN_MS;
		++i;
	}
	_intern_time.last_frame = current;
	return i;
}

void SDLX_CapFPS()
{
	static Uint32 start;
	int delay;

	if (!start)
		start = SDL_GetTicks();
	delay = start - SDL_GetTicks();
	delay = MAX(FRAME_TIME - delay , 0);
	SDL_Delay(delay);
	start = SDL_GetTicks();
}

SDLX_Time SDLX_TimeGet(void) {return _intern_time;}

