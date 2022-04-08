/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 19:39:20 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/08 16:12:40 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_for_identifier(t_data *data, char *str)
{
	int	i;
	int	j;
	int	x;
	int	face;
	char	*tmp;

	face = -1;
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		if (str[i] == '\0')
			return (0);
		if (str[i] == 'N' && str[i + 1] == 'O' && (str[i + 2] == ' ' || str[i + 2] == '\t' || str[i + 2] == '\0'))
			face = NORTH;
		else if (str[i] == 'S' && str[i + 1] == 'O' && (str[i + 2] == ' ' || str[i + 2] == '\t' || str[i + 2] == '\0'))
			face = SOUTH;
		else if (str[i] == 'E' && str[i + 1] == 'A' && (str[i + 2] == ' ' || str[i + 2] == '\t' || str[i + 2] == '\0'))
			face = EAST;
		else if (str[i] == 'W' && str[i + 1] == 'E' && (str[i + 2] == ' ' || str[i + 2] == '\t' || str[i + 2] == '\0'))
			face = WEST;
		else if (str[i] == 'F' && (str[i + 1] == ' ' || str[i + 1] == '\t' || str[i + 1] == '\0'))
			face = FLOOR;
		else if (str[i] == 'C' && (str[i + 1] == ' ' || str[i + 1] == '\t' || str[i + 1] == '\0'))
			face = CEILING;
		else
			return (ft_putstr_error("Error\ntexture identifier not valid.\n"));
		i+= 2;
		break ;
	}
	j = 0;
	x = 0;
	while (str[i] && str[i] != '\n')
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
		{
			j++;
			i++;
		}
		if (j == 0)
			return (ft_putstr_error("Error\ntexture path missing.\n"));
		if (data->img[face].img != NULL)
			return (ft_putstr_error("Error\nmultiple delcaration of the same texture\n"));
		tmp = (char *)malloc(j + 1);
		if (!tmp)
		{
			perror("Error\n");
			return (1);
		}
		while (j > 0)
			tmp[x++] = str[i - j--];
		tmp[x] = '\0';
		data->img[face].img = mlx_xpm_file_to_image(data->vars.mlx, tmp, &data->img[face].width, &data->img[face].height);
		if (!data->img[face].img)
			return (ft_putstr_error("Error\nTexture not found\n"));
		data->img[face].imgptr = (int *)mlx_get_data_addr(data->img[face].img, &data->img[face].bitPerPx, &data->img[face].sizeLine, &data->img[face].endian);
		free(tmp);
	}
	return (0);
}

int	is_empty_line(char *line)
{
	int i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
	}
	return (1);
}

