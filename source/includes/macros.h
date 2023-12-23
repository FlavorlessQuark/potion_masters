#ifndef MACROS_H
# define MACROS_H

# define EXIT_GAME (-1)
# define GAME_TITLE (0)
# define GAME_MENU (1)
# define GAME_LOBBY_IN (2)
# define GAME_LOBBY_WAIT (3)
# define GAME_MAIN (4)
# define GAME_END (5)

# define TOK_A 0
# define TOK_B 1
# define TOK_C 2
# define TOK_D 3
# define TOK_R 4

# define MASTER_ROW 0
# define LVL2_ROW 1
# define LVL1_ROW 2
# define BASE_ROW 3

# define POTION_TYPES (3)

# define ROW_COUNT (3)

# define MAX_BREWING (3)
# define ESSENCE_TYPES (4)

# define MAX_NAME_LEN 20

# define POTION_TYPES (20)

# define ROW_COUNT (3)

# define MAX_POTIONS (9)
# define MAX_ROWCARD (4)
# define MAX_MASTER_POTIONS (4)
# define MAX_BREWING (3)
# define ESSENCE_TYPES (4)

# define MAX_COST_TYPES (3)

# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF
# define TRANSPARENT 0x00000000


# define MIN_PLAYERS (2)
# define MAX_PLAYERS (4)

# define NUMS	"-0123456789"
# define NMATHC	"0123456789-+()x*=/ "

# define ASSETS "../assets"

# define CARD_ID_LEN ((sizeof(char) * 5) + (ESSENCE_TYPES * (sizeof(char) * 3)) - 1)
# define PLAYER_STATE_LEN (sizeof(char)+ \
						 sizeof(char) * 4 + \
						(sizeof(char) * 2 * ESSENCE_TYPES) + \
						(sizeof(char) * 2 + (MAX_POTIONS)  * CARD_ID_LEN) + \
						(sizeof(char) * 2 + CARD_ID_LEN)   + \
						 sizeof(char) * 2 + \
						 sizeof(char) * 2)

# define BOARD_STATE_LEN (sizeof(char) * 2 + (MAX_MASTER_POTIONS) * CARD_ID_LEN) + \
						(ROW_COUNT * (sizeof(char) * 6 + (MAX_POTIONS * CARD_ID_LEN)))



# define MSG_LEN PLAYER_STATE_LEN + BOARD_STATE_LEN

#endif
