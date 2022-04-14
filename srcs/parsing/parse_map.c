/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 03:20:22 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:48:34 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
		data->vars.map_height++;
		while (line[i] && line[i] != '\n')
		{
			i++;
			len++;
		}
		if (len > data->vars.map_width)
			data->vars.map_width = len;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
}

int	malloc_map(t_data *data, int *description_map_start, int fd)
{
	data->vars.map_width = 0;
	data->vars.map_height = 0;
	(*description_map_start)--;
	get_map_height_and_width(data, fd,
		description_map_start, get_next_line(fd));
	close(fd);
	data->vars.ray_max_len = sqrt((data->vars.map_width * data->vars.map_width)
			+ (data->vars.map_height * data->vars.map_height));
	data->vars.map
		= (char **)malloc(sizeof(char *) * (data->vars.map_height + 1));
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
		data->vars.map[i] = (char *)malloc(data->vars.map_width + 1);
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
		while (j < data->vars.map_width)
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
	i = -2;
	fd = open(config_path, O_RDONLY);
	if (fd < 0)
		return (1);
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
