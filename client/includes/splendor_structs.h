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

# define PLAYERSTATUS 0
# define CARDSELECT 1
# define BUYMENU 2
# define CONNECT 3

# define DISCONNECTED 0
# define CONNECTED 1
# define HASMESSAGE 2

# define ASSETS "../assets"

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
	uint8_t id;
}	Player;

typedef struct Row
{
	uint8_t revealedCount;

	Card	*revealed[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
}	Row;

typedef struct Board
{
	uint8_t tokens[TOK_COUNT];
	uint8_t remainingTitles;

	Card	titles[MAX_TITLES];
	Row 	rows[ROW_COUNT];
	SDLX_Button tokenTake[TOK_COUNT];
	SDLX_Button tokenRemove[TOK_COUNT];
	SDLX_Sprite tokenSprites[TOK_COUNT];
}	Board;

typedef struct Connection
{
	char *message;
	uint8_t status;
}			Connection;

typedef struct BuyScreen
{
	Card  *selected;
	SDLX_Sprite showSelected;
	SDLX_Button buyButton;
	SDLX_Button exit;
	SDLX_Button addToken[TOK_COUNT];
	SDLX_Button delToken[TOK_COUNT];
}			BuyScreen;

typedef struct BuyMode
{
	SDLX_Button switToBuy;
	SDLX_Button switToCards;
	Board	board;
}				BuyMenu;

typedef struct Context
{
	Card 	*cards;
	Player player;
	Connection connection;
	Board board;
	int state;
	SDLX_Display *display;
}	Context;
#endif