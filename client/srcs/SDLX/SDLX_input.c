#include "SDLX/SDLX.h"
#include <time.h>

typedef struct SDLX_KeyMap
{
	int type;

	int fromKey;
	int toKey;

}	SDLX_KeyMap;

static SDLX_Input _intern_input;
static SDLX_KeyMap *keymap = NULL;

static uint8_t	keyboard_buffer[SDL_NUM_SCANCODES];
static int	keymap_size;
static int	keymap_cap;

void SDLX_InputLoop(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			exit(0);
	}
}

int SDLX_GetKeyState(int key)
{
	int state;
	int prev_state;

	state = _intern_input.keyboard[key];
	prev_state = keyboard_buffer[key];

	state = ((prev_state & 1) << 1) + state;

	return state;
}

void SDLX_InputMap(int keyType, int fromKey, int toKey)
{
	int i;

	if (fromKey >= SDL_NUM_SCANCODES || toKey >= SDL_NUM_SCANCODES)
		return ;

	if (keymap_cap <= keymap_size)
	{
		keymap_cap = (keymap_cap + 1) * 2;
		keymap = realloc(keymap, keymap_cap * sizeof(SDLX_KeyMap));
	}

	keymap[keymap_size].type = keyType;
	keymap[keymap_size].fromKey = fromKey;
	keymap[keymap_size].toKey = toKey;

	keymap_size++;
}

void SDLX_BuildMouseInput(void)
{
	uint32_t	newMouseState;
	uint32_t    mouse[4];

	newMouseState = SDL_GetMouseState(&_intern_input.mouse.x, &_intern_input.mouse.y);
	SDL_GetRelativeMouseState(&_intern_input.mouse_delta.x, &_intern_input.mouse_delta.y);

	mouse[SDL_BUTTON_LEFT] = (newMouseState & SDL_BUTTON_LMASK) >> (SDL_BUTTON_LEFT - 1);
	mouse[SDL_BUTTON_MIDDLE] = (newMouseState & SDL_BUTTON_MMASK) >> (SDL_BUTTON_MIDDLE - 1);
	mouse[SDL_BUTTON_RIGHT] = (newMouseState & SDL_BUTTON_RMASK)  >> (SDL_BUTTON_RIGHT - 1) ;

	_intern_input.mouse_buttons[SDL_BUTTON_LEFT] = ((_intern_input.mouse_buttons[SDL_BUTTON_LEFT] & 1) << 1) + mouse[SDL_BUTTON_LEFT];
	_intern_input.mouse_buttons[SDL_BUTTON_MIDDLE] = ((_intern_input.mouse_buttons[SDL_BUTTON_MIDDLE]) & 1 << 1) + mouse[SDL_BUTTON_MIDDLE];
	_intern_input.mouse_buttons[SDL_BUTTON_RIGHT] = ((_intern_input.mouse_buttons[SDL_BUTTON_RIGHT] & 1)<< 1) + mouse[SDL_BUTTON_RIGHT];

	// SDL_Log("Left %d, Mid %d, Right %d",
	// 	_intern_input.mouse_buttons[SDL_BUTTON_LEFT],
	// 	_intern_input.mouse_buttons[SDL_BUTTON_MIDDLE],
	// 	_intern_input.mouse_buttons[SDL_BUTTON_RIGHT]
	// );
}

void SDLX_BuildKeyMapInput(void)
{
	SDLX_KeyMap currentMapping;
	uint8_t		*keyboard;

	int i;

	i = 0;
	keyboard = SDL_GetKeyboardState(NULL);

	memcpy(keyboard_buffer, _intern_input.keyboard, SDL_NUM_SCANCODES * sizeof(u_int8_t));
	memset(_intern_input.keyboard, 0,  SDL_NUM_SCANCODES * sizeof(uint8_t));
	while (i < keymap_size)
	{
		currentMapping = keymap[i];
		_intern_input.keyboard[currentMapping.toKey] |= keyboard[currentMapping.fromKey];
		i++;
	}

	// SDL_Log("Left %d %d, Right %d %d, Space %d %d",
	// _intern_input.keyboard[SDL_SCANCODE_LEFT], SDLX_GetKeyState(SDL_SCANCODE_LEFT),
	// _intern_input.keyboard[SDL_SCANCODE_RIGHT],SDLX_GetKeyState(SDL_SCANCODE_RIGHT),
	// _intern_input.keyboard[SDL_SCANCODE_SPACE],SDLX_GetKeyState(SDL_SCANCODE_SPACE)
	// );
}

void SDLX_InputUpdate()
{
	SDLX_BuildMouseInput();
	SDLX_BuildKeyMapInput();
}

SDLX_Input SDLX_InputGet() { return _intern_input; }
