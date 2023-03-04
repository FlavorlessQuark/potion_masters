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

#define SEP_Y 13
#define SEP_X 13
#define CARD_H 233
#define CARD_W 136
#define CARD_OFF_X CARD_W + SEP_X
#define CARD_OFF_Y CARD_H + (SEP_Y * 2)
# define CARD_VARIATIONS 0

# define CARD_ID_LEN (3 + CARD_TYPES + 2)
# define MSG_LEN TOK_COUNT + (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) + 2


#define TOKEN_BUTTON_RESET TOK_COUNT + 0
#define TOKEN_BUTTON_CONFIRM TOK_COUNT + 1
# define MAIN_SCREEN_SPRITE_COUNT MAX_RESERVE + (CARD_TYPES * 2) + 1 + 3 + 1 + 1

# define ASSETS "../assets"

typedef struct Card
{
	SDLX_Sprite sprite;
	uint8_t cost[TOK_COUNT - 1];
	uint8_t points;
	uint8_t type;
	int _id;
	char id[CARD_ID_LEN];
}	Card;

typedef struct Player
{
	Card reserved[MAX_RESERVE];
	uint8_t tokens[TOK_COUNT];
	uint8_t owned[CARD_TYPES];
	uint8_t reserveCount;
	uint8_t id;
	uint8_t points;
}	Player;

typedef struct Row
{
	uint8_t revealedCount;
	SDLX_Sprite rowIcon;
	Card	revealed[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
}	Row;

typedef struct Board
{
	uint8_t tokens[TOK_COUNT];

	Row 	rows[ROW_COUNT];
	SDLX_Button tokenButton[TOK_COUNT + 2];
	SDLX_Sprite tokenSprites[TOK_COUNT + 2];
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
	Card  *selected;
	uint8_t cardOrigin; // -1 = no card, 0 = board, 1 reserverd index
}			BuyScreen;

typedef struct PlayerUI
{
	SDLX_Button reserved[MAX_RESERVE];
	SDLX_Button switchMode;
	//			 REserved card + tokens + owned + golden + point display + no card + Buy
	// SDLX_Sprite UI[MAIN_SCREEN_SPRITE_COUNT];
	SDLX_Sprite name;
	SDLX_Sprite points;
	SDLX_Sprite permanents[CARD_TYPES];
	SDLX_Sprite tokens[TOK_COUNT];

}		PlayerUI;

typedef struct Context
{
	Player player;
	Connection connection;
	Board board;
	int state;
	PlayerUI UI;
	BuyScreen buyscreen;
	SDLX_Display *display;
	SDL_Texture *cardTex;
}	Context;
#endif
