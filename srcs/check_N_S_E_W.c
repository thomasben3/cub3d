/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_N_S_E_W.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbensem <thbensem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 15:49:48 by thbensem          #+#    #+#             */
/*   Updated: 2022/04/06 15:50:24 by thbensem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	find_distance(t_data *data, t_ray *ray)
{
	int	i;
	int	map_x;
	int	map_y;
	int	max;

	max = sqrt((data->vars.mapWidth * data->vars.mapWidth)
			+ (data->vars.mapHeight * data->vars.mapHeight));
	i = 0;
	while (i < max)
	{
		map_x = (int)ray->rx / CUBE_SIZE;
		map_y = (int)ray->ry / CUBE_SIZE;
		if (is_in_map(data, map_x, map_y)
			&& data->vars.map[map_y][map_x] != '0')
			break ;
		else
		{
			ray->rx += ray->rxo;
			ray->ry += ray->ryo;
			i++;
		}
	}
	ray->length = distance(data->player.x, data->player.y, ray->rx, ray->ry);
}

void	check_north(t_data *data, t_ray *ray)
{
	double	neg_i_tan;

	neg_i_tan = -1 / tan(ray->Angle);
	ray->ry
		= ((int)data->player.y) - ((int)data->player.y % CUBE_SIZE) - 0.0001;
	ray->rx = (data->player.y - ray->ry) * neg_i_tan + data->player.x;
	ray->ryo = -CUBE_SIZE;
	ray->rxo = -ray->ryo * neg_i_tan;
	find_distance(data, ray);
}

void	check_south(t_data *data, t_ray *ray)
{
	double	neg_i_tan;

	neg_i_tan = -1 / tan(ray->Angle);
	ray->ry = ((int)data->player.y + CUBE_SIZE)
		- ((int)data->player.y % CUBE_SIZE);
	ray->rx = (data->player.y - ray->ry) * neg_i_tan + data->player.x;
	ray->ryo = CUBE_SIZE;
	ray->rxo = -ray->ryo * neg_i_tan;
	find_distance(data, ray);
}

void	check_east(t_data *data, t_ray *ray)
{
	double	neg_tan;

	neg_tan = -tan(ray->Angle);
	ray->rx = ((int)data->player.x + CUBE_SIZE)
		- ((int)data->player.x % CUBE_SIZE);
	ray->ry = (data->player.x - ray->rx) * neg_tan + data->player.y;
	ray->rxo = CUBE_SIZE;
	ray->ryo = -ray->rxo * neg_tan;
	find_distance(data, ray);
}

void	check_west(t_data *data, t_ray *ray)
{
	double	neg_tan;

	neg_tan = -tan(ray->Angle);
	ray->rx = ((int)data->player.x)
		- ((int)data->player.x % CUBE_SIZE) - 0.0001;
	ray->ry = (data->player.x - ray->rx) * neg_tan + data->player.y;
	ray->rxo = -CUBE_SIZE;
	ray->ryo = -ray->rxo * neg_tan;
	find_distance(data, ray);
}
