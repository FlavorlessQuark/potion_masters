NAME = client

DEBUG_FLAGS = -fsanitize=address -g
FLAGS = $(DEBUG_FLAGS)
INCLUDES = -I includes/ -I includes/SDL2/ -I includes/SDLX/ -I includes/MT/

LIB_DIR = libs/
STATIC_LIB = -l SDL2 -l SDL2_image -l SDL2_ttf -lm

SDLX_DIR = SDLX/
SRCS_DIR = srcs/
INIT_DIR = init/
BOARD_DIR = board/
CONNECT_DIR = connect/
IO_DIR = I_O/
PLAYER_DIR = player/

BIN_DIR = bin/

SDLX_NAMES = 			\
	SDLX_buttons		\
	SDLX_containers_utils\
	SDLX_containers		\
	SDLX_debug			\
	SDLX_init			\
	SDLX_input			\
	SDLX_render			\
	SDLX_text			\
	SDLX_utils			\

BOARD_NAMES =			\
	board_screen		\
	init_board			\
	fill_board			\

CONNECT_NAMES =			\
	connect_screen		\
	init_connect_screen	\
	fill_connect_screen	\

IO_NAMES =			\
	connection 		\
	messages		\
	window_events	\

PLAYER_NAMES =			\
	init_main_screen	\
	main_screen			\
	fill_main_screen	\



SRCS_NAMES =								\
	$(addprefix $(SDLX_DIR), $(SDLX_NAMES))	\
	init			\
	main 			\
	$(addprefix $(INIT_DIR),   $(INIT_NAMES))	\
	$(addprefix $(BOARD_DIR),  $(BOARD_NAMES))	\
	$(addprefix $(IO_DIR),     $(IO_NAMES))	\
	$(addprefix $(PLAYER_DIR), $(PLAYER_NAMES))	\
	$(addprefix $(CONNECT_DIR),$(CONNECT_NAMES))	\
	utils			\

SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRCS_NAMES)))
OBJS = $(addprefix $(BIN_DIR), $(SRCS:.c=.o))

all: $(NAME)
# all:
# 	echo $(SRCS)
html:
	emcc $(SRCS)  -I includes -I includes/SDL2 -I includes/SDLX -s  USE_SDL=2 \
        -s USE_SDL_TTF=2 \
        -s USE_SDL_IMAGE=2 \
		-s SDL2_IMAGE_FORMATS='["png"]'\
		-s EXPORT_ES6=1 \
        -s MODULARIZE=1 \
        -s ENVIRONMENT=web \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s EXPORTED_FUNCTIONS=_main,_recvMessage,_setConnectionStatus,_resizeWindow,_malloc \
		-s EXPORTED_RUNTIME_METHODS=ccall,stringToUTF8,lengthBytesUTF8,UTF8ToString \
        --preload-file srcs/SDLX/default.ttf \
		--preload-file assets/ \
        -o $(NAME).js
		mv $(NAME).js node/$(NAME).js
		mv $(NAME).data node/$(NAME).data
		mv $(NAME).wasm node/$(NAME).wasm

$(NAME): $(BIN_DIR) $(OBJS)
	gcc $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJS)  $(STATIC_LIB)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)%.o: %.c
	mkdir -p $(BIN_DIR)$(dir $<)
	gcc $(FLAGS) $(INCLUDES) -c $< -o $@

run: re clean
	./$(NAME)

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
