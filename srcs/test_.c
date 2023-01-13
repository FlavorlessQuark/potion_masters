#include "../includes/SDLX/SDLX.h"


void inputLoop(SDLX_RectContainer *container)
{
	SDLX_Input input;
	int align;

	input = SDLX_InputGet();

	align = container->alignment;
	if (SDLX_GetKeyState(SDL_SCANCODE_UP) == SDLX_KEYDOWN)
	{
		if (container->alignDirection == SDLX_ALIGN_HORIZONTAL)
			container->alignDirection = SDLX_ALIGN_VERTICAL;
		else
			container->alignDirection = SDLX_ALIGN_HORIZONTAL;
		SDLX_ContainerUpdate(container, NULL);
	}

	if (SDLX_GetKeyState(SDL_SCANCODE_RIGHT) == SDLX_KEYDOWN)
	{
		align++;
		SDLX_ContainerUpdate(container, NULL);
	}
	if (SDLX_GetKeyState(SDL_SCANCODE_LEFT) == SDLX_KEYDOWN)
	{
		align--;
		SDLX_ContainerUpdate(container, NULL);
	}

	if (align < 0)
		align = SDLX_SPACE_BETWEEN;
	if (align > SDLX_SPACE_BETWEEN)
		align = SDLX_RIGHT_ALIGN;
	container->alignment = align;

}

int main()
{
	SDLX_Display *display;
	SDLX_Input input;
	SDLX_RectContainer container;
	SDL_Rect rect;
	SDL_Rect rect2;
	SDLX_Button button;
	SDLX_Button button2;

	SDLX_InitDefault();
	display = SDLX_DisplayGet();

	// RunInputTests();
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_UP, SDL_SCANCODE_UP);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_W, SDL_SCANCODE_UP);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_DOWN, SDL_SCANCODE_DOWN);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_S, SDL_SCANCODE_DOWN);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_LEFT, SDL_SCANCODE_LEFT);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_A, SDL_SCANCODE_LEFT);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_RIGHT, SDL_SCANCODE_RIGHT);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_D, SDL_SCANCODE_RIGHT);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_SPACE, SDL_SCANCODE_SPACE);
	SDLX_InputMap(SDLX_KEYBOARDTYPE, SDL_SCANCODE_P, SDL_SCANCODE_SPACE);

	SDLX_ButtonCreate(&button, &rect);
	SDLX_ButtonCreate(&button2, &rect2);
	SDLX_ButtonSet_Keys(
		SDL_SCANCODE_LEFT,
		SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_UP,
		SDL_SCANCODE_DOWN,
		SDL_SCANCODE_SPACE);

	rect.w = DEFAULT_WIN_W / 4;
	rect.h = DEFAULT_WIN_H / 4;
	rect.x = (DEFAULT_WIN_W / 2) - (rect.w / 2);
	rect.y = (DEFAULT_WIN_H / 2) - (rect.h / 2) - rect.h;

	rect2.w = DEFAULT_WIN_W / 4;
	rect2.h = DEFAULT_WIN_H / 4;
	rect2.x = (DEFAULT_WIN_W / 2) - (rect2.w / 2);
	rect2.y = (DEFAULT_WIN_H / 2) - (rect2.h / 2) + rect2.h;

	SDLX_ButtonSet_Neighbours(&button, NULL, NULL, &button2, &button2);
	SDLX_ButtonSet_Neighbours(&button2, NULL, NULL, &button, &button);

	container.alignDirection = SDLX_ALIGN_VERTICAL;
	container.alignment = SDLX_CENTER_ALIGN;
	container.containerCount = 0;
	container.elemCount = 2;
	container.elems = calloc(2, sizeof(SDLX_ContainerElem));

	container.self._boundingBox = (SDL_Rect){.h = display->win_h , .w = display->win_w, .x = 0, .y = 0};
	// container.self.boundingBox = &container.self._boundingBox;
	SDLX_ContainerElemCreate(&container.self ,NULL, 0, 0, 0);
	SDLX_ContainerElemCreate(&container.elems[0], &rect, 0, 0, 0);
	SDLX_ContainerElemCreate(&container.elems[1], &rect2, 0, 0, 0);
	SDLX_ContainerUpdate(&container, NULL);

	// SDL_Log("Box 1 %p => %p", &rect, container.elems[0].boundingBox);
	SDL_Log("Box 1 (%d,%d) | w: %d, h: %d", rect.x, rect.y, rect.w, rect.h);
	SDL_Log("Box 2 (%d,%d) | w: %d, h: %d", rect2.x, rect2.y, rect2.w, rect2.h);

	while (1)
	{
		SDLX_RenderReset(display);
		SDLX_InputLoop();
		SDLX_InputUpdate();
		input = SDLX_InputGet();
		inputLoop(&container);
		SDLX_ButtonUpdate();
		SDLX_ContainerUpdate(&container, NULL);
		SDL_SetRenderDrawColor(display->renderer,
							   255 * (button.triggered),
							   255 * (button.state == SDLX_FOCUS_STAY),
							   255,
							   255);
		SDL_RenderFillRect(display->renderer, &rect);

		SDL_SetRenderDrawColor(display->renderer,
							   255 * (button2.triggered),
							   255 * (button2.state == SDLX_FOCUS_STAY),
							   255,
							   255);
		SDL_RenderFillRect(display->renderer, &rect2);
		SDL_SetRenderDrawColor(display->renderer,
							   255,
							   0,
							   0,
							   255);
		SDL_RenderDrawRect(display->renderer, container.self.boundingBox);
		SDL_RenderDrawRect(display->renderer, &rect);
		SDL_RenderDrawRect(display->renderer, &rect2);
		SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
		SDL_RenderPresent(display->renderer);
	}
}
