#ifndef SPL_STRUCTS
# define SPL_STRUCTS

# include "SDLX/SDLX.h"
# include "scenes_structs.h"

# define ASSETS "../assets"

typedef struct Assets
{
	SDL_Texture *connectBg;
	SDL_Texture *mainBg;
	SDL_Texture *recipeBg;
	SDL_Texture *texPotions;
	SDL_Texture *texUI;
	SDL_Texture *essences;
	SDL_Texture *potions;
	SDL_Texture *texUI;
	SDL_Texture *overlayBG;
	SDL_Texture *main_bg;
	SDL_Texture *board_bg;
	SDL_Texture *shelf;
	SDL_Texture *recipe;

}	Assets;


typedef struct Connection
{
    int hasMessage;
    char message[MSG_LEN];
}              Connection;

typedef struct Context
{
	uint8_t playerCount;
	uint8_t turn;
	uint8_t state;
	uint8_t winner;

	SDLX_Display 	*display;

    Connection connection;
	Player  player;
	Assets  assets;
}	Context;



#endif
