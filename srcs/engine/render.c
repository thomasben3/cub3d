/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 15:45:06 by thbensem          #+#    #+#             */
/*   Updated: 2022/04/14 03:53:07 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	calculate_door_decalage(t_data *data, t_img **texture, t_ray *ray)
{
	int	i;

	if (data->vars.map[(int)ray->ry / CUBE_SIZE]
		[(int)ray->rx / CUBE_SIZE] != 'D')
		return (0);
	*texture = &data->door_text;
	i = 0;
	while (data->doors[i] && (data->doors[i]->x != (int)ray->rx
			/ CUBE_SIZE || data->doors[i]->y != (int)ray->ry / CUBE_SIZE))
		i++;
	return (data->doors[i]->start * (*texture)->width);
}

void	draw_raycast(t_data *data, t_ray *ray, int face)
{
	t_img	*texture;
	int		decalage;

	texture = &data->img[face];
	decalage = calculate_door_decalage(data, &texture, ray);
	if (face == NORTH)
		draw_text_vline(data, texture, *ray, (((int)ray->rx % CUBE_SIZE)
				/ (double)CUBE_SIZE) * texture->width - decalage);
	else if (face == SOUTH)
		draw_text_vline(data, texture, *ray,
			(texture->width - 1) - ((((int)ray->rx % CUBE_SIZE)
					/ (double)CUBE_SIZE) * texture->width) + decalage);
	else if (face == EAST)
		draw_text_vline(data, texture, *ray, (((int)ray->ry % CUBE_SIZE)
				/ (double)CUBE_SIZE) * texture->width - decalage);
	else if (face == WEST)
		draw_text_vline(data, texture, *ray,
			(texture->width - 1) - ((((int)ray->ry % CUBE_SIZE)
					/ (double)CUBE_SIZE) * texture->width) + decalage);
	draw_sky(ray->nb, 0, ray->line_start, data);
	if (DEBUG)
		draw_vline(ray, SCREEN_HEIGHT, data, create_rgb(80, 47, 0));
	else
		draw_floor(data, *ray, &data->img[FLOOR], ray->nb);
}

t_ray	find_face_and_shortest_ray(t_ray v_ray, t_ray h_ray, int *face)
{
	if (v_ray.length < h_ray.length)
	{
		if (v_ray.angle < 3 * (M_PI / 2) && v_ray.angle > M_PI / 2)
			*face = EAST;
		else
			*face = WEST;
		return (v_ray);
	}
	if (h_ray.angle > M_PI)
		*face = SOUTH;
	else
		*face = NORTH;
	return (h_ray);
}

t_ray	raycasting(t_data *data, t_ray v_ray, t_ray h_ray, int *face)
{
	if (h_ray.angle < M_PI)
		check_north(data, &h_ray);
	else if (h_ray.angle > M_PI)
		check_south(data, &h_ray);
	if (h_ray.angle == M_PI || h_ray.angle == 0)
	{
		h_ray.rx = data->player.x;
		h_ray.ry = data->player.y;
		h_ray.length = 100000 * CUBE_SIZE;
	}
	if (v_ray.angle > 3 * (M_PI / 2) || v_ray.angle < M_PI / 2)
		check_west(data, &v_ray);
	else if (v_ray.angle < 3 * (M_PI / 2) && v_ray.angle > M_PI / 2)
		check_east(data, &v_ray);
	else if (v_ray.angle == 3 * (M_PI / 2) || v_ray.angle == M_PI / 2)
	{
		v_ray.rx = data->player.x;
		v_ray.ry = data->player.y;
		v_ray.length = 100000 * CUBE_SIZE;
	}
	return (find_face_and_shortest_ray(v_ray, h_ray, face));
}

void	draw_rays(t_data *data)
{
	t_ray	ray;
	int		face;

	ray.angle = fix_angle(data->player.dir - ((FOV / 2) * DEGREE));
	face = -1;
	ray.nb = -1;
	while (++ray.nb < SCREEN_WIDTH)
	{
		ray = raycasting(data, ray, ray, &face);
		if (!FISHEYE)
			ray.length *= cos(fix_angle(data->player.dir - ray.angle));
		ray.line_height = ((SCREEN_HEIGHT * CUBE_SIZE) / ray.length);
		ray.line_start = -(ray.line_height * (1 - data->camera_height))
			+ data->horizon_line;
		if (ray.line_start < 0)
			ray.line_start = 0;
		ray.line_end = (ray.line_height * data->camera_height)
			+ data->horizon_line;
		if (ray.line_end >= SCREEN_HEIGHT)
			ray.line_end = SCREEN_HEIGHT - 1;
		draw_raycast(data, &ray, face);
		data->ray_save[ray.nb] = ray;
		ray.angle = fix_angle(ray.angle + (DEGREE / (SCREEN_WIDTH / FOV)));
	}
}
