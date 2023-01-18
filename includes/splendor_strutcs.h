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
	SDLX_Button reservedButton[MAX_ROWCARD];
	uint8_t tokens[TOK_COUNT];
	uint8_t owned[CARD_TYPES];
	uint8_t reserveCount;
}	Player;

typedef struct Row
{
	uint8_t remainCount;
	uint8_t revealedCount;

	Card	*remaining;
	Card	*revealed[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
	SDLX_Sprite rowIcon;
}	Row;

typedef struct Board
{
	uint8_t tokens[TOK_COUNT];
	uint8_t remainingTitles;

	Card	titles[MAX_TITLES];
	Row 	rows[ROW_COUNT];
}	Board;


typedef struct Context
{
	uint8_t playerCount;
	uint8_t  turn;

	Board	board;
	Card 	*cards;
	Player players[MAX_PLAYERS];
	SDLX_Display *display;
}	Context;
#endif
