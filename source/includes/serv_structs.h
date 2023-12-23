#ifndef SRV_STRUCTS
# define SRV_STRUCTS

# include "SDLX/SDLX.h"
# include "macros.h"
# include "tcp.h"

# define MIN_PLAYERS (2)
# define MAX_PLAYERS (4)
# define MAX_POTIONS (9)
# define MAX_ROWCARD (4)
# define MAX_MASTER_POTIONS (3)

# define MAX_ACTIONS (3)

# define MAX_FILL (3)

# define HANDLE_LEN (21)

# define STATUS_DISCONNECTED (0)
# define STATUS_CONNECTED (1)
# define STATUS_READY (2)

typedef struct SERV_Potion
{
	uint8_t 	cost[ESSENCE_TYPES];
	uint8_t		type;
	uint8_t		fill;

	char id[CARD_ID_LEN];
	int _id;
}	SERV_Potion;

typedef struct SERV_Player
{
	SERV_Potion 	owned[MAX_POTIONS];
	SERV_Potion 	brewing;
	uint8_t tokens[ESSENCE_TYPES];
	uint8_t potionCount;
	uint8_t status;
	uint8_t isBrewing;
	uint8_t actionsRemaining;
    SOCKET sock;

    char name[MAX_NAME_LEN];
	char handle[HANDLE_LEN];
}	SERV_Player;

typedef struct SERV_Row
{
	uint8_t remainCount;
	uint8_t recipeCount;

	SERV_Potion	recipes[MAX_ROWCARD];
}	SERV_Row;

typedef struct SERV_Board
{
    SERV_Potion		master[MAX_MASTER_POTIONS];
	SERV_Row 		rows[ROW_COUNT];

	uint8_t 	masterCount;
}	SERV_Board;



typedef struct SERV_Context
{
	uint8_t playerCount;
	uint8_t turn;
	uint8_t state;
	uint8_t winner;

    char message[MSG_LEN];

	SERV_Board	board;
	SERV_Player players[MAX_PLAYERS];
}	SERV_Context;



#endif
