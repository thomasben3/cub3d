/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 03:28:45 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/13 03:41:21 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	new_generator(t_data *data, int *count, int x, int y)
{
	data->generators[*count]
		= (t_generator *)malloc(sizeof(t_generator));
	if (!data->generators[*count])
		return ;
	data->generators[*count]->mapX = x;
	data->generators[*count]->mapY = y;
	data->generators[*count]->timer = -1;
	data->generators[*count]->nb_gen = 0;
	data->generators[(*count)++]->wave = 0;
	data->vars.map[y][x] = '0';
}

int	treat_char(t_data *data, int x, int y, int *count)
{
	if (data->vars.map[y][x] == 'N' || data->vars.map[y][x] == 'S'
		|| data->vars.map[y][x] == 'E' || data->vars.map[y][x] == 'W')
	{
		if (data->player.x != -1)
			return (ft_putstr_error(
					"Error\nmore than one player starting position.\n"));
		init_player(data, x, y, data->vars.map[y][x]);
	}
	else if (data->vars.map[y][x] == 'L' || data->vars.map[y][x] == 'T'
		|| data->vars.map[y][x] == 'C' || data->vars.map[y][x] == 'G')
	{
		if (data->vars.map[y][x] == 'L')
			push_back(&data->sprites, new_luffy(data, x, y));
		else if (data->vars.map[y][x] == 'T')
			push_back(&data->sprites, new_teach(data, x, y));
		else if (data->vars.map[y][x] == 'C')
			push_back(&data->sprites, new_chopper(data, x, y));
		else if (data->vars.map[y][x] == 'G')
			new_generator(data, count, x, y);
		data->vars.map[y][x] = '0';
	}
	else if (data->vars.map[y][x] != '1' && data->vars.map[y][x] != '0'
		&& data->vars.map[y][x] != ' ' && data->vars.map[y][x] != 'D')
		return (ft_putstr_error("Error\ninvalid component on the map\n"));
	return (0);
}

int	set_player_and_sprites(t_data *data, char **map)
{
	int	y;
	int	x;
	int	count;

	count = 0;
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (treat_char(data, x, y, &count))
			{
				y = -1;
				while (++y < count)
					free(data->generators[y]);
				return (1);
			}
		}
	}
	data->generators[count] = NULL;
	if (data->player.x == -1)
		return (ft_putstr_error(
				"Error\nyou must have at least one player starting position.\n"));
	return (0);
}

int	set_doors(t_data *data, char **map, int x, int y)
{
	int	count;

	count = 0;
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == 'D')
			{
				data->doors[count] = (t_door *)malloc(sizeof(t_door));
				if (!data->doors[count])
					return (1);
				data->doors[count]->status = 1;
				data->doors[count]->x = x;
				data->doors[count]->y = y;
				data->doors[count]->start = 0.0;
				data->doors[count++]->count = -1;
			}
		}
	}
	data->doors[count] = NULL;
	return (0);
}

int	fill_map(t_data *data)
{
	if (!is_map_surrounded(&data->vars, data->vars.map))
		return (ft_putstr_error("Error\nmap must be surrounded by walls\n"));
	data->generators = (t_generator **)malloc(sizeof(t_generator *)
			* (count_component(data->vars.map, 'G') + 1));
	if (!data->generators)
		return (1);
	data->doors = (t_door **)malloc(sizeof(t_door *)
			* (count_component(data->vars.map, 'D') + 1));
	if (!data->doors)
		return (1);
	data->sprites = NULL;
	data->player.x = -1;
	if (set_player_and_sprites(data, data->vars.map)
		|| set_doors(data, data->vars.map, -1, -1))
	{
		list_clear(&data->sprites);
		free(data->generators);
		free(data->doors);
		return (1);
	}
	return (0);
}
