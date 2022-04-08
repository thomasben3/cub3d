/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 15:45:06 by thbensem          #+#    #+#             */
/*   Updated: 2022/04/08 19:08:49 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_sky(int x, int start, int end, t_data *data)
{
	int	x1;

	x1 = x + (data->img[CEILING].width / (2 * M_PI) * data->player.dir);
	if (x1 >= data->img[CEILING].width)
		x1 -= data->img[CEILING].width;
	while (start < end)
	{
		data->frame.imgptr[start * (data->frame.sizeLine / 4) + x]
			= data->img[CEILING].imgptr[(int)(data->img[CEILING].height - 350
				- (data->horizon_line - start))
			* (data->img[CEILING].sizeLine / 4) + x1];
		start++;
	}
}

void	draw_raycast(t_data *data, t_ray *ray, int face)
{
	t_img	*texture;
	int		decalage;

	texture = &data->img[face];
	decalage = 0;
	if (data->vars.map[(int)ray->ry / CUBE_SIZE]
		[(int)ray->rx / CUBE_SIZE] == 'D')
	{
		texture = &data->img[CEILING];
		while (data->doors[decalage] && (data->doors[decalage]->x != (int)ray->rx / CUBE_SIZE || data->doors[decalage]->y != (int)ray->ry / CUBE_SIZE))
			decalage++;
		decalage = data->doors[decalage]->start * texture->width;
	}
	if (face == NORTH)
		draw_text_vline(data, texture, *ray,
			(((int)ray->rx % CUBE_SIZE) / (double)CUBE_SIZE) * texture->width - decalage);
	else if (face == SOUTH)
		draw_text_vline(data, texture, *ray,
			(texture->width - 1) - ((((int)ray->rx % CUBE_SIZE)
					/ (double)CUBE_SIZE) * texture->width) + decalage);
	else if (face == EAST)
		draw_text_vline(data, texture, *ray,
			(((int)ray->ry % CUBE_SIZE) / (double)CUBE_SIZE) * texture->width - decalage);
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
		if (v_ray.Angle < 3 * (M_PI / 2) && v_ray.Angle > M_PI / 2)
			*face = EAST;
		else
			*face = WEST;
		return (v_ray);
	}
	if (h_ray.Angle > M_PI)
		*face = SOUTH;
	else
		*face = NORTH;
	return (h_ray);
}

t_ray	raycasting(t_data *data, t_ray v_ray, t_ray h_ray, int *face)
{
	if (h_ray.Angle < M_PI)
		check_north(data, &h_ray);
	else if (h_ray.Angle > M_PI)
		check_south(data, &h_ray);
	if (h_ray.Angle == M_PI || h_ray.Angle == 0)
	{
		h_ray.rx = data->player.x;
		h_ray.ry = data->player.y;
		h_ray.length = 100000 * CUBE_SIZE;
	}
	if (v_ray.Angle > 3 * (M_PI / 2) || v_ray.Angle < M_PI / 2)
		check_west(data, &v_ray);
	else if (v_ray.Angle < 3 * (M_PI / 2) && v_ray.Angle > M_PI / 2)
		check_east(data, &v_ray);
	else if (v_ray.Angle == 3 * (M_PI / 2) || v_ray.Angle == M_PI / 2)
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

	ray.Angle = fix_angle(data->player.dir - ((FOV / 2) * DEGREE));
	face = -1;
	ray.nb = -1;
	while (++ray.nb < SCREEN_WIDTH)
	{
		ray = raycasting(data, ray, ray, &face);
		if (!FISHEYE)
			ray.length *= cos(fix_angle(data->player.dir - ray.Angle));
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
		ray.Angle = fix_angle(ray.Angle + (DEGREE / (SCREEN_WIDTH / FOV)));
	}
}
