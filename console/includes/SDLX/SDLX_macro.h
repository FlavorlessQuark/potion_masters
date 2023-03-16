#ifndef SDLX_MACRO
# define SDLX_MACRO

# define DEFAULT_SDL_FLAG 0
# define DEFAULT_WIN_NAME "Default"
# define DEFAULT_WIN_X SDL_WINDOWPOS_CENTERED
# define DEFAULT_WIN_Y SDL_WINDOWPOS_CENTERED
# define DEFAULT_WIN_W 1920
# define DEFAULT_WIN_H 1080

// Render queue macros

# define DEFAULT_QUEUES_COUNT 2
# define DEFAULT_QUEUE_SIZE 5

// Frame and update macros

# define FPS 60
# define FRAME_TIME 1000 / FPS
# define UPDATE_LEN_MS 10
# define MAX_UPDATE_PER_FRAME 30


// Text macros

# define DEFAULT_FONT_SIZE 20
# define DEFAULT_FONT_COLOR (SDL_Color){255, 255, 255}
# define DEFAULT_FONT_PATH "./srcs/SDLX/default.ttf"

enum {
	SDLX_RIGHT_ALIGN = 1,
	SDLX_CENTER_ALIGN,
	SDLX_LEFT_ALIGN,
	SDLX_TOP_ALIGN,
	SDLX_BOTTOM_ALIGN,
	SDLX_SPACE_EVEN,
	SDLX_SPACE_BETWEEN,
	SDLX_ALIGN_VERTICAL,
	SDLX_ALIGN_HORIZONTAL
};

enum {
	SDLX_MOUSETYPE,
	SDLX_KEYBOARDTYPE,
	SDLX_CONTROLLERTYPE
};

enum {
	SDLX_NONE = 0,
	SDLX_KEYDOWN,
	SDLX_KEYUP,
	SDLX_KEYHELD
};


enum {
	SDLX_FOCUS_ENTER = 1,
	SDLX_FOCUS_LEAVE,
	SDLX_FOCUS_STAY,
	SDLX_SELECTED
};


# ifndef MAX
#  define MAX(a, b)\
	({ 	__typeof__ (a) _a = (a);\
		__typeof__ (b) _b = (b);\
		(_a > _b) ? (a) : (b);})\

# endif

# ifndef MIN
#  define MIN(a, b)			\
({ 	__typeof__ (a) _a = (a);\
	__typeof__ (b) _b = (b);\
	(_a < _b) ? (a) : (b);})\

# endif

#endif
