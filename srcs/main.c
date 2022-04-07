/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbensem <thbensem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:08:45 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/06 17:11:19 by thbensem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_in_map(t_data *data, int x, int y)
{
	return (x >= 0 && y >= 0
		&& x < data->vars.mapWidth && y < data->vars.mapHeight);
}

double	fix_angle(double f)
{
	if (f < 0)
		return (f + 2 * M_PI);
	else if (f > 2 * M_PI)
		return (f - 2 * M_PI);
	return (f);
}

void	free_all(t_data *data)
{
	int		i;
	t_list	*tmp;

	free_double_array(data->vars.map);
	i = -1;
	while (++i < 6)
		mlx_destroy_image(data->vars.mlx, data->img[i].img);
	while (data->sprites)
	{
		tmp = data->sprites;
		data->sprites = data->sprites->next;
		free(tmp);
	}
	i = -1;
	while (data->generators[++i])
		free(data->generators[i]);
	free(data->generators);
}

void	free_sprite_imgs(t_data *data, t_sprite_text *texture)
{
	int	i;

	i = -1;
	while (++i < 6)
		mlx_destroy_image(data->vars.mlx, texture->text[i].img);
	mlx_destroy_image(data->vars.mlx, texture->attack_text[0].img);
	mlx_destroy_image(data->vars.mlx, texture->attack_text[1].img);
	i = -1;
	while (++i < 4)
		mlx_destroy_image(data->vars.mlx, texture->dead_text[i].img);
}

int	ft_exit(t_data *data)
{
	int	i;

	free_all(data);
	free_sprite_imgs(data, &data->sprite_text[CHOPPER]);
	free_sprite_imgs(data, &data->sprite_text[LUFFY]);
	free_sprite_imgs(data, &data->sprite_text[TEACH]);
	i = -1;
	while (++i < 4)
		mlx_destroy_image(data->vars.mlx, data->hands[i].img);
	mlx_destroy_image(data->vars.mlx, data->frame.img);
	mlx_destroy_window(data->vars.mlx, data->vars.win);
	free(data->vars.mlx);
	exit(EXIT_SUCCESS);
}

void	draw_square(t_data *data, int x, int y, int size)
{
	int	x1;
	int	y1;

	if (size % 2 != 0)
		size--;
	y1 = y - size / 2;
	if (y1 < 0)
		y1 = 0;
	while (y1 < y + size / 2 && y1 < SCREEN_WIDTH - 1)
	{	
		x1 = x - size / 2;
		if (x1 < 0)
			x1 = 0;
		while (x1 < x + size / 2 && x1 < SCREEN_WIDTH - 1)
		{
			data->frame.imgptr[y1 * (data->frame.sizeLine / 4) + x1]
				= create_rgb(255, 0, 255);
			x1++;
		}
		y1++;
	}
}

int	create_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

void	display_center_cross(t_data *data)
{
	int	y;
	int	x;
	int	cross_color;

	cross_color = create_rgb(150, 150, 150);
	y = (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT * 0.01) - 1;
	while (++y < (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT * 0.01))
		data->frame.imgptr[y * data->frame.sizeLine / 4 + (SCREEN_WIDTH / 2)]
			= cross_color;
	x = (SCREEN_WIDTH / 2) - (SCREEN_HEIGHT * 0.01) - 1;
	while (++x < (SCREEN_WIDTH / 2) + (SCREEN_HEIGHT * 0.01))
		data->frame.imgptr[(SCREEN_HEIGHT / 2) * data->frame.sizeLine / 4 + x]
			= cross_color;
}