void	get_mapHeight_and_mapWidth(t_data *data, int fd, int *descriptionMapStart)
{
	char	*line;
	int		len;
	int		i;

	data->vars.mapWidth = 0;
	data->vars.mapHeight = 0;
	line = get_next_line(fd);
	(*descriptionMapStart)--;
	while (*line && is_empty_line(line))
	{
		free(line);
		line = get_next_line(fd);
		(*descriptionMapStart)++;
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

int	parse_map(t_data *data, int fd, int descriptionMapStart, char *config_path)
{
	char	*line;
	int		i;

	get_mapHeight_and_mapWidth(data, fd, &descriptionMapStart);
	data->vars.map = (char **)malloc(sizeof(char *) * (data->vars.mapHeight + 1));
	if (!data->vars.map)
	{
		perror("Error\n");
		return (1);
	}
	close(fd);
	i = -2;
	fd = open(config_path, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\n");
		return (1);
	}
	line = get_next_line(fd);
	while (*line && i < descriptionMapStart)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	i = 0;
	while (*line)
	{
		int j = 0;
		data->vars.map[i] = (char *)malloc(data->vars.mapWidth + 1);
		if (!data->vars.map[i])
		{
			perror("Error\n");
			return (1);
		}
		while (line[j] && line[j] != '\n')
		{
			if (line[j] == '\t')
				data->vars.map[i][j] = ' ';
			else
				data->vars.map[i][j] = line[j];
			j++;
		}
		while (j < data->vars.mapWidth)
			data->vars.map[i][j++] = ' ';
		data->vars.map[i][j] = '\0';
		free(line);
		line = get_next_line(fd);
		i++;
	}
	data->vars.map[i] = NULL;
	free(line);
	close(fd);
	return (0);
}

int	is_map_surrounded(t_vars *vars, char **map)
{
	int	y;
	int	x;

	y = -1;
	while (map[++y])
		if ((map[y][0] != '1' && map[y][0] != ' ') || (map[y][vars->mapWidth - 1] != '1' && map[y][vars->mapWidth - 1] != ' '))
			return (0);
	x = -1;
	while (++x < vars->mapWidth)
		if ((map[0][x] != '1' && map[0][x] != ' ') || (map[vars->mapHeight - 1][x] != '1' && map[vars->mapHeight - 1][x] != ' '))
			return (0);
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if ((map[y][x] == ' ')
				&& ((x + 1 < vars->mapWidth && map[y][x + 1] != '1' && map[y][x + 1] != ' ')
					|| (x - 1 >= 0 && map[y][x - 1] != '1' && map[y][x - 1] != ' ')
					|| (y + 1 < vars->mapHeight && map[y + 1][x] != '1' && map[y + 1][x] != ' ')
					|| (y - 1 >= 0 && map[y - 1][x] != '1' && map[y - 1][x] != ' ')))
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
}

void	load_teach_imgs(t_data *data, t_sprite_text *teach)
{
	teach->text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach0.xpm", &teach->text[0].width, &teach->text[0].height);
	teach->text[0].imgptr = (int *)mlx_get_data_addr(teach->text[0].img, &teach->text[0].bitPerPx, &teach->text[0].sizeLine, &teach->text[0].endian);
	teach->text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach1.xpm", &teach->text[1].width, &teach->text[1].height);
	teach->text[1].imgptr = (int *)mlx_get_data_addr(teach->text[1].img, &teach->text[1].bitPerPx, &teach->text[1].sizeLine, &teach->text[1].endian);
	teach->text[2].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach2.xpm", &teach->text[2].width, &teach->text[2].height);
	teach->text[2].imgptr = (int *)mlx_get_data_addr(teach->text[2].img, &teach->text[2].bitPerPx, &teach->text[2].sizeLine, &teach->text[2].endian);
	teach->text[3].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach3.xpm", &teach->text[3].width, &teach->text[3].height);
	teach->text[3].imgptr = (int *)mlx_get_data_addr(teach->text[3].img, &teach->text[3].bitPerPx, &teach->text[3].sizeLine, &teach->text[3].endian);
	teach->text[4].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach4.xpm", &teach->text[4].width, &teach->text[4].height);
	teach->text[4].imgptr = (int *)mlx_get_data_addr(teach->text[4].img, &teach->text[4].bitPerPx, &teach->text[4].sizeLine, &teach->text[4].endian);
	teach->text[5].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach5.xpm", &teach->text[5].width, &teach->text[5].height);
	teach->text[5].imgptr = (int *)mlx_get_data_addr(teach->text[5].img, &teach->text[5].bitPerPx, &teach->text[5].sizeLine, &teach->text[5].endian);

	teach->attack_text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach_attack0.xpm", &teach->attack_text[0].width, &teach->attack_text[0].height);
	teach->attack_text[0].imgptr = (int *)mlx_get_data_addr(teach->attack_text[0].img, &teach->attack_text[0].bitPerPx, &teach->attack_text[0].sizeLine, &teach->attack_text[0].endian);
	teach->attack_text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach_attack1.xpm", &teach->attack_text[1].width, &teach->attack_text[1].height);
	teach->attack_text[1].imgptr = (int *)mlx_get_data_addr(teach->attack_text[1].img, &teach->attack_text[1].bitPerPx, &teach->attack_text[1].sizeLine, &teach->attack_text[1].endian);

	teach->dead_text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach_dead0.xpm", &teach->dead_text[0].width, &teach->dead_text[0].height);
	teach->dead_text[0].imgptr = (int *)mlx_get_data_addr(teach->dead_text[0].img, &teach->dead_text[0].bitPerPx, &teach->dead_text[0].sizeLine, &teach->dead_text[0].endian);
	teach->dead_text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach_dead1.xpm", &teach->dead_text[1].width, &teach->dead_text[1].height);
	teach->dead_text[1].imgptr = (int *)mlx_get_data_addr(teach->dead_text[1].img, &teach->dead_text[1].bitPerPx, &teach->dead_text[1].sizeLine, &teach->dead_text[1].endian);
	teach->dead_text[2].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach_dead2.xpm", &teach->dead_text[2].width, &teach->dead_text[2].height);
	teach->dead_text[2].imgptr = (int *)mlx_get_data_addr(teach->dead_text[2].img, &teach->dead_text[2].bitPerPx, &teach->dead_text[2].sizeLine, &teach->dead_text[2].endian);
	teach->dead_text[3].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/teach/teach_dead3.xpm", &teach->dead_text[3].width, &teach->dead_text[3].height);
	teach->dead_text[3].imgptr = (int *)mlx_get_data_addr(teach->dead_text[3].img, &teach->dead_text[3].bitPerPx, &teach->dead_text[3].sizeLine, &teach->dead_text[3].endian);
}

void	load_chopper_imgs(t_data *data, t_sprite_text *chopper)
{
	chopper->text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper0.xpm", &chopper->text[0].width, &chopper->text[0].height);
	chopper->text[0].imgptr = (int *)mlx_get_data_addr(chopper->text[0].img, &chopper->text[0].bitPerPx, &chopper->text[0].sizeLine, &chopper->text[0].endian);
	chopper->text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper1.xpm", &chopper->text[1].width, &chopper->text[1].height);
	chopper->text[1].imgptr = (int *)mlx_get_data_addr(chopper->text[1].img, &chopper->text[1].bitPerPx, &chopper->text[1].sizeLine, &chopper->text[1].endian);
	chopper->text[2].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper2.xpm", &chopper->text[2].width, &chopper->text[2].height);
	chopper->text[2].imgptr = (int *)mlx_get_data_addr(chopper->text[2].img, &chopper->text[2].bitPerPx, &chopper->text[2].sizeLine, &chopper->text[2].endian);
	chopper->text[3].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper3.xpm", &chopper->text[3].width, &chopper->text[3].height);
	chopper->text[3].imgptr = (int *)mlx_get_data_addr(chopper->text[3].img, &chopper->text[3].bitPerPx, &chopper->text[3].sizeLine, &chopper->text[3].endian);
	chopper->text[4].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper4.xpm", &chopper->text[4].width, &chopper->text[4].height);
	chopper->text[4].imgptr = (int *)mlx_get_data_addr(chopper->text[4].img, &chopper->text[4].bitPerPx, &chopper->text[4].sizeLine, &chopper->text[4].endian);
	chopper->text[5].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper5.xpm", &chopper->text[5].width, &chopper->text[5].height);
	chopper->text[5].imgptr = (int *)mlx_get_data_addr(chopper->text[5].img, &chopper->text[5].bitPerPx, &chopper->text[5].sizeLine, &chopper->text[5].endian);

	chopper->attack_text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper_attack0.xpm", &chopper->attack_text[0].width, &chopper->attack_text[0].height);
	chopper->attack_text[0].imgptr = (int *)mlx_get_data_addr(chopper->attack_text[0].img, &chopper->attack_text[0].bitPerPx, &chopper->attack_text[0].sizeLine, &chopper->attack_text[0].endian);
	chopper->attack_text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper_attack1.xpm", &chopper->attack_text[1].width, &chopper->attack_text[1].height);
	chopper->attack_text[1].imgptr = (int *)mlx_get_data_addr(chopper->attack_text[1].img, &chopper->attack_text[1].bitPerPx, &chopper->attack_text[1].sizeLine, &chopper->attack_text[1].endian);

	chopper->dead_text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper_dead0.xpm", &chopper->dead_text[0].width, &chopper->dead_text[0].height);
	chopper->dead_text[0].imgptr = (int *)mlx_get_data_addr(chopper->dead_text[0].img, &chopper->dead_text[0].bitPerPx, &chopper->dead_text[0].sizeLine, &chopper->dead_text[0].endian);
	chopper->dead_text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper_dead1.xpm", &chopper->dead_text[1].width, &chopper->dead_text[1].height);
	chopper->dead_text[1].imgptr = (int *)mlx_get_data_addr(chopper->dead_text[1].img, &chopper->dead_text[1].bitPerPx, &chopper->dead_text[1].sizeLine, &chopper->dead_text[1].endian);
	chopper->dead_text[2].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper_dead2.xpm", &chopper->dead_text[2].width, &chopper->dead_text[2].height);
	chopper->dead_text[2].imgptr = (int *)mlx_get_data_addr(chopper->dead_text[2].img, &chopper->dead_text[2].bitPerPx, &chopper->dead_text[2].sizeLine, &chopper->dead_text[2].endian);
	chopper->dead_text[3].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/chopper/chopper_dead3.xpm", &chopper->dead_text[3].width, &chopper->dead_text[3].height);
	chopper->dead_text[3].imgptr = (int *)mlx_get_data_addr(chopper->dead_text[3].img, &chopper->dead_text[3].bitPerPx, &chopper->dead_text[3].sizeLine, &chopper->dead_text[3].endian);
}

void	load_luffy_imgs(t_data *data, t_sprite_text *luffy)
{
	luffy->text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy5.xpm", &luffy->text[0].width, &luffy->text[0].height);
	luffy->text[0].imgptr = (int *)mlx_get_data_addr(luffy->text[0].img, &luffy->text[0].bitPerPx, &luffy->text[0].sizeLine, &luffy->text[0].endian);
	luffy->text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy4.xpm", &luffy->text[1].width, &luffy->text[1].height);
	luffy->text[1].imgptr = (int *)mlx_get_data_addr(luffy->text[1].img, &luffy->text[1].bitPerPx, &luffy->text[1].sizeLine, &luffy->text[1].endian);
	luffy->text[2].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy3.xpm", &luffy->text[2].width, &luffy->text[2].height);
	luffy->text[2].imgptr = (int *)mlx_get_data_addr(luffy->text[2].img, &luffy->text[2].bitPerPx, &luffy->text[2].sizeLine, &luffy->text[2].endian);
	luffy->text[3].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy2.xpm", &luffy->text[3].width, &luffy->text[3].height);
	luffy->text[3].imgptr = (int *)mlx_get_data_addr(luffy->text[3].img, &luffy->text[3].bitPerPx, &luffy->text[3].sizeLine, &luffy->text[3].endian);
	luffy->text[4].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy1.xpm", &luffy->text[4].width, &luffy->text[4].height);
	luffy->text[4].imgptr = (int *)mlx_get_data_addr(luffy->text[4].img, &luffy->text[4].bitPerPx, &luffy->text[4].sizeLine, &luffy->text[4].endian);
	luffy->text[5].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy0.xpm", &luffy->text[5].width, &luffy->text[5].height);
	luffy->text[5].imgptr = (int *)mlx_get_data_addr(luffy->text[5].img, &luffy->text[5].bitPerPx, &luffy->text[5].sizeLine, &luffy->text[5].endian);

	luffy->attack_text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy_attack0.xpm", &luffy->attack_text[0].width, &luffy->attack_text[0].height);
	luffy->attack_text[0].imgptr = (int *)mlx_get_data_addr(luffy->attack_text[0].img, &luffy->attack_text[0].bitPerPx, &luffy->attack_text[0].sizeLine, &luffy->attack_text[0].endian);
	luffy->attack_text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy_attack1.xpm", &luffy->attack_text[1].width, &luffy->attack_text[1].height);
	luffy->attack_text[1].imgptr = (int *)mlx_get_data_addr(luffy->attack_text[1].img, &luffy->attack_text[1].bitPerPx, &luffy->attack_text[1].sizeLine, &luffy->attack_text[1].endian);

	luffy->dead_text[0].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy_dead0.xpm", &luffy->dead_text[0].width, &luffy->dead_text[0].height);
	luffy->dead_text[0].imgptr = (int *)mlx_get_data_addr(luffy->dead_text[0].img, &luffy->dead_text[0].bitPerPx, &luffy->dead_text[0].sizeLine, &luffy->dead_text[0].endian);
	luffy->dead_text[1].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy_dead1.xpm", &luffy->dead_text[1].width, &luffy->dead_text[1].height);
	luffy->dead_text[1].imgptr = (int *)mlx_get_data_addr(luffy->dead_text[1].img, &luffy->dead_text[1].bitPerPx, &luffy->dead_text[1].sizeLine, &luffy->dead_text[1].endian);
	luffy->dead_text[2].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy_dead2.xpm", &luffy->dead_text[2].width, &luffy->dead_text[2].height);
	luffy->dead_text[2].imgptr = (int *)mlx_get_data_addr(luffy->dead_text[2].img, &luffy->dead_text[2].bitPerPx, &luffy->dead_text[2].sizeLine, &luffy->dead_text[2].endian);
	luffy->dead_text[3].img = mlx_xpm_file_to_image(data->vars.mlx, "assets/luffy/luffy_dead3.xpm", &luffy->dead_text[3].width, &luffy->dead_text[3].height);
	luffy->dead_text[3].imgptr = (int *)mlx_get_data_addr(luffy->dead_text[3].img, &luffy->dead_text[3].bitPerPx, &luffy->dead_text[3].sizeLine, &luffy->dead_text[3].endian);
}

t_sprite	new_sprite(int x, int y)
{
	t_sprite sprite;

	sprite.attack_y = -1000;
	sprite.attack_x = -1000;
	sprite.x = x * CUBE_SIZE + CUBE_SIZE / 2;
	sprite.y = y * CUBE_SIZE + CUBE_SIZE / 2;
	sprite.z = -(0.45 * CUBE_SIZE);
	sprite.count = 0;
	sprite.status = 1;
	sprite.power = 9;
	sprite.last_frame = -1;
	sprite.calls = 0;
	sprite.dead_position = 0;
	return (sprite);
}

t_sprite	new_teach(t_data *data, int x, int y)
{
	t_sprite teach;

	teach = new_sprite(x, y);
	teach.life = 5;
	teach.attack_damage = 10;
	teach.speed = (CUBE_SIZE * 0.00070);
	teach.text = &data->sprite_text[TEACH];
	return (teach);
}

t_sprite	new_chopper(t_data *data, int x, int y)
{
	t_sprite chopper;

	chopper = new_sprite(x, y);
	chopper.life = 1;
	chopper.attack_damage = 2;
	chopper.speed = (CUBE_SIZE * 0.0025);
	chopper.text = &data->sprite_text[CHOPPER];
	return (chopper);
}

t_sprite	new_luffy(t_data *data, int x, int y)
{
	t_sprite luffy;

	luffy = new_sprite(x, y);
	luffy.life = 3;
	luffy.attack_damage = 5;
	luffy.speed = (CUBE_SIZE * 0.0015);
	luffy.text = &data->sprite_text[LUFFY];
	return (luffy);
}

int	set_player_and_sprites(t_data *data, char **map)
{
	int y;
	int x;
	int ret;
	int	count = 0;

	data->sprites = NULL;
	ret = 0;
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W')
			{
				if (ret > 0)
					return (ft_putstr_error("Error\nyou must have only one player starting position.\n"));
				init_player(data, x, y, map[y][x]);
				map[y][x] = '0';
				ret++;
			}
			else if (map[y][x] == 'L')
			{
				push_back(&data->sprites, new_luffy(data, x, y));
				map[y][x] = '0';
			}
			else if (map[y][x] == 'T')
			{
				push_back(&data->sprites, new_teach(data, x, y));
				map[y][x] = '0';
			}
			else if (map[y][x] == 'C')
			{
				push_back(&data->sprites, new_chopper(data, x, y));
				map[y][x] = '0';
			}
			else if (map[y][x] == 'G')
			{
				data->generators[count] = (t_generator *)malloc(sizeof(t_generator));
				if (!data->generators[count])
					return (1);
				data->generators[count]->mapX = x;
				data->generators[count]->mapY = y;
				data->generators[count]->timer = -1;
				data->generators[count]->nb_gen = 0;
				data->generators[count++]->wave = 0;
				map[y][x] = '0';
			}
			else if (map[y][x] != '1' && map[y][x] != '0' && map[y][x] != ' ' && map[y][x] != 'D')
				return (ft_putstr_error("Error\ninvalid component on the map\n"));
		}
	}
	data->generators[count] = NULL;
	if (ret == 0)
		return (ft_putstr_error("Error\nyou must have at least one player starting position.\n"));
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

int	set_doors(t_data *data, char **map)
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
		{
			if (map[y][x] == 'D')
			{
				data->doors[count] = (t_door *)malloc(sizeof(t_door));
				if (!data->doors[count])
					return (1);
				data->doors[count]->status = 1;
				data->doors[count]->x = x;
				data->doors[count]->y = y;
				data->doors[count]->start = 0;
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
	data->generators = (t_generator **)malloc(sizeof(t_generator *) * (count_component(data->vars.map, 'G') + 1));
	if (!data->generators)
		return (1);
	data->doors = (t_door **)malloc(sizeof(t_door *) * (count_component(data->vars.map, 'D') + 1));
	if (!data->doors)
		return (1);
	if (set_doors(data, data->vars.map))
		return (1);
	if (set_player_and_sprites(data, data->vars.map))
		return (1);
	return (0);
}

int	parse_all(t_data *data, char *path)
{
	int		fd;
	int		descriptionMapStart;
	char	*line;

	descriptionMapStart = 0;
	for (int i = 0; i < 6; i++)
		data->img[i].img = NULL;
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
			// BESOIN DE CLEAR LES TEXTURES DEJA ENREGISTREES
			return (1);
		}
		free(line);
		if (data->img[NORTH].img != NULL && data->img[SOUTH].img != NULL && data->img[EAST].img != NULL && data->img[WEST].img != NULL && data->img[FLOOR].img != NULL && data->img[CEILING].img != NULL)
			break ;
		descriptionMapStart++;
		line = get_next_line(fd);
	}
	if (data->img[NORTH].img == NULL || data->img[SOUTH].img == NULL || data->img[EAST].img == NULL || data->img[WEST].img == NULL)
	{
		// BESOIN DE CLEAR LES TEXTURES DEJA ENREGISTREES
		return (ft_putstr_error("Error\ntexture identifier missing\n"));
	}
	if (parse_map(data, fd, descriptionMapStart, path))
		return (1);
	if (fill_map(data))
		return (1);
	return (0);
}