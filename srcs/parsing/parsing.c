/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 19:39:20 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/13 03:40:14 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_map_extremities(t_vars *vars, char **map)
{
	int	y;
	int	x;

	y = -1;
	while (map[++y])
		if ((map[y][0] != '1' && map[y][0] != ' ')
			|| (map[y][vars->mapWidth - 1] != '1'
				&& map[y][vars->mapWidth - 1] != ' '))
			return (1);
	x = -1;
	while (++x < vars->mapWidth)
		if ((map[0][x] != '1' && map[0][x] != ' ')
			|| (map[vars->mapHeight - 1][x] != '1'
				&& map[vars->mapHeight - 1][x] != ' '))
			return (1);
	return (0);
}

int	is_map_surrounded(t_vars *vars, char **map)
{
	int	y;
	int	x;

	if (check_map_extremities(vars, map))
		return (0);
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if ((map[y][x] == ' ')
				&& ((x + 1 < vars->mapWidth && map[y][x + 1] != '1'
					&& map[y][x + 1] != ' ')
					|| (x - 1 >= 0 && map[y][x - 1] != '1'
						&& map[y][x - 1] != ' ')
					|| (y + 1 < vars->mapHeight && map[y + 1][x] != '1'
						&& map[y + 1][x] != ' ')
					|| (y - 1 >= 0 && map[y - 1][x] != '1'
						&& map[y - 1][x] != ' ')))
				return (0);
		}
	}
	return (1);
}

void	init_player(t_data *data, int x, int y, char dir)
{
	data->player.x = x * CUBE_SIZE + CUBE_SIZE / 2;
	data->player.y = y * CUBE_SIZE + CUBE_SIZE / 2;
	if (dir == 'N')
		data->player.dir = (M_PI / 2);
	else if (dir == 'S')
		data->player.dir = 3 * (M_PI / 2);
	else if (dir == 'E')
		data->player.dir = M_PI;
	else if (dir == 'W')
		data->player.dir = 0;
	data->player.dx = cos(data->player.dir);
	data->player.dy = -sin(data->player.dir);
	data->player.attack = 0;
	data->player.in_jump = 0;
	data->player.jump_calls = 0;
	data->player.attack_dir_x = 0;
	data->player.attack_dir_y = 0;
	data->player.life = 100;
	data->player.mana = 0;
	data->vars.map[y][x] = '0';
}

int	count_component(char **map, char c)
{
	int	x;
	int	y;
	int	count;

	count = 0;
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
			if (map[y][x] == c)
				count++;
	}
	return (count);
}

int	parse_all(t_data *data, char *path, int description_map_start)
{
	int		fd;

	while (++description_map_start < 6)
		data->img[description_map_start].img = NULL;
	description_map_start = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\n");
		return (1);
	}
	if (parse_identifier(data, fd, &description_map_start)
		|| parse_map(data, fd, description_map_start, path))
	{
		free_textures(data);
		close(fd);
		return (1);
	}
	if (fill_map(data))
	{
		free_textures(data);
		free_double_array(data->vars.map);
		return (1);
	}
	return (0);
}
