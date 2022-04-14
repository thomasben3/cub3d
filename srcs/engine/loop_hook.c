/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:26:35 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:50:05 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	display_center_cross(t_data *data)
{
	int	y;
	int	x;
	int	cross_color;

	cross_color = create_rgb(150, 150, 150);
	y = (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT * 0.01) - 1;
	while (++y < (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT * 0.01))
		data->frame.imgptr[y * data->frame.size_line / 4 + (SCREEN_WIDTH / 2)]
			= cross_color;
	x = (SCREEN_WIDTH / 2) - (SCREEN_HEIGHT * 0.01) - 1;
	while (++x < (SCREEN_WIDTH / 2) + (SCREEN_HEIGHT * 0.01))
		data->frame.imgptr[(SCREEN_HEIGHT / 2) * data->frame.size_line / 4 + x]
			= cross_color;
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
				data->hands_to_display].size_line / 4 + x];
			if (color != create_trgb(255, 0, 0, 0) && x >= 0
				&& x < SCREEN_WIDTH * 0.6 && y >= 0 && y < SCREEN_HEIGHT)
				data->frame.imgptr[((SCREEN_HEIGHT - 1)
						- data->hands[data->hands_to_display].height + y)
					* (data->frame.size_line / 4)
					+ (int)(SCREEN_WIDTH * 0.4 + x)] = color;
		}
	}
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
			parse_all(data, data->vars.config_file, -1);
		}
		*timer = get_timestamp();
	}
}

void	make_frame(t_data *data)
{
	draw_rays(data);
	manage_doors(data);
	manage_generators(data, data->generators);
	draw_sprites(data);
	draw_hands(data);
	display_center_cross(data);
	draw_life(data, 0, -1, -1);
	draw_mini_map(data);
	mlx_put_image_to_window(data->vars.mlx, data->vars.win,
		data->frame.img, 0, 0);
	if (data->generators && data->generators[0]
		&& data->generators[0]->wave > 0)
		display_wave_nb(data);
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
//	mlx_mouse_move(data->vars.mlx, data->vars.win,
//		SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mlx_mouse_move(data->vars.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return (0);
}
