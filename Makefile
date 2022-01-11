# NAME
NAME		=	pipex
#NAME_B =

# DIR
PATH_SRC	=	srcs/
PATH_INC	=	inc/
PATH_O		=	temp_o/
O_DBG_PATH	=	temp_o/dbg
O_FDBG_PATH	=	temp_o/fdbg
LIB_PATH	=	libft/
#PATH_BONUS = ./bonus/

# COMPILE
CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -I$(PATH_INC) -I$(LIB_PATH)
DBGFLAGS	=	-g3
FDBGFLAGS	=	-g3 -fsanitize=address
DEPEND		=	$(LIB_PATH)libft.h \

DEPEND		+=	$(addprefix include/, pipex.h)

# SOURCES
SOURCES		=	test.c
SRCS		=	$(addprefix $(PATH_SRC), $(SOURCES))
#SRC_B =

# OBJECTS
OBJECTS		=	$(SOURCES:.c=.o)
OBJS		=	$(addprefix $(PATH_O), $(OBJECTS))
DBG_OBJS	=	$(addprefix $(O_DBG_PATH), $(OBJECTS))
FDBG_OBJS	=	$(addprefix $(O_FDBG_PATH), $(OBJECTS))
#OBJ_B = $(SRC_B:.c=.o)

all					:	libft
					@echo "Building $(NAME):"
					@make --no-print-directory $(NAME)

$(NAME)			:	$(OBJS)
					@echo "Linking $(NAME)..."
					@$(CC) $(CFLAGS) $^ -o $@
					@echo "$(NAME) built successfully!"

$(OBJS)			:	$(PATH_O)%.o: $(PATH_SRC)%.c $(DEPEND)
					@echo "Compiling $<"
					$@mkdir -p $(@D)
					@$(CC) $(CFLAGS) -c $< -o$@

debug			:	libft
					@echo "[DEBUG] Building $(NAME):"
					@make --no-print-directory build_debug

build_debug		:	$(DBG_OBJS)
					@echo "[DEBUG] Linking $(NAME)..."
					@$(CC) $(CFLAGS) $(DBGFLAGS) -c $< -o $@

$(DBG_OBJS)		:	$(O_DBG_PATH)%.o: $(PATH_SRC)%.c $(DEPEND)
					@echo "Compiling $<"
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(DBGFLAGS) -c $< -o $@

fdebug			:	libft
					@echo "[DEBUG] Building $(NAME):"
					@make --no-print-directory build_debug

build_debug		:	$(FDBG_OBJS)
					@echo "[DEBUG] Linking $(NAME)..."
					@$(CC) $(CFLAGS) $(FDBGFLAGS) -c $< -o $@

$(FDBG_OBJS)	:	$(O_FDBG_PATH)%.o: $(PATH_SRC)%.c $(DEPEND)
					@echo "Compiling $<"
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(FDBGFLAGS) -c $< -o $@

libft			:
					@make --noprint-directory -C $(LIB_PATH)

clean			:
					@echo "$(NAME): cleaning objs"
					rm -r -f $(PATH_O)

fclean			:	clean
					@echo "$(NAME): cleaning binary"
					rm -r -f $(NAME)


allclean		:	fclean
				@make fclean --no-print-directory -C $(LIB_PATH)

re				:	fclean all


.PHONY			:	all debug fdebug libft sanitize norm clean fclean allclean re
