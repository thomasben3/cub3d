/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 00:51:24 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:52:59 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define DEBUG			1

# define SCREEN_WIDTH	1080
# define SCREEN_HEIGHT	720
# define CUBE_SIZE		1024
# define FOV			66
# define FISHEYE		0

# define V_MOV_SPEED	3
# define H_MOV_SPEED	1.5
# define ROTATION_SPEED	(1)

# define DEGREE			0.01745329252
# define NORTH			0
# define SOUTH			1
# define EAST			2
# define WEST			3
# define FLOOR			4
# define CEILING		5

# define CHOPPER		0
# define LUFFY			1
# define TEACH			2

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "../mlx/mlx.h"

typedef struct s_vars
{
	void	*mlx;
	void	*win;

	char	*config_file;
	char	**map;
	int		map_width;
	int		map_height;
	int		ray_max_len;
}				t_vars;

typedef struct s_img
{
	void	*img;
	int		*imgptr;
	int		height;
	int		width;
	int		bit_per_px;
	int		size_line;
	int		endian;
}				t_img;

typedef struct s_generator
{
	int		map_x;
	int		map_y;

	long	timer;
	int		wave;
	int		nb_gen;
}				t_generator;

typedef struct s_sprite_text
{
	t_img	text[6];
	t_img	attack_text[2];
	t_img	dead_text[4];
}				t_sprite_text;

typedef struct s_sprite
{
	double			x;
	double			y;
	double			z;

	int				x_on_screen;
	int				y_on_screen;

	double			angle;

	int				status;
	int				life;
	double			attack_x;
	double			attack_y;
	t_sprite_text	*text;
	t_img			*text_in_use;

	int				attack_damage;
	float			speed;
	int				dead_position;

	long			last_frame;
	int				calls;
	int				count;
	int				power;
}				t_sprite;

typedef struct s_norm
{
	int		x;
	int		y;
	int		tmp;
	int		end_x;
	double	text_x;
	double	text_y;
}				t_norm;

typedef struct s_list
{
	struct s_list	*next;
	t_sprite		sprite;
}				t_list;

typedef struct s_door
{
	int		status;
	int		x;
	int		y;
	int		count;
	float	start;
	long	last_frame;
}				t_door;

typedef struct s_ray
{
	int		nb;
	double	angle;
	double	ry;
	double	rx;
	double	ryo;
	double	rxo;
	double	length;
	int		line_start;
	int		line_end;
	int		line_height;
}				t_ray;

typedef struct s_player
{
	double		x;
	double		y;

	double		dir;
	double		dx;
	double		dy;

	int			life;
	int			mana;
	int			in_jump;
	int			jump_calls;
	double		attack_dir_x;
	double		attack_dir_y;
	int			attack;
}				t_player;

typedef struct s_keys
{
	int			z;
	int			q;
	int			s;
	int			d;
	int			right;
	int			left;
	int			esp;
	int			left_click;
	int			enter;
}				t_keys;

typedef struct s_data
{
	t_vars			vars;
	t_player		player;
	t_keys			keys;
	t_img			img[6];
	t_img			frame;
	t_list			*sprites;
	t_sprite_text	sprite_text[3];
	t_ray			ray_save[SCREEN_WIDTH];
	int				pause;
	int				horizon_line;
	double			camera_height;
	time_t			last_frame_time;
	int				fps;
	t_img			hands[4];
	int				hands_to_display;
	t_door			**doors;
	t_img			door_text;
	t_generator		**generators;
}				t_data;

//	main.c :
long		get_timestamp(void);
int			is_in_map(t_data *data, int x, int y);

/*------------------------ ENGINE -----------------------*/
//	minimap_and_life.c :
void		draw_mini_map(t_data *data);
void		draw_life(t_data *data, int i, int y, int x);

//	key_and_mouse_events.c :
int			key_press(int keycode, void *data1);
int			key_release(int keycode, void *keys1);
int			mouse_move(int x, int y, void *data1);
int			mouse_click(int button, int x, int y, void *data1);

//	check_N_S_E_W.c :
void		check_north(t_data *data, t_ray *ray);
void		check_south(t_data *data, t_ray *ray);
void		check_east(t_data *data, t_ray *ray);
void		check_west(t_data *data, t_ray *ray);

