#ifndef SPL_STRUCTS
# define SPL_STRUCTS

# include "SDLX/SDLX.h"

# define MIN_PLAYERS 2
# define MAX_PLAYERS 4
# define MAX_RESERVE 4
# define MAX_ROWCARD 4
# define MAX_TITLES 5

# define CARD_TYPES 4

# define ROW_COUNT 3
# define TOK_COUNT 5

# define TOP_ROW 0
# define MID_ROW 1
# define BOT_ROW 2

# define TOK_A 0
# define TOK_B 1
# define TOK_C 2
# define TOK_D 3
# define TOK_R 4

# define EXIT_GAME -1
# define CONNECT_SCREEN 0
# define PLAYING 1

# define HANDLE_LEN 21

# define DISCONNECTED 0
# define CONNECTED 1
# define READY 2

# define ASSETS "../assets"

typedef struct c_string_vec {
  char **ptr;
  uint64_t len;
  uint64_t cap;
} c_string_vec;

typedef struct Card
{
	SDLX_Sprite sprite;
	uint8_t cost[TOK_COUNT - 1];
	uint8_t points;
	uint8_t type;
	uint8_t id;
}	Card;

typedef struct Player
{
	Card *reserved[MAX_RESERVE];
	uint8_t tokens[TOK_COUNT];
	uint8_t owned[CARD_TYPES];
	uint8_t reserveCount;
	uint8_t status;
	char handle[HANDLE_LEN];
}	Player;

typedef struct PlayerUI
{
	SDL_Rect nameTag;
	SDL_Rect pointsTag;
	SDLX_Sprite ressourceIcon[TOK_COUNT];
	SDLX_Sprite permanentIcon[CARD_TYPES];
	SDLX_Sprite reservedIcon[MAX_RESERVE];
	SDLX_Sprite reservedPrice[CARD_TYPES * (TOK_COUNT - 1)];
}			PlayerUI;

typedef struct Row
{
	uint8_t remainCount;
	uint8_t revealedCount;

	Card	*remaining;
	Card	*revealed[MAX_ROWCARD];
	SDLX_Sprite rowIcon;
}	Row;

typedef struct Board
{
	uint8_t tokens[TOK_COUNT];
	uint8_t remainingTitles;

	PlayerUI playerUI[MAX_PLAYERS];
	Card	titles[MAX_TITLES];
	Row 	rows[ROW_COUNT];
}	Board;


typedef struct Context
{
	uint8_t playerCount;
	uint8_t  turn;
	uint8_t state;

	Board	board;
	Card 	*cards;
	Player players[MAX_PLAYERS];
	SDLX_Display *display;
}	Context;
#endif
