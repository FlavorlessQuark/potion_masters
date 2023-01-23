NAME = console_bin

DEBUG_FLAGS = -fsanitize=address -g
FLAGS = $(DEBUG_FLAGS)
INCLUDES = -I includes/ -I includes/SDL2/ -I includes/SDLX/ -I includes/MT/

LIB_DIR = libs/
STATIC_LIB = -l SDL2 -l SDL2_image -l SDL2_ttf -lm

SDLX_DIR = SDLX/
SRCS_DIR = srcs/

BIN_DIR = bin/

SDLX_NAMES = 			\
	SDLX_buttons		\
	SDLX_containers		\
	SDLX_debug			\
	SDLX_init			\
	SDLX_input			\
	SDLX_render			\
	SDLX_utils			\

SRCS_NAMES =								\
	$(addprefix $(SDLX_DIR), $(SDLX_NAMES))	\
	config_parser		\
	game				\
	init				\
	input 				\
	main 				\
	utils				\

SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRCS_NAMES)))
OBJS = $(addprefix $(BIN_DIR), $(SRCS:.c=.o))

all: $(NAME)
# all:
# 	echo $(SRCS)

html:
	emcc srcs/test.c srcs/SDLX/*.c -I includes -I includes/SDL2 -I includes/SDLX -s USE_SDL=2 \
        -s USE_SDL_TTF=2 \
        -s WASM=0 \
		-s EXPORT_ES6=1 \
        -s MODULARIZE=1 \
        -s ENVIRONMENT=web \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s EXPORTED_FUNCTIONS=_main,_test_send,_test_receive \
		-s EXPORTED_RUNTIME_METHODS=ccall,stringToUTF8,lengthBytesUTF8 \
        --preload-file srcs/SDLX/default.ttf \
        -o draw.js

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
