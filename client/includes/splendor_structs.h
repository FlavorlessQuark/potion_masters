#ifndef SPL_STRUCTS
# define SPL_STRUCTS

# include "SDLX/SDLX.h"

# define MIN_PLAYERS 2
# define MAX_PLAYERS 4
# define MAX_RESERVE 4
# define MAX_ROWCARD 4
# define MAX_TITLES 5

# define CARD_TYPES 3

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

# define CARD_BACK 0
# define CARD 1
# define TOK_RECT 2
# define TOK_HEX 3

#define OFF_X (198)
#define OFF_Y (225)
#define SEP_Y (133)
#define SEP_X (128)
#define TOK_OFF_X (187)
#define TOK_OFF_Y (150)
#define TOK_W (804)
#define TOK_H (958)
#define CARD_H (2342)
#define CARD_W (1570)
#define CARD_OFF_X (CARD_W + SEP_X)
#define CARD_OFF_Y (CARD_H + (SEP_Y * 2))
# define CARD_VARIATIONS 0

# define CARD_ID_LEN (3 + CARD_TYPES + 2)
#define MSG_LEN (TOK_COUNT * 3) + (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) + (MAX_RESERVE * CARD_ID_LEN + 1) + 1 + 3


#define TOKEN_BUTTON_RESET TOK_COUNT + 0
#define TOKEN_BUTTON_CONFIRM TOK_COUNT + 1
# define MAIN_SCREEN_SPRITE_COUNT MAX_RESERVE + (CARD_TYPES * 2) + 1 + 3 + 1 + 1

# define ASSETS "../assets"


typedef struct Button
{
	SDLX_Sprite sprite;
	SDLX_Button button;
	void *data;
}	Button;
typedef struct Card
{
	SDLX_Sprite sprite;
	SDLX_Sprite costSprite[TOK_COUNT - 1];
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
	uint8_t revealedCount;//RM
	SDLX_Sprite rowIcon;//RM BAKE INTO BG
	Card	revealed[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
}	Row;

typedef struct Board
{
	uint8_t tokens[TOK_COUNT];

	Row 	rows[ROW_COUNT];
	SDLX_Button tokenButton[TOK_COUNT + 2];
	SDLX_Sprite tokenCount[TOK_COUNT];
	SDLX_Sprite tokenTaken[TOK_COUNT];
	SDLX_Button switchMode;
	SDL_Texture *bg;
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
	SDLX_Sprite reserveSprite;
	SDLX_Sprite buySprite;
	SDLX_Sprite costSprite[TOK_COUNT - 1];
	SDLX_Button buyButton;
	SDLX_Button reserveButton;
	SDLX_Button exit;
	Card  		*selected;
	SDL_Texture *bg;
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
	SDL_Texture *bg;
}		PlayerUI;

typedef struct Context
{
	Player player;
	Connection connection;
	Board board;
	int state;
	PlayerUI UI;
	BuyScreen buyscreen;
	SDL_Rect nums;
	SDLX_Display *display;
	SDL_Texture *cardTex;
	SDLX_TextSheet textSheet;
}	Context;
#endif
