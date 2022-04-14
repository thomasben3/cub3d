/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_skills.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:38:10 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 02:49:50 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
	if (data->player.attack_dir_x != 0 || data->player.attack_dir_y != 0)
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
