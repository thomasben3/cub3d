NAME		=	cub3D

SRCS		= 	srcs/main.c \
				srcs/parsing/parsing.c srcs/parsing/parse_textures.c srcs/parsing/parse_map.c \
				srcs/parsing/fill_map.c srcs/parsing/gnl.c \
				srcs/sprites/sprites.c srcs/sprites/draw_sprite.c srcs/sprites/attack_and_move_sprite.c \
				srcs/sprites/sprites_utils.c srcs/sprites/create_sprites.c srcs/sprites/load_sprites_imgs.c \
				srcs/engine/render.c srcs/engine/check_N_S_E_W.c srcs/engine/draw.c srcs/engine/loop_hook.c \
				srcs/engine/key_and_mouse_events.c srcs/engine/player_controls.c srcs/engine/player_skills.c \
				srcs/engine/doors_and_generators.c srcs/engine/minimap_and_life.c srcs/engine/engine_utils.c \
				srcs/utils/utils.c srcs/utils/list.c srcs/utils/color.c srcs/utils/init.c srcs/utils/free_and_exit.c \

OBJS 		=	${SRCS:.c=.o}

UNAME		:=	$(shell uname)

CC			=	gcc

RM			=	rm -f

CFLAGS		=	-Wall -Wextra -Werror #-g3 -fsanitize=address

PATH_MLX	=	mlx_linux

LINKFLAGS	=	-Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

ifeq ($(UNAME), Darwin)
PATH_MLX	=	mlx
LINKFLAGS	=	-Imlx -L${PATH_MLX} -lmlx -framework OpenGL -framework AppKit
%.o: %.c
			$(CC) $(CFLAGS) -DZ=13 -DQ=0 -DS=1 -DD=2 -DLEFT=123 -DRIGHT=124 -DESC=53 -DESP=49 -DRCLICK=2 -DP=35 -DENTER=36 -Imlx -c $< -o $@
else
%.o: %.c
			$(CC) $(CFLAGS) -DZ=119 -DQ=97 -DS=115 -DD=100 -DLEFT=65361 -DRIGHT=65363 -DESC=65307 -DESP=32 -DRCLICK=3 -DP=112 -DENTER=65293 -I/usr/include -Imlx_linux -O3 -c $< -o $@
endif


all: ${NAME}

$(NAME): 	$(OBJS)
			$(MAKE) -C ${PATH_MLX}
			${CC} ${CFLAGS} ${PATH_MLX}/libmlx.a ${OBJS} ${LINKFLAGS} -o ${NAME}

clean:
			${RM} ${OBJS}

fclean: clean
			${RM} ${NAME}
			make -C $(PATH_MLX) clean

re: fclean all

.PHONY:		bonus all clean fclean re
