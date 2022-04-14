/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:19:40 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 02:47:40 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

int	player_can_move(t_data *data, double x, double y)
{
	return (!there_is_sprite(data, NULL, (int)(data->player.x + x * data->fps),
		(int)(data->player.y + y * data->fps))
			&& is_in_map(data, (int)(data->player.x + x * data->fps)
				/ CUBE_SIZE, (int)(data->player.y + y * data->fps) / CUBE_SIZE)
			&& (data->vars.map[(int)(data->player.y
				+ y * data->fps) / CUBE_SIZE]
				[(int)(data->player.x + x * data->fps) / CUBE_SIZE] == '0'
				|| (data->vars.map[(int)(data->player.y
				+ y * data->fps) / CUBE_SIZE]
				[(int)(data->player.x + x * data->fps) / CUBE_SIZE] == 'D'
					&& ((x && !there_is_door(data, data->player.x
					+ x * data->fps, data->player.y + y * data->fps, EAST))
						|| (y && !there_is_door(data, data->player.x
						+ x * data->fps,
						data->player.y + y * data->fps, NORTH))))));
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
