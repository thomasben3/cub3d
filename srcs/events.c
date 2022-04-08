/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 17:10:13 by thbensem          #+#    #+#             */
/*   Updated: 2022/04/08 18:25:33 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	key_press(int keycode, void *data1)
{
	t_data	*data;

	data = (t_data *)data1;
	if (keycode == ESC)
		ft_exit(data);
	else if (keycode == Z)
		data->keys.z = 1;
	else if (keycode == Q)
		data->keys.q = 1;
	else if (keycode == S)
		data->keys.s = 1;
	else if (keycode == D)
		data->keys.d = 1;
	else if (keycode == LEFT)
		data->keys.left = 1;
	else if (keycode == RIGHT)
		data->keys.right = 1;
	else if (keycode == ESP)
		data->keys.esp = 1;
	else if (keycode == ENTER)
		data->keys.enter = 1;
	else if (keycode == P)
		pause_pressed(data);
	return (0);
}

int	key_release(int keycode, void *keys1)
{
	t_keys	*keys;

	keys = (t_keys *)keys1;
	if (keycode == Z)
		keys->z = 0;
	else if (keycode == Q)
		keys->q = 0;
	else if (keycode == S)
		keys->s = 0;
	else if (keycode == D)
		keys->d = 0;
	else if (keycode == LEFT)
		keys->left = 0;
	else if (keycode == RIGHT)
		keys->right = 0;
	else if (keycode == ESP)
		keys->esp = 0;
	else if (keycode == ENTER)
		keys->enter = 0;
	//printf("%d\n",keycode);
	return (0);
}

void	mouse_move_y(t_data *data, int y)
{
	if (y < SCREEN_HEIGHT / 2)
	{
		data->horizon_line += (SCREEN_HEIGHT / 2 - y) / 2;
		if (data->horizon_line >= SCREEN_HEIGHT - 1)
			data->horizon_line = SCREEN_HEIGHT - 2;
	}
	else if (y > SCREEN_HEIGHT / 2)
	{
		data->horizon_line -= (y - SCREEN_HEIGHT / 2) / 2;
		if (data->horizon_line < 0)
			data->horizon_line = 0;
	}
}

int	mouse_move(int x, int y, void *data1)
{
	t_data	*data;

	data = (t_data *)data1;
	if (data->pause == 1)
		return (0);
	if (x != SCREEN_WIDTH / 2)
	{
		if (x < SCREEN_WIDTH / 2)
			data->player.dir = fix_angle(data->player.dir - (DEGREE * 0.1
						+ DEGREE * (((SCREEN_WIDTH / 2) - x) / 20)));
		else if (x > SCREEN_WIDTH / 2)
			data->player.dir = fix_angle(data->player.dir + (DEGREE * 0.1
						+ DEGREE * ((x - (SCREEN_WIDTH / 2)) / 20)));
		data->player.dx = cos(data->player.dir);
		data->player.dy = -sin(data->player.dir);
	}
	mouse_move_y(data, y);
	return (0);
}

int	mouse_click(int button, int x, int y, void *data1)
{
	t_data	*data;
	int i = 0;

	(void)x;
	(void)y;
	data = (t_data *)data1;
	if (button == 1 && data->player.life > 0)
		data->keys.left_click = 1;
	else if (button == RCLICK && data->vars.map[
			(int)(data->player.y + data->player.dy * CUBE_SIZE) / CUBE_SIZE]
				[(int)(data->player.x - data->player.dx * CUBE_SIZE)
				/ CUBE_SIZE]
			== 'D')
	{
		while (data->doors[i] && (data->doors[i]->x != (int)(data->player.x - data->player.dx * CUBE_SIZE) / CUBE_SIZE || data->doors[i]->y != (int)(data->player.y + data->player.dy * CUBE_SIZE) / CUBE_SIZE))
			i++;
		if (data->doors[i] && data->doors[i]->count == -1)
		{
			data->doors[i]->count = 0;
			data->doors[i]->last_frame = get_timestamp();
		}
	}
	return (0);
}