void	draw_life(t_data *data, int i, int y, int x)
{
	while (++y < 30)
	{
		x = -1;
		while (++x < 3 * 100)
		{
			if (x % 3 == 0)
				i++;
			if (data->player.life >= i)
				data->frame.imgptr[(10 + y) * data->frame.sizeLine / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(200, 0, 0);
			else
				data->frame.imgptr[(10 + y) * data->frame.sizeLine / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(30, 30, 30);
			if (y < 5 && x >= 60 * 3 - 1 && x <= 60 * 3 + 1)
				data->frame.imgptr[(45 + y) * data->frame.sizeLine / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(255, 255, 255);
			else if (y < 5 && data->player.mana >= i)
				data->frame.imgptr[(45 + y) * data->frame.sizeLine / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(255, 210, 0);
			else if (y < 5)
				data->frame.imgptr[(45 + y) * data->frame.sizeLine / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(175, 130, 0);
		}
		i = 0;
	}
}

int	check_for_sprite(t_data *data, int x, int y)
{
	t_list	*curr;

	curr = data->sprites;
	while (curr)
	{
		if (curr->sprite.life > 0 && distance(
				x, y, curr->sprite.x, curr->sprite.y) < CUBE_SIZE * 0.25)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	draw_minimap_line(t_data *data, float start_y, int y,
		int minimap_size_x)
{
	int	x;
	int	start_x;

	x = 10;
	start_x = data->player.x - (CUBE_SIZE / 10) * (minimap_size_x / 2);
	while (x < minimap_size_x + 10)
	{
		if (is_in_map(data, start_x / CUBE_SIZE, start_y / CUBE_SIZE)
			&& data->vars.map[(int)start_y / CUBE_SIZE]
			[(int)start_x / CUBE_SIZE] == '1')
			data->frame.imgptr[y * data->frame.sizeLine / 4 + x] = 0xFFFFFFF;
		else if (is_in_map(data, start_x / CUBE_SIZE, start_y / CUBE_SIZE)
			&& data->vars.map[(int)start_y / CUBE_SIZE]
			[(int)start_x / CUBE_SIZE] == 'D')
			data->frame.imgptr[y * data->frame.sizeLine / 4 + x]
				= create_rgb(255, 200, 0);
		if (check_for_sprite(data, start_x, start_y))
			data->frame.imgptr[y * data->frame.sizeLine / 4 + x]
				= create_rgb(255, 0, 0);
		start_x += CUBE_SIZE / 10;
		x++;
	}
}

void	draw_mini_map(t_data *data)
{
	int		minimap_size_y;
	int		minimap_size_x;
	float	start_y;
	int		y;

	minimap_size_x = (SCREEN_WIDTH / 4);
	minimap_size_y = (SCREEN_HEIGHT / 4);
	start_y = data->player.y - (CUBE_SIZE / 10) * (minimap_size_y / 2);
	y = 10;
	while (y < minimap_size_y + 10)
	{
		draw_minimap_line(data, start_y, y, minimap_size_x);
		start_y += CUBE_SIZE / 10;
		y++;
	}
	draw_square(data, (minimap_size_x / 2 + 10), (minimap_size_y / 2 + 10), 5);
	draw_square(data, (minimap_size_x / 2 + 10 - data->player.dx * 5),
		(minimap_size_y / 2 + 10 + data->player.dy * 5), 2);
	draw_square(data, (minimap_size_x / 2 + 10 - data->player.dx * 7),
		(minimap_size_y / 2 + 10 + data->player.dy * 7), 2);
	draw_square(data, (minimap_size_x / 2 + 10 - data->player.dx * 9),
		(minimap_size_y / 2 + 10 + data->player.dy * 9), 2);
}

double	distance(double ax, double ay, double bx, double by)
{
	return (sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)));
}

int	get_r(int trgb)
{
	return ((trgb >> 16) & 0xFF);
}

int	get_g(int trgb)
{
	return ((trgb >> 8) & 0xFF);
}

int	get_b(int trgb)
{
	return (trgb & 0xFF);
}

int	shadow_color(int color, int distance)
{
	int	r;
	int	g;
	int	b;

	r = get_r(color) - (distance * 0.008);
	g = get_g(color) - (distance * 0.008);
	b = get_b(color) - (distance * 0.008);
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	return (create_rgb(r, g, b));
}

void	draw_text_vline(t_data *data, t_img *img, t_ray ray, int x_in_text)
{
	int		truestart;
	int		y1;
	int		color;

	truestart
		= -(ray.line_height * (1 - data->camera_height)) + data->horizon_line;
	while (ray.line_start < ray.line_end)
	{
		y1 = ((double)(ray.line_start - truestart)
				/ (double)ray.line_height) * (img->height - 1);
		if (y1 >= img->height || y1 < 0)
			printf("y1 = %d\n", y1);// A VIRER A TERME
		if (x_in_text < 0 || x_in_text >= img->width) // A VIRER A TERME
			printf("x_in_text = %d\n", x_in_text);// A VIRER A TERME
		color = img->imgptr[(int)(y1 * (img->sizeLine / 4) + x_in_text)];
		data->frame.imgptr[ray.line_start * data->frame.sizeLine / 4 + ray.nb]
			= shadow_color(color, ray.length);
		(ray.line_start)++;
	}
}

void	draw_vline(t_ray *ray, int end, t_data *data, int color)
{
	while (ray->line_end < end)
	{
		data->frame.imgptr[ray->line_end * data->frame.sizeLine / 4 + ray->nb]
			= color;
		ray->line_end++;
	}
}

void	draw_floor(t_data *data, t_ray ray, t_img *text, int x)
{
	int		text_x;
	int		text_y;
	float	dy;
	float	ra_fix;

	ra_fix = cos(fix_angle(data->player.dir - ray.Angle));
	while (ray.line_end < SCREEN_HEIGHT)
	{
		dy = ray.line_end - data->horizon_line;
		text_x = (int)(data->player.x - (cos(ray.Angle) * (SCREEN_HEIGHT / 2)
					* (text->width * 2 * data->camera_height) / dy / ra_fix));
		text_y = (int)(data->player.y - (sin(ray.Angle) * (SCREEN_HEIGHT / 2)
					* (text->height * 2 * data->camera_height) / dy / ra_fix));
		data->frame.imgptr[ray.line_end * (data->frame.sizeLine / 4) + x]
			= shadow_color(text->imgptr[(text_y & (text->height - 1))
				* (text->sizeLine / 4) + (text_x & (text->width - 1))],
				(CUBE_SIZE / ((ray.line_end - data->horizon_line) * 0.005))
				* 2 * data->camera_height);
		(ray.line_end)++;
	}
}

#include <sys/time.h>

long	get_timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	draw_hands(t_data *data)
{
	int	x;
	int	y;
	int	color;

	x = -1;
	while (++x < data->hands[data->hands_to_display].width)
	{
		y = -1;
		while (++y < data->hands[data->hands_to_display].height)
		{
			color = data->hands[data->hands_to_display].imgptr[y * data->hands[
				data->hands_to_display].sizeLine / 4 + x];
			if (color != create_trgb(255, 0, 0, 0) && x >= 0
				&& x < SCREEN_WIDTH * 0.6 && y >= 0 && y < SCREEN_HEIGHT)
				data->frame.imgptr[((SCREEN_HEIGHT - 1)
						- data->hands[data->hands_to_display].height + y)
					* (data->frame.sizeLine / 4)
					+ (int)(SCREEN_WIDTH * 0.4 + x)] = color;
		}
	}
}

void	generate_sprites(t_data *data, t_generator **generator,
		int i, int percent)
{
	while (generator[++i])
	{
		if (distance(data->player.x, data->player.y,
				(generator[i]->mapX * CUBE_SIZE) + (CUBE_SIZE / 2),
				(generator[i]->mapY * CUBE_SIZE) + (CUBE_SIZE / 2))
			< CUBE_SIZE * 25
			&& (get_timestamp() - generator[i]->timer > 10000
				|| generator[i]->timer == -1)
			&& generator[i]->nb_gen < generator[i]->wave)
		{
			percent = rand();
			if (percent % 100 < 40)
				push_back(&data->sprites,
					new_luffy(data, generator[i]->mapX, generator[i]->mapY));
			else if (percent % 100 >= 40 && percent % 100 < 90)
				push_back(&data->sprites,
					new_chopper(data, generator[i]->mapX, generator[i]->mapY));
			else if (percent % 100 >= 90)
				push_back(&data->sprites,
					new_teach(data, generator[i]->mapX, generator[i]->mapY));
			generator[i]->nb_gen++;
			generator[i]->timer = get_timestamp();
		}
	}
}

void	manage_generators(t_data *data, t_generator **generator)
{
	int	i;

	i = -1;
	if (data->sprites == NULL && generator[0]
		&& generator[0]->nb_gen >= generator[0]->wave)
	{
		while (generator[++i])
		{
			generator[i]->wave++;
			generator[i]->nb_gen = 0;
		}
		data->player.life += generator[0]->wave * 10;
		if (data->player.life > 100)
			data->player.life = 100;
	}
	generate_sprites(data, generator, -1, 0);
}

void	display_wave_nb(t_data *data)
{
	char	*tmp;
	char	*wave;

	tmp = ft_itoa(data->generators[0]->wave);
	wave = ft_join("WAVE: ", tmp);
	free(tmp);
	mlx_string_put(data->vars.mlx, data->vars.win,
		SCREEN_WIDTH / 2 - 40, 20, 0x00FFFFFF, wave);//CREE UN LEAK JSP PQ
	free(wave);
}

void	make_frame(t_data *data)
{
	draw_rays(data);
	manage_generators(data, data->generators);
	attack_sprite(data, data->sprites);
	draw_sprites(data);
	draw_hands(data);
	display_center_cross(data);
	draw_life(data, 0, -1, -1);
	draw_mini_map(data);
	mlx_put_image_to_window(data->vars.mlx, data->vars.win,
		data->frame.img, 0, 0);
	if (data->generators[0] && data->generators[0]->wave > 0)
		display_wave_nb(data);
}

int	player_can_move(t_data *data, double x, double y)
{
	return (!there_is_sprite(data, NULL, (int)(data->player.x + x * data->fps),
		(int)(data->player.y + y * data->fps))
			&& is_in_map(data, (int)(data->player.x + x * data->fps)
				/ CUBE_SIZE, (int)(data->player.y + y * data->fps) / CUBE_SIZE)
			&& data->vars.map[(int)(data->player.y + y * data->fps) / CUBE_SIZE]
				[(int)(data->player.x + x * data->fps) / CUBE_SIZE] == '0');
}

void	reset_jump_values(t_data *data, int *power)
{
	data->player.jump_calls = 0;
	*power = 9;
	data->player.in_jump = 0;
	data->player.attack_dir_x = 0;
	data->player.attack_dir_y = 0;
}

void	player_being_attacked_movement(t_data *data)
{
	if (player_can_move(data,
			-data->player.attack_dir_x * (CUBE_SIZE * 0.001), 0))
		data->player.x
			-= data->player.attack_dir_x * (CUBE_SIZE * 0.001) * data->fps;
	if (player_can_move(data,
			0, data->player.attack_dir_y * (CUBE_SIZE * 0.001)))
		data->player.y
			+= data->player.attack_dir_y * (CUBE_SIZE * 0.001) * data->fps;
}

void	jump(t_data *data)
{
	static int	power = 9;
	static long	last_frame = -1;

	if (last_frame != -1 && get_timestamp() - last_frame < 20)
		return ;
	if (data->player.jump_calls < 8)
	{
		power--;
		data->camera_height += 0.007 * power;
	}
	else if (data->player.jump_calls > 8 && data->player.jump_calls < 17)
	{
		data->camera_height -= 0.007 * power;
		power++;
	}
	player_being_attacked_movement(data);
	data->player.jump_calls++;
	if (data->player.jump_calls == 17)
		reset_jump_values(data, &power);
	last_frame = get_timestamp();
}

void	haki_des_rois(t_data *data)
{
	t_list	*curr;

	curr = data->sprites;
	while (curr)
	{
		if (distance(data->player.x, data->player.y,
				curr->sprite.x, curr->sprite.y)
			< CUBE_SIZE * (float)((data->player.mana - 45) / 10.0f))
		{
			curr->sprite.dead_position
				= sprite_is_facing_left(data, &curr->sprite);
			curr->sprite.life = 0;
			curr->sprite.count = 0;
			curr->sprite.last_frame = get_timestamp();
		}
		curr = curr->next;
	}
	data->player.mana = 0;
}

void	dead_screen(t_data *data, long *timer, int *calls)
{
	if ((*timer == -1 || get_timestamp() - (*timer) > 50))
	{
		if (*calls < 30)
			data->camera_height -= 0.012;
		(*calls)++;
		if (*calls == 100)
		{
			*calls = 0;
			data->camera_height = 0.5;
			data->horizon_line = SCREEN_HEIGHT / 2;
			data->hands_to_display = 0;
			free_all(data);
			parse_all(data, data->vars.config_file);
		}
		*timer = get_timestamp();
	}
}

void	move_player_h(t_data *data)
{
	if (data->keys.z)
	{
		if (player_can_move(data, -data->player.dx * V_MOV_SPEED, 0))
			data->player.x -= (data->player.dx * V_MOV_SPEED) * data->fps;
		if (player_can_move(data, 0, data->player.dy * V_MOV_SPEED))
			data->player.y += (data->player.dy * V_MOV_SPEED) * data->fps;
	}
	if (data->keys.s)
	{
		if (player_can_move(data, data->player.dx * V_MOV_SPEED, 0))
			data->player.x += data->player.dx * V_MOV_SPEED * data->fps;
		if (player_can_move(data, 0, -data->player.dy * V_MOV_SPEED))
			data->player.y -= data->player.dy * V_MOV_SPEED * data->fps;
	}
}

void	move_player_v(t_data *data)
{
	if (data->keys.q)
	{
		if (player_can_move(data, data->player.dy * H_MOV_SPEED, 0))
			data->player.x += data->player.dy * H_MOV_SPEED * data->fps;
		if (player_can_move(data, 0, data->player.dx * H_MOV_SPEED))
			data->player.y += data->player.dx * H_MOV_SPEED * data->fps;
	}
	if (data->keys.d)
	{
		if (player_can_move(data, -data->player.dy * H_MOV_SPEED, 0))
			data->player.x -= data->player.dy * H_MOV_SPEED * data->fps;
		if (player_can_move(data, 0, -data->player.dx * H_MOV_SPEED))
			data->player.y -= data->player.dx * H_MOV_SPEED * data->fps;
	}
}

void	rotate_camera(t_data *data)
{
	if (data->keys.left)
	{
		data->player.dir = fix_angle(data->player.dir
				- (DEGREE * ROTATION_SPEED * (data->fps / 5)));
		data->player.dx = cos(data->player.dir);
		data->player.dy = -sin(data->player.dir);
	}
	if (data->keys.right)
	{
		data->player.dir = fix_angle(data->player.dir
				+ (DEGREE * ROTATION_SPEED * (data->fps / 5)));
		data->player.dx = cos(data->player.dir);
		data->player.dy = -sin(data->player.dir);
	}
}

void	bonus_controls(t_data *data, long *timer, int *calls)
{
	if (data->keys.esp)
		data->player.in_jump = 1;
	if (data->player.in_jump)
		jump(data);
	if (data->keys.enter && data->player.mana >= 60)
		haki_des_rois(data);
	if (data->keys.left_click
		&& (*timer == -1 || get_timestamp() - (*timer) > 50))
	{
		(*calls)++;
		if (*calls < 4)
			data->hands_to_display++;
		else if (*calls > 5)
			data->hands_to_display--;
		if (*calls == 3)
			data->player.attack = 1;
		else if (*calls == 5)
			data->player.attack = 0;
		if (*calls == 8)
		{
			*calls = 0;
			data->keys.left_click = 0;
		}
		*timer = get_timestamp();
	}
}

int	render_next_frame(void *data1)
{
	t_data		*data;
	time_t		tmp;
	static long	timer = -1;
	static int	calls = 0;

	data = (t_data *)data1;
	tmp = get_timestamp();
	data->fps = tmp - data->last_frame_time;
	data->last_frame_time = tmp;
	if (data->pause == 1)
		return (0);
	if (data->player.life <= 0 && !data->player.in_jump)
		dead_screen(data, &timer, &calls);
	else
	{
		move_player_h(data);
		move_player_v(data);
		rotate_camera(data);
		bonus_controls(data, &timer, &calls);
	}
	make_frame(data);
	mlx_mouse_move(data->vars.mlx, data->vars.win,
		SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
//	mlx_mouse_move(data->vars.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return (0);
}

void	pause_pressed(t_data *data)
{
	data->pause *= -1;
	if (data->pause == 1)
	{
		mlx_string_put(data->vars.mlx, data->vars.win, SCREEN_WIDTH
			/ 2 - 20, SCREEN_HEIGHT / 2, 0x00FFFFFFFF, "PAUSE");
		mlx_mouse_show(data->vars.mlx, data->vars.win);
	}
	else
		mlx_mouse_hide(data->vars.mlx, data->vars.win);
}

#include <time.h>

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (ft_putstr_error("Error\nbad argument.\n"));
	data.pause = -1;
	data.vars.config_file = argv[1];
	srand(time(NULL));

	data.vars.mlx = mlx_init();
	data.vars.win
		= mlx_new_window(data.vars.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "CUB3D");

	if (parse_all(&data, argv[1]))
		return (1);

	load_chopper_imgs(&data, &data.sprite_text[CHOPPER]);
	load_luffy_imgs(&data, &data.sprite_text[LUFFY]);
	load_teach_imgs(&data, &data.sprite_text[TEACH]);

	data.camera_height = 0.5;
	data.horizon_line = SCREEN_HEIGHT / 2;

	init_keys(&data.keys);

	data.frame.img = mlx_new_image(data.vars.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	data.frame.imgptr
		= (int *)mlx_get_data_addr(data.frame.img, &data.frame.bitPerPx,
			&data.frame.sizeLine, &data.frame.endian);

	init_hands(&data);

	data.last_frame_time = get_timestamp();
	data.fps = 50;

	make_frame(&data);
	mlx_hook(data.vars.win, 02, 1L << 0, key_press, &data);
	mlx_hook(data.vars.win, 03, 1L << 1, key_release, &data.keys);
	mlx_loop_hook(data.vars.mlx, render_next_frame, &data);
	mlx_hook(data.vars.win, 17, 0L, ft_exit, &data);

	mlx_mouse_hide(data.vars.mlx, data.vars.win);
	mlx_hook(data.vars.win, 06, 1L << 6, mouse_move, &data);
	mlx_mouse_hook(data.vars.win, mouse_click, &data);
	mlx_loop(data.vars.mlx);
	return (0);
}
