#ifndef SPL_STRUCTS
# define SPL_STRUCTS

# include "SDLX/SDLX.h"

#define PLAYER_STATE_LEN (sizeof(char)+ \
						 sizeof(char) * 4 + \
						(sizeof(char) * 2 * ESSENCE_TYPES) + \
						(sizeof(char) * 2 + (MAX_POTIONS)  * CARD_ID_LEN) + \
						(sizeof(char) * 2 + CARD_ID_LEN)   + \
						 sizeof(char) * 2 + \
						 sizeof(char) * 2)

#define BOARD_STATE_LEN (sizeof(char) * 2 + (MAX_MASTER_POTIONS) * CARD_ID_LEN) + \
						(ROW_COUNT * (sizeof(char) * 6 + (MAX_POTIONS * CARD_ID_LEN)))



#define MSG_LEN PLAYER_STATE_LEN + BOARD_STATE_LEN


# define MIN_PLAYERS (2)
# define MAX_PLAYERS (4)
# define MAX_POTIONS (10)
# define MAX_ROWCARD (4)
# define MAX_MASTER_POTIONS (4)
# define MAX_BREWING (3)
# define ESSENCE_TYPES (4)

# define MAX_COST_TYPES (3)
# define CARD_ID_LEN ((sizeof(char) * 8) + (MAX_COST_TYPES * (sizeof(char) * 4)) + 1)

# define POTION_TYPES (3)

# define ROW_COUNT (3)

# define MASTER_ROW 0
# define LVL2_ROW 1
# define LVL1_ROW 2
# define BASE_ROW 3

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


// #define MSG_LEN (ESSENCE_TYPES * 3) + (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) + (MAX_RESERVE * CARD_ID_LEN + 1) + 1 + 3


#define TOKEN_BUTTON_RESET ESSENCE_TYPES + 0
#define TOKEN_BUTTON_CONFIRM ESSENCE_TYPES + 1
# define MAIN_SCREEN_SPRITE_COUNT MAX_RESERVE + (POTION_TYPES * 2) + 1 + 3 + 1 + 1

# define ASSETS "../assets"


typedef struct Button
{
	SDLX_Sprite sprite;
	SDLX_Button button;
	void *data;
}	Button;

typedef struct Potion
{
	SDLX_Sprite sprite;
	SDLX_Sprite costSprite[ESSENCE_TYPES - 1];
	uint8_t cost[ESSENCE_TYPES - 1];
	uint8_t points;
	uint8_t type;
	int _id;
	char id[CARD_ID_LEN];
}	Potion;

typedef struct Player
{
	Potion reserved[MAX_POTIONS];
	uint8_t tokens[ESSENCE_TYPES];
	uint8_t owned[POTION_TYPES];
	uint8_t reserveCount;
	uint8_t id;
	uint8_t points;
	uint8_t actionsRemaining;
}	Player;

typedef struct Row
{
	Potion	card[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
}	Row;

typedef struct BuyOverlay
{
	SDLX_Sprite background;
	SDLX_Sprite potion;
	SDLX_Sprite essences[ESSENCE_TYPES];

	SDL_Rect nameDst;
	SDL_Rect costDst;
	SDL_Rect descDst;

	Button buy;
	Button exit;

	Potion  	*selected;
}			BuyOverlay;


typedef struct Board
{
	Row 	masterPotions;
	Row 	rows[ROW_COUNT];
	Button  switchScreen;

	BuyOverlay overlay;
	SDL_Texture *bg;
}	Board;

typedef struct Connection
{
	char message[MSG_LEN];
	uint8_t status;
	uint8_t hasMessage;
	SDLX_Button connectButton;
}			Connection;


typedef struct PlayerUI
{
	SDLX_Sprite name;
	SDLX_Sprite points;
	Potion 		owned[MAX_POTIONS];
	SDLX_Sprite essences[ESSENCE_TYPES];
	SDL_Texture *bg;
}		PlayerUI;

typedef struct Assets
{
	SDL_Rect numberDimensions;
	SDLX_Display *display;
	SDL_Texture *cardTex;
	SDLX_TextSheet textSheet;
}			  Assets;

typedef struct Context
{
	int state;
	Player player;
	PlayerUI UI;
	Connection connection;
	Board board;
	Assets assets;
	SDLX_Display *display;
}	Context;
#endif
