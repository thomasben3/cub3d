/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 19:39:20 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/10 03:38:21 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	get_identifier_face(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (str[*i] == '\0')
		return (-1);
	(*i)++;
	if (str[(*i) - 1] == 'F'
		&& (str[(*i)] == ' ' || str[(*i)] == '\t' || str[(*i)] == '\0'))
		return (FLOOR);
	else if (str[(*i) - 1] == 'C'
		&& (str[(*i)] == ' ' || str[(*i)] == '\t' || str[(*i)] == '\0'))
		return (CEILING);
	(*i)++;
	if (str[(*i) - 2] == 'N' && str[(*i) - 1] == 'O'
		&& (str[(*i)] == ' ' || str[(*i)] == '\t' || str[(*i)] == '\0'))
		return (NORTH);
	else if (str[(*i) - 2] == 'S' && str[(*i) - 1] == 'O'
		&& (str[(*i)] == ' ' || str[(*i)] == '\t' || str[(*i)] == '\0'))
		return (SOUTH);
	else if (str[(*i) - 2] == 'E' && str[(*i) - 1] == 'A'
		&& (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\0'))
		return (EAST);
	else if (str[(*i) - 2] == 'W' && str[(*i) - 1] == 'E'
		&& (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\0'))
		return (WEST);
	return (-2);
}

char	*get_identifier_path(char *str, int i)
{
	int		j;
	int		x;
	char	*tmp;

	j = 0;
	x = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
	{
		j++;
		i++;
	}
	if (j == 0)
	{
		ft_putstr_error("Error\ntexture path missing.\n");
		return (NULL);
	}
	tmp = (char *)malloc(j + 1);
	if (!tmp)
		return (NULL);
	while (j > 0)
		tmp[x++] = str[i - j--];
	tmp[x] = '\0';
	return (tmp);
}

int	check_for_identifier(t_data *data, char *str)
{
	int		i;
	int		face;
	char	*tmp;

	i = 0;
	face = get_identifier_face(str, &i);
	if (face == -1)
		return (0);
	else if (face == -2)
		return (ft_putstr_error("Error\ntexture identifier not valid.\n"));
	else if (data->img[face].img != NULL)
		return (ft_putstr_error(
				"Error\nmultiple delcaration of the same texture\n"));
	tmp = get_identifier_path(str, i);
	if (!tmp)
		return (1);
	data->img[face].img = mlx_xpm_file_to_image(data->vars.mlx, tmp,
			&data->img[face].width, &data->img[face].height);
	free(tmp);
	if (!data->img[face].img)
		return (ft_putstr_error("Error\nTexture not found\n"));
	data->img[face].imgptr = (int *)mlx_get_data_addr(
			data->img[face].img, &data->img[face].bitPerPx,
			&data->img[face].sizeLine, &data->img[face].endian);
	return (0);
}

int	is_empty_line(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
	}
	return (1);
}

void	get_map_height_and_width(
		t_data *data, int fd, int *description_map_start, char *line)
{
	int		len;
	int		i;

	while (*line && is_empty_line(line))
	{
		free(line);
		line = get_next_line(fd);
		(*description_map_start)++;
	}
	while (*line)
	{
		len = 0;
		i = 0;
		data->vars.mapHeight++;
		while (line[i] && line[i] != '\n')
		{
			i++;
			len++;
		}
		if (len > data->vars.mapWidth)
			data->vars.mapWidth = len;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
}

int	malloc_map(t_data *data, int *description_map_start, int fd)
{
	data->vars.mapWidth = 0;
	data->vars.mapHeight = 0;
	(*description_map_start)--;
	get_map_height_and_width(data, fd,
		description_map_start, get_next_line(fd));
	data->vars.map
		= (char **)malloc(sizeof(char *) * (data->vars.mapHeight + 1));
	if (!data->vars.map)
	{
		perror("Error\n");
		return (1);
	}
	return (0);
}

int	fill_map_array(t_data *data, char **line, int i, int fd)
{
	int		j;

	while (**line)
	{
		j = 0;
		data->vars.map[i] = (char *)malloc(data->vars.mapWidth + 1);
		if (!data->vars.map[i])
			return (1);
		while ((*line)[j] && (*line)[j] != '\n')
		{
			if ((*line)[j] == '\t')
				data->vars.map[i][j] = ' ';
			else
				data->vars.map[i][j] = (*line)[j];
			j++;
		}
		while (j < data->vars.mapWidth)
			data->vars.map[i][j++] = ' ';
		data->vars.map[i][j] = '\0';
		free(*line);
		*line = get_next_line(fd);
		i++;
	}
	data->vars.map[i] = NULL;
	return (0);
}

int	parse_map(
	t_data *data, int fd, int description_map_start, char *config_path)
{
	char	*line;
	int		i;

	if (malloc_map(data, &description_map_start, fd))
		return (1);
	close(fd);
	i = -2;
	fd = open(config_path, O_RDONLY);
	if (fd < 0)
	{
		free(data->vars.map);
		perror("Error\n");
		return (1);
	}
	line = get_next_line(fd);
	while (*line && i < description_map_start)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	if (fill_map_array(data, &line, 0, fd))
	{
		free_double_array(data->vars.map);
		return (1);
	}
	free(line);
	close(fd);
	return (0);
}

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

	data->sprites = NULL;
	count = 0;
	data->player.x = -1;
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
	if (set_player_and_sprites(data, data->vars.map) || set_doors(data, data->vars.map, -1, -1))
	{
		list_clear(&data->sprites);
		free(data->generators);
		free(data->doors);
		return (1);
	}
	return (0);
}

int	parse_all(t_data *data, char *path)
{
	int		fd;
	int		description_map_start;
	char	*line;

	description_map_start = 0;
	while (++description_map_start < 6)
		data->img[description_map_start].img = NULL;
	description_map_start = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\n");
		return (1);
	}
	line = get_next_line(fd);
	while (*line)
	{
		if (check_for_identifier(data, line))
		{
			free_textures(data);
			free(line);
			close(fd);
			return (1);
		}
		free(line);
		if (data->img[NORTH].img != NULL && data->img[SOUTH].img != NULL
			&& data->img[EAST].img != NULL && data->img[WEST].img != NULL
			&& data->img[FLOOR].img != NULL && data->img[CEILING].img != NULL)
			break ;
		description_map_start++;
		line = get_next_line(fd);
	}
	if (data->img[NORTH].img == NULL || data->img[SOUTH].img == NULL
		|| data->img[EAST].img == NULL || data->img[WEST].img == NULL
		|| data->img[FLOOR].img == NULL || data->img[CEILING].img == NULL)
	{
		free_textures(data);
		return (ft_putstr_error("Error\ntexture identifier missing\n"));
	}
	if (parse_map(data, fd, description_map_start, path))
	{
		free_textures(data);
		return (1);
	}
	if (fill_map(data))
	{
		free_textures(data);
		free_double_array(data->vars.map);
		return (1);
	}
	load_img(data, &data->door_text, "assets/door.xpm");
	load_chopper_imgs(data, &data->sprite_text[CHOPPER]);
	load_luffy_imgs(data, &data->sprite_text[LUFFY]);
	load_teach_imgs(data, &data->sprite_text[TEACH]);
	return (0);
}
