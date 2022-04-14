/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_and_life.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:13:38 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:50:05 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_life(t_data *data, int i, int y, int x)
{
	while (++y < 30)
	{
		x = -1;
		while (++x < 3 * 100)
		{
			if (x % 3 == 0)
				i++;
			if (data->player.life >= i)
				data->frame.imgptr[(10 + y) * data->frame.size_line / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(200, 0, 0);
			else
				data->frame.imgptr[(10 + y) * data->frame.size_line / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(30, 30, 30);
			if (y < 5 && x >= 60 * 3 - 1 && x <= 60 * 3 + 1)
				data->frame.imgptr[(45 + y) * data->frame.size_line / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(255, 255, 255);
			else if (y < 5 && data->player.mana >= i)
				data->frame.imgptr[(45 + y) * data->frame.size_line / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(255, 210, 0);
			else if (y < 5)
				data->frame.imgptr[(45 + y) * data->frame.size_line / 4
					+ (SCREEN_WIDTH - 10 - x)] = create_rgb(175, 130, 0);
		}
		i = 0;
	}
}

void	draw_square(t_data *data, int x, int y, int size)
{
	int	x1;
	int	y1;

	if (size % 2 != 0)
		size--;
	y1 = y - size / 2;
	if (y1 < 0)
		y1 = 0;
	while (y1 < y + size / 2 && y1 < SCREEN_WIDTH - 1)
	{	
		x1 = x - size / 2;
		if (x1 < 0)
			x1 = 0;
		while (x1 < x + size / 2 && x1 < SCREEN_WIDTH - 1)
		{
			data->frame.imgptr[y1 * (data->frame.size_line / 4) + x1]
				= create_rgb(255, 0, 255);
			x1++;
		}
		y1++;
	}
}

int	check_for_sprite(t_data *data, int x, int y)
{
	t_list	*curr;

	curr = data->sprites;
	while (curr)
	{
		if (curr->sprite.life > 0 && distance(
				x, y, curr->sprite.x, curr->sprite.y) < CUBE_SIZE * 0.25)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	draw_minimap_line(t_data *data, float start_y, int y,
		int minimap_size_x)
{
	int	x;
	int	start_x;

	x = 10;
	start_x = data->player.x - (CUBE_SIZE / 10) * (minimap_size_x / 2);
	while (x < minimap_size_x + 10)
	{
		if (is_in_map(data, start_x / CUBE_SIZE, start_y / CUBE_SIZE)
			&& data->vars.map[(int)start_y / CUBE_SIZE]
			[(int)start_x / CUBE_SIZE] == '1')
			data->frame.imgptr[y * data->frame.size_line / 4 + x] = 0xFFFFFFF;
		else if (is_in_map(data, start_x / CUBE_SIZE, start_y / CUBE_SIZE)
			&& data->vars.map[(int)start_y / CUBE_SIZE]
			[(int)start_x / CUBE_SIZE] == 'D')
			data->frame.imgptr[y * data->frame.size_line / 4 + x]
				= create_rgb(255, 200, 0);
		if (check_for_sprite(data, start_x, start_y))
			data->frame.imgptr[y * data->frame.size_line / 4 + x]
				= create_rgb(255, 0, 0);
		start_x += CUBE_SIZE / 10;
		x++;
	}
}

void	draw_mini_map(t_data *data)
{
	int		minimap_size_y;
	int		minimap_size_x;
	float	start_y;
	int		y;

	minimap_size_x = (SCREEN_WIDTH / 4);
	minimap_size_y = (SCREEN_HEIGHT / 4);
	start_y = data->player.y - (CUBE_SIZE / 10) * (minimap_size_y / 2);
	y = 10;
	while (y < minimap_size_y + 10)
	{
		draw_minimap_line(data, start_y, y, minimap_size_x);
		start_y += CUBE_SIZE / 10;
		y++;
	}
	draw_square(data, (minimap_size_x / 2 + 10), (minimap_size_y / 2 + 10), 5);
	draw_square(data, (minimap_size_x / 2 + 10 - data->player.dx * 5),
		(minimap_size_y / 2 + 10 + data->player.dy * 5), 2);
	draw_square(data, (minimap_size_x / 2 + 10 - data->player.dx * 7),
		(minimap_size_y / 2 + 10 + data->player.dy * 7), 2);
	draw_square(data, (minimap_size_x / 2 + 10 - data->player.dx * 9),
		(minimap_size_y / 2 + 10 + data->player.dy * 9), 2);
}
