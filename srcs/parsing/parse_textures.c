/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 03:11:36 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/13 03:31:36 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

int	parse_identifier(t_data *data, int fd, int *description_map_start)
{
	char	*line;

	line = get_next_line(fd);
	while (*line)
	{
		if (check_for_identifier(data, line))
		{
			free(line);
			return (1);
		}
		free(line);
		if (data->img[NORTH].img != NULL && data->img[SOUTH].img != NULL
			&& data->img[EAST].img != NULL && data->img[WEST].img != NULL
			&& data->img[FLOOR].img != NULL && data->img[CEILING].img != NULL)
			break ;
		(*description_map_start)++;
		line = get_next_line(fd);
	}
	if (data->img[NORTH].img == NULL || data->img[SOUTH].img == NULL
		|| data->img[EAST].img == NULL || data->img[WEST].img == NULL
		|| data->img[FLOOR].img == NULL || data->img[CEILING].img == NULL)
		return (ft_putstr_error("Error\ntexture identifier missing\n"));
	return (0);
}
