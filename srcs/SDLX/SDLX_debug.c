#include "SDLX/SDLX.h"

void SDLX_SpritePrint(SDLX_Sprite *sprite)
{
	SDL_Log("Sprite: primary %d secondary %d \
			\n_dst : {x: %d, y: %d, w: %d, h: %d},\
			\n_src : {x: %d, y: %d, w: %d, h: %d},\
			\n*dst address : %p, *src address : %p, center address %p,\
			\ncenter : {x: %d, y: %d},\
			\ntexture : %p, angle: %f, flip %d\
			",
			sprite->primary_Layer, sprite->secondary_layer,
			sprite->_dst.x, sprite->_dst.y, sprite->_dst.w, sprite->_dst.h,
			sprite->_src.x, sprite->_src.y, sprite->_src.w, sprite->_src.h,
			sprite->dst, sprite->src, sprite->center,
			sprite->_center.x , sprite->_center.y,
			sprite->texture, sprite->angle, sprite->flip
			);

	if (sprite->dst)
		SDL_Log("*dst : {x: %d, y: %d, w: %d, h: %d}",
		sprite->dst->x, sprite->dst->y, sprite->dst->w, sprite->dst->h);
	if (sprite->src)
		SDL_Log("*src : {x: %d, y: %d, w: %d, h: %d}",
		sprite->src->x, sprite->src->y, sprite->src->w, sprite->src->h);
}

# include <time.h>
float SDLX_CPUTime_Function(void (*fn)(void *),void *data,  int iter)
{
	clock_t before;
	float 	total;
	int i;

	while (i < iter)
	{
		before = clock();
		fn(data);
		total += (float)(clock() - before) / CLOCKS_PER_SEC;
		++i;
	}
	return total;
}