//	draw.c :
int			create_rgb(int r, int g, int b);
void		draw_text_vline(t_data *data, t_img *img, t_ray ray, int x_in_text);
void		draw_vline(t_ray *ray, int end, t_data *data, int color);
void		draw_floor(t_data *data, t_ray ray, t_img *text, int x);
void		draw_sky(int x, int start, int end, t_data *data);

//	render.c :
void		draw_rays(t_data *data);

//	loop_hook :
void		make_frame(t_data *data);
int			render_next_frame(void *data1);

//	doors_and_generators.c :
int			there_is_door(t_data *data, double rx, double ry, int face);
void		manage_doors(t_data *data);
void		manage_generators(t_data *data, t_generator **generator);

//	player_controls.c :
void		bonus_controls(t_data *data, long *timer, int *calls);
int			player_can_move(t_data *data, double x, double y);
void		move_player_h(t_data *data);
void		move_player_v(t_data *data);
void		rotate_camera(t_data *data);

//	player_skills.c :
void		jump(t_data *data);
void		haki_des_rois(t_data *data);

//	engine_utils.c :
double		distance(double ax, double ay, double bx, double by);
double		fix_angle(double f);
void		display_wave_nb(t_data *data);
void		pause_pressed(t_data *data);

/*------------------------ PARSING -----------------------*/
//	parsing.c :
int			is_map_surrounded(t_vars *vars, char **map);
int			count_component(char **map, char c);
void		init_player(t_data *data, int x, int y, char dir);
int			parse_all(t_data *data, char *path, int description_map_start);

//	parse_texture.c :
int			parse_identifier(t_data *data, int fd, int *description_map_start);

//	parse_map.c :
int			parse_map(t_data *data,
				int fd, int description_map_start, char *config_path);

//	fill_map.c :
int			fill_map(t_data *data);

//	gnl.c :
char		*get_next_line(int fd);
int			ft_strlen(char *str);

/*----------------------- SPRITES ----------------------*/
//	sprites.c :
int			there_is_sprite(t_data *data, t_sprite *sprite, int x, int y);
void		draw_sprites(t_data *data);

//	draw_sprites.c :
void		draw_sprite(t_data *data, t_sprite *s, t_img *t, double scale);
t_img		*get_sprite_texture(
				t_player *player, t_sprite *sprite, double angle);

//	attack_and_move_sprite.c :
void		move_sprite(
				t_data *data, t_sprite *sprite, double scale, t_img **texture);
void		attack_sprite(t_data *data, t_sprite *sprite);

//	create_sprites.c :
t_sprite	new_teach(t_data *data, int x, int y);
t_sprite	new_chopper(t_data *data, int x, int y);
t_sprite	new_luffy(t_data *data, int x, int y);

//	sprites_utils :
int			sprite_is_facing_left(t_data *data, t_sprite *sprite);
void		free_dead_sprites(t_list **list);
void		sort_list(t_data *data, t_list **list);
double		calculate_sprite_angle(t_data *data, t_sprite *sprite);

//	load_sprites_imgs :
void		load_img(t_data *data, t_img *img, char *path);
void		load_chopper_imgs(t_data *data, t_sprite_text *chopper);
void		load_luffy_imgs(t_data *data, t_sprite_text *luffy);
void		load_teach_imgs(t_data *data, t_sprite_text *teach);

/*----------------------- UTILS ----------------------*/
//	color.c :
int			create_trgb(int t, int r, int g, int b);
int			shadow_color(int color, int distance);

//	utils.c :
void		free_double_array(char **array);
int			ft_putstr_error(char *str);
void		push_back(t_list **list, t_sprite sprite);
char		*ft_join(char *s1, char *s2);
char		*ft_itoa(int n);

//	list.c :
void		list_clear(t_list **list);
void		push_back(t_list **list, t_sprite sprite);

//	init.c :
void		init_keys(t_keys *keys);
void		init_hands(t_data *data);

//	free_and_exit.c :
void		free_textures(t_data *data);
void		free_all(t_data *data);
int			ft_exit(t_data *data);

#endif
