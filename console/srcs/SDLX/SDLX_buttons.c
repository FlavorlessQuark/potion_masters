#include "SDLX/SDLX.h"

static SDLX_Button **buttons;
static size_t capacity;
static size_t size;
static SDLX_Button *currentFocus;
static int keys[5] = {-1, -1, -1, -1, -1};

# define UP 0
# define DOWN 1
# define LEFT 2
# define RIGHT 3
# define SELECT 4

void SDLX_ButtonSet_Neighbours(SDLX_Button *dest, SDLX_Button *left, SDLX_Button *right, SDLX_Button *up, SDLX_Button *down)
{
	dest->neighbours[UP] = up;
	dest->neighbours[DOWN] = down;
	dest->neighbours[LEFT] = left;
	dest->neighbours[RIGHT] = right;
}

void SDLX_ButtonSet_Keys(int left, int right, int up, int down, int select)
{
	keys[UP] = up;
	keys[DOWN] = down;
	keys[LEFT] = left;
	keys[RIGHT] = right;
	keys[SELECT] = select;
}

void SDLX_ButtonCreate (SDLX_Button *dest, SDL_Rect *boundingBox)
{
	if (size >= capacity)
	{
		capacity = (capacity * 2) + 1;
		buttons = realloc(buttons, sizeof(SDLX_Button *) * capacity);
	}
	buttons[size] = dest;

	if (!boundingBox)
		dest->boundingBox = &dest->_boundingBox;
	else
		dest->boundingBox = boundingBox;

	dest->state = SDLX_NONE;
	dest->enabled = SDL_TRUE;
	dest->triggered = SDL_FALSE;
	//Memset faster ?
	dest->neighbours[0] = NULL;
	dest->neighbours[1] = NULL;
	dest->neighbours[2] = NULL;
	dest->neighbours[3] = NULL;

	size++;
}

void SDLX_ButtonUpdate()
{
	size_t i;
	int isTriggered;
	int mouseOver;
	SDLX_Input input;

	i = 0;
	input = SDLX_InputGet();

	while(i < 4)
	{
		if (currentFocus && keys[i] != -1 && SDLX_GetKeyState(keys[i]) == SDLX_KEYDOWN && currentFocus->neighbours[i] != NULL)
		{
			// SDL_Log("Here %d", i);
			currentFocus = currentFocus->neighbours[i];
			break ;
		}
		i++;
	}

	i = 0;
	// TODO: Only do bit things on unsigned types :)
	while (i < size)
	{
		buttons[i]->state = ((buttons[i]->state & 1) << 1);
		buttons[i]->triggered = ((buttons[i]->triggered & 1) << 1);
		mouseOver = SDL_PointInRect(&input.mouse, buttons[i]->boundingBox);

		if (buttons[i]->enabled == SDL_TRUE && (mouseOver || buttons[i] == currentFocus))
		{
			buttons[i]->state += 1;

			// if (currentFocus != buttons[i])
			// 	currentFocus->triggered <<= 1;

			currentFocus = buttons[i];
			// >:) hehe
			isTriggered = ((input.mouse_buttons[SDL_BUTTON_LEFT] & 1) * mouseOver) | (input.keyboard[keys[SELECT ]] & 1);
			buttons[i]->triggered += (isTriggered) * !!(input.mouse_buttons[SDL_BUTTON_LEFT] - buttons[i]->triggered - SDLX_KEYHELD);
		}
		i++;
	}
}
