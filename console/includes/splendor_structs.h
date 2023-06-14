#ifndef SPL_STRUCTS
# define SPL_STRUCTS

# include "SDLX/SDLX.h"

# define MIN_PLAYERS (2)
# define MAX_PLAYERS (4)
# define MAX_POTIONS (10)
# define MAX_ROWCARD (4)
# define MAX_MASTER_POTIONS (4)

# define ESSENCE_TYPES (4)

# define CARD_ID_LEN (3 + POTION_TYPES + 2)

# define POTION_TYPES (3)

# define ROW_COUNT (3)

# define EXIT_GAME (-1)
# define CONNECT_SCREEN (0)
# define PLAYING (1)
# define TITLE (2)

# define HANDLE_LEN (21)

# define DISCONNECTED (0)
# define CONNECTED (1)
# define READY (2)

# define NUMS	"-0123456789"
# define NMATHC	"0123456789-+()x*=/ "

# define ASSETS "../assets"

typedef struct Potion
{
	SDLX_Sprite sprite;
	uint8_t 	cost[ESSENCE_TYPES - 1];
	uint8_t 	points;
	uint8_t		type;

	char id[CARD_ID_LEN];
	int _id;
}	Potion;

typedef struct Player
{
	Potion 	owned[MAX_POTIONS];
	uint8_t tokens[ESSENCE_TYPES];
	uint8_t potionCount;
	uint8_t status;
	uint8_t points;

	SDL_Rect pointsTag;
	SDL_Rect potions[MAX_POTIONS];

	SDLX_Sprite pointSprite;
	SDLX_Sprite ressources[ESSENCE_TYPES];

	char handle[HANDLE_LEN];
}	Player;

typedef struct Row
{
	uint8_t remainCount;
	uint8_t revealedCount;

	Potion		revealed[MAX_ROWCARD];
	SDLX_Sprite rowIcon;
}	Row;

typedef struct Board
{
	Potion		titles[MAX_MASTER_POTIONS];

	SDLX_Sprite essenceUI[ESSENCE_TYPES];
	Row 		rows[ROW_COUNT];
}	Board;

typedef struct ConnectScreen
{
	int counter;
	SDL_Rect status;
	SDL_Texture *buttons;
	SDLX_Sprite playerSprites[MAX_PLAYERS];
}	ConnectScreen;

typedef struct Assets
{
	SDL_Texture *Potions;
	SDL_Texture *UI;
	SDLX_TextSheet text;

	SDL_Rect textSrc;

}	Assets;

typedef struct Context
{
	uint8_t playerCount;
	uint8_t turn;
	uint8_t state;

	SDLX_Display 	*display;

	ConnectScreen connectscreen;
	Board	board;
	Player players[MAX_PLAYERS];
	Assets assets;
}	Context;
#endif
