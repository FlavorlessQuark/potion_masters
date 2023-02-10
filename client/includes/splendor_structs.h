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
# define CONNECTEDCONSOLE 2

# define MSG_LEN 20
# define MAIN_SCREEN_SPRITE_COUNT MAX_RESERVE + (CARD_TYPES * 2) + 1 + 3 + 1 + 1

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

	Row 	rows[ROW_COUNT];
	SDLX_Button tokenTake[TOK_COUNT];
	SDLX_Button tokenRemove[TOK_COUNT];
	SDLX_Sprite tokenSprites[TOK_COUNT];
	SDLX_Button switchMode;
}	Board;

typedef struct Connection
{
	char message[MSG_LEN];
	uint8_t status;
	uint8_t hasMessage;
	SDLX_Button connectButton;
}			Connection;

typedef struct BuyScreen
{
	SDLX_Sprite showSelected;
	SDLX_Button buyButton;
	SDLX_Button reserveButton;
	SDLX_Button exit;
	SDLX_Button addToken[TOK_COUNT];
	SDLX_Button delToken[TOK_COUNT];
	Card  *selected;
	uint8_t cardOrigin; // -1 = no card, 0 = board, 1 reserverd index
}			BuyScreen;

typedef struct MainScreen
{
	SDLX_Button reserved[MAX_RESERVE];
	SDLX_Button switchMode;
	//			 REserved card + tokens + owned + golden + point display + no card + Buy
	SDLX_Sprite UI[MAIN_SCREEN_SPRITE_COUNT];
}		MainScreen;

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
	MainScreen *mainscreen;
	BuyScreen *buyscreen;
	SDLX_Display *display;
}	Context;
#endif
