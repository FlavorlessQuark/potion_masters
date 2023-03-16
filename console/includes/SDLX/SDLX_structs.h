/**
 *  FILE: SDLX_structs.h
 *  AUTHOR:
		FlavorlessQuark
 *  CONTENTS:
		Structures used within SDLX
 *  CREATION: 2022-05-10 16:47:11
 *  MODIFIED: 2022-05-10 23:29:14
 */

#ifndef SDLX_STRUCT_H
# define SDLX_STRUCT_H

#include "SDL2/SDL.h"

 typedef struct SDLX_Display
{
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Texture     *background;
	SDL_Color		bgColor;

    TTF_Font        *defaultFont;

    int             win_w;
    int             win_h;
}   SDLX_Display;

/**
 * @brief SDLX_Sprite has both a _dst variable and a dst pointer
 * This is in order to allow another variable to be "linked" to that pointer and modify it
 * as well as giving the ability to null it entirely
 * SDLX shall only use the pointer version of this variable
 * SDLX_Sprite_Create shall  default *src to _src and *dst to _dst
 * A NULL *dst shall behave like an SDL NULL dst (image is rendered to the whole screen)
 * A NULL *src shall behave like an SDL NULL src (image source is the entire file)
 */

typedef struct SDLX_Sprite
{
    uint32_t    primary_Layer;
    uint32_t    secondary_layer;

    SDL_Rect _dst;
    SDL_Rect *dst;
    SDL_Rect _src;
    SDL_Rect *src;

    SDL_Texture *texture;
    SDL_Point   *center;
    SDL_Point   _center;

    SDL_RendererFlip    flip;

    double      angle;
}   SDLX_Sprite;

typedef struct SDLX_RenderQueue
{
    uint64_t    capacity;
    uint64_t    size;

    SDL_bool    is_sorted_queue;
    SDLX_Sprite **sprites;
}   SDLX_RenderQueue;

typedef struct SDLX_Input
{
    SDL_Point   mouse;
    uint32_t    mouse_buttons[4];
	SDL_Point	mouse_delta;

	uint8_t		keyboard[SDL_NUM_SCANCODES];
}   SDLX_Input;

typedef struct SDLX_Time
{
    Uint32     delta_time;
    Uint32     last_frame;
    uint32_t    frame_count;
}   SDLX_Time;

typedef struct SDLX_Button
{
	SDL_Rect _boundingBox;
	SDL_Rect *boundingBox;

	uint8_t state;
	uint8_t triggered;
	uint8_t enabled;

	struct SDLX_Button *neighbours[4];
}	SDLX_Button;

typedef struct SDLX_ContainerElem
{
	SDL_Rect	*boundingBox;
	SDL_Rect	_boundingBox;

	SDL_Rect	_origin;

	int			widthTYpe;
	int			heightTYpe;

	int			margin;
}				SDLX_ContainerElem;

typedef struct SDLX_RectContainer
{
	SDLX_ContainerElem self;
	SDLX_ContainerElem *elems;

	struct SDLX_RectContainer *containers;

	int elemCount;
	int containerCount;
	int alignment;
	int alignDirection;// H / V
	int innerWidth;
	int innerHeight;
}				SDLX_RectContainer;

typedef struct SDLX_Circle
{
    SDL_Point   center;
    int         radius;
}   SDLX_Circle;

typedef struct SDLX_TextSheet
{
	SDL_Texture *tex;
	SDL_Rect next;
	int maxW, maxH;
}				SDLX_TextSheet;

#endif
