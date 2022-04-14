/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_and_generators.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:32:34 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 02:33:29 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

int	there_is_door(t_data *data, double rx, double ry, int face)
{
	int	i;

	i = -1;
	while (data->doors[++i])
	{
		if (data->doors[i]->x == (int)rx / CUBE_SIZE
			&& data->doors[i]->y == (int)ry / CUBE_SIZE
			&& (((face == NORTH || face == SOUTH)
					&& (int)rx % CUBE_SIZE + 1
					>= data->doors[i]->start * CUBE_SIZE)
				|| ((face == EAST || face == WEST)
					&& (int)ry % CUBE_SIZE + 1
					>= data->doors[i]->start * CUBE_SIZE)))
			return (1);
	}
	return (0);
}

void	manage_doors(t_data *data)
{
	int	i;

	i = -1;
	while (data->doors[++i])
	{
		if (data->doors[i]->count != -1
			&& get_timestamp() - data->doors[i]->last_frame > 50
			&& (data->doors[i]->status == 1
				|| data->doors[i]->x != (int)data->player.x / CUBE_SIZE
				|| data->doors[i]->y != (int)data->player.y / CUBE_SIZE))
		{
			if (data->doors[i]->status == 1)
				data->doors[i]->start += 0.05;
			else
				data->doors[i]->start -= 0.05;
			data->doors[i]->count++;
			if (data->doors[i]->count == 18)
			{
				data->doors[i]->status *= -1;
				data->doors[i]->count = -1;
			}
		}
	}
}
