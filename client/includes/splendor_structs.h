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
# define MAX_POTIONS (9)
# define MAX_ROWCARD (4)
# define MAX_MASTER_POTIONS (4)
# define MAX_BREWING (3)
# define ESSENCE_TYPES (4)

# define MAX_COST_TYPES (3)
# define CARD_ID_LEN ((sizeof(char) * 5) + (ESSENCE_TYPES * (sizeof(char) * 3)) - 1)


# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF
# define TRANSPARENT 0x00000000

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
# define BOARD 1
# define CONNECT 2
# define END 3

# define DISCONNECTED 0
# define CONNECTED 1
# define READY 2

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
#define CARD_OFF_Y (CARD_H + (SEP_Y * 2))// USE SNPRINTF YOU IDIOT


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
	SDLX_Sprite e_costSprite[ESSENCE_TYPES];
	SDLX_Sprite p_costSprite[3];
	int cost[ESSENCE_TYPES];
	int type;
	int fill;
	int _id;
	char id[CARD_ID_LEN];
}	Potion;

typedef struct Player
{
	Potion owned[MAX_POTIONS];
	Potion brewing;
	int tokens[ESSENCE_TYPES];
	int ownedCount;
	uint8_t id;
	int isBrewing;
	uint8_t points;
	int actionsRemaining;
}	Player;

typedef struct Row
{
	int count;
	Potion	card[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
}	Row;

typedef struct BuyOverlay
{
	//  Must be sprite otherwise render order will be messed up
	SDLX_Sprite bg;
	SDLX_Sprite potion;
	SDLX_Sprite name;
	SDLX_Sprite effect;
	SDLX_Sprite cost;
	SDLX_Sprite recycle;
	SDLX_Sprite ecost;
	SDLX_Sprite pcost;
	SDLX_Sprite rcost;

	Button buy;
	Button exit;

	uint8_t position;

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
	SDLX_Sprite connectSprite;
	SDLX_Sprite bg;
	SDLX_Sprite name;
}			Connection;

typedef struct PotionOverlay {

	Button use;
	Button exit;
	Button convert;

	uint8_t position;
	SDLX_Sprite bg;
	SDLX_Sprite name;
	SDLX_Sprite potion;
	SDLX_Sprite effect;
	SDLX_Sprite recyle;
	SDLX_Sprite fillAmount;

	Potion  	*selected;
}				PotionOverlay;

typedef struct PlayerUI
{
	SDLX_Sprite actions;

	Button endTurn;
	Button switchScreen;

	PotionOverlay overlay;

	SDLX_Button ownedButtons[MAX_POTIONS];
	SDLX_Sprite essences[ESSENCE_TYPES];
	SDL_Texture *bg;
}		PlayerUI;

typedef struct Assets
{
	SDL_Rect numberDimensions;
	SDLX_Display *display;
	SDL_Texture *potions;
	SDL_Texture *texUI;
	SDL_Texture *overlayBG;
	SDL_Texture *essence;
	SDL_Texture *main_bg;
	SDL_Texture *board_bg;
	SDL_Texture *shelf;
	SDL_Texture *recipe;
}			  Assets;

typedef struct Context
{
	int state;
	int isTurn;
	int winner;
	Player player;
	PlayerUI mainUI;
	Connection connection;
	Board board;
	Assets assets;
	SDLX_Display *display;
}	Context;
#endif
