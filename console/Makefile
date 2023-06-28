NAME = console_bin

# DEBUG_FLAGS = -fsanitize=address -g
DEBUG_FLAGS = -g
FLAGS = $(DEBUG_FLAGS)
INCLUDES = -I includes/ -I includes/SDL2/ -I includes/SDLX/ -I includes/MT/

LIB_DIR = libs/
STATIC_LIB = -l SDL2 -l SDL2_image -l SDL2_ttf -lm -ldl -lpthread
# STATIC_LIB = libc_controlpads.a -l SDL2 -l SDL2_image -l SDL2_ttf -lm -ldl -lpthread

SDLX_DIR       = SDLX/
CONNECTION_DIR = connection/
GRAPHIC_DIR    = graphics/
LOGIC_DIR      = logic/
SRCS_DIR       = srcs/

BIN_DIR = bin/

SDLX_NAMES = 			\
	SDLX_buttons		\
	SDLX_containers		\
	SDLX_debug			\
	SDLX_init			\
	SDLX_input			\
	SDLX_render			\
	SDLX_text			\
	SDLX_utils			\

CONNECTION_NAMES = 		\
	connection			\

GRAPHIC_NAMES = 		\
	UI/parser_utils		\
	UI/ui_parser		\
	init				\
	render				\
	utils				\

LOGIC_NAMES =			\
		core			\
		init			\
		messages		\
		utils			\

SRCS_NAMES =											\
	$(addprefix $(SDLX_DIR), 		$(SDLX_NAMES))		\
	$(addprefix $(CONNECTION_DIR),  $(CONNECTION_NAMES))\
	$(addprefix $(GRAPHIC_DIR),		$(GRAPHIC_NAMES))	\
	$(addprefix $(LOGIC_DIR),		$(LOGIC_NAMES))		\
	main 												\

TEST_NAMES =											\
	$(addprefix $(SDLX_DIR), 		$(SDLX_NAMES))		\
	graphics/UI/parser_utils		\
	graphics/UI/ui_parser		\
	test_main 					\

SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRCS_NAMES)))
TESTSRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(TEST_NAMES)))

OBJS = $(addprefix $(BIN_DIR), $(SRCS:.c=.o))
TESTOBJS = $(addprefix $(BIN_DIR), $(TESTSRCS:.c=.o))

all: $(NAME)
# all:
# 	echo $(SRCS)
$(NAME): $(BIN_DIR) $(OBJS)
	gcc $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJS)  $(STATIC_LIB)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)%.o: %.c
	mkdir -p $(BIN_DIR)$(dir $<)
	gcc $(FLAGS) $(INCLUDES) -c $< -o $@

ui:$(BIN_DIR) $(TESTOBJS)
	gcc $(FLAGS) $(INCLUDES) -o UI_test $(TESTOBJS) $(STATIC_LIB)

run: re clean
	./$(NAME)

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
