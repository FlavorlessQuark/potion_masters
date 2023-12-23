#ifndef _GAME_SCENES_H
#define _GAME_SCENES_H

# include "SDLX/SDLX.h"
# include "macros.h"
# include "entities.h"

//-~-~-~-~-~-~-~-~ Utils -~-~-~-~-~-~-~-~

typedef struct Button
{
	SDLX_Sprite sprite;
	SDLX_Button button;
	void *data;
}	Button;

typedef struct Row
{
	int count;
	Potion	card[MAX_ROWCARD];
	SDLX_Button cardButton[MAX_ROWCARD];
}	Row;

//-~-~-~-~-~-~-~-~ Host -~-~-~-~-~-~-~-~

typedef struct HostInput
{
	int counter;
	SDL_Rect status;
	SDLX_Sprite bg;
	SDLX_Sprite timer;
	SDLX_Sprite playerName[MAX_PLAYERS];
	SDLX_Sprite playerStatus[MAX_PLAYERS];
}	CliInput;

typedef struct HostLobby
{
	int counter;
	SDL_Rect status;
	SDLX_Sprite bg;
	SDLX_Sprite timer;
	SDLX_Sprite playerName[MAX_PLAYERS];
	SDLX_Sprite playerStatus[MAX_PLAYERS];
}	HostInput;


//-~-~-~-~-~-~-~-~ Client -~-~-~-~-~-~-~-~

typedef struct CliInput
{
	int counter;
	SDL_Rect status;
	SDLX_Sprite bg;
	SDLX_Sprite timer;
	SDLX_Sprite playerName[MAX_PLAYERS];
	SDLX_Sprite playerStatus[MAX_PLAYERS];
}	CliInput;

typedef struct CliLobby
{
	int counter;
	SDL_Rect status;
	SDLX_Sprite bg;
	SDLX_Sprite timer;
	SDLX_Sprite playerName[MAX_PLAYERS];
	SDLX_Sprite playerStatus[MAX_PLAYERS];
}	CliLobby;

//-~-~-~-~-~-~-~-~ Core -~-~-~-~-~-~-~-~

typedef struct MainMenu {
    SDLX_Sprite title;
    SDLX_Sprite credit;

    Button host_button;
    Button join_button;
    Button exit_button;

}   MainMenu;

typedef struct EndScreen {
    SDLX_Sprite text;

    Button back;
}  EndScreen;


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

	Potion     *selected;
}			BuyOverlay;


typedef struct BoardScreen
{
	Row 	masterPotions;
	Row 	rows[ROW_COUNT];
	Button  switchScreen;

	BuyOverlay overlay;
	SDL_Texture *bg;
}	Board;


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

typedef struct MainScreen
{
	SDLX_Sprite actions;

	Button endTurn;
	Button switchScreen;

	PotionOverlay overlay;

	SDLX_Button ownedButtons[MAX_POTIONS];
	SDLX_Sprite essences[ESSENCE_TYPES];
	SDL_Texture *bg;
}		MainScreen;

#endif
