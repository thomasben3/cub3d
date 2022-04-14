/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:34:31 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:53:02 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	create_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

void	draw_text_vline(t_data *data, t_img *img, t_ray ray, int x_in_text)
{
	int		truestart;
	int		y1;
	int		color;

	truestart
		= -(ray.line_height * (1 - data->camera_height)) + data->horizon_line;
	while (ray.line_start < ray.line_end)
	{
		y1 = ((double)(ray.line_start - truestart)
				/ (double)ray.line_height) * (img->height - 1);
		if (y1 >= img->height || y1 < 0)
			printf("y1 = %d\n", y1);// A VIRER A TERME
		if (x_in_text < 0 || x_in_text >= img->width) // A VIRER A TERME
			printf("x_in_text = %d\n", x_in_text);// A VIRER A TERME
		color = img->imgptr[(int)(y1 * (img->size_line / 4) + x_in_text)];
		data->frame.imgptr[ray.line_start * data->frame.size_line / 4 + ray.nb]
			= shadow_color(color, ray.length);
		(ray.line_start)++;
	}
}

void	draw_vline(t_ray *ray, int end, t_data *data, int color)
{
	while (ray->line_end < end)
	{
		data->frame.imgptr[ray->line_end * data->frame.size_line / 4 + ray->nb]
			= color;
		ray->line_end++;
	}
}

void	draw_floor(t_data *data, t_ray ray, t_img *text, int x)
{
	int		text_x;
	int		text_y;
	float	dy;
	float	ra_fix;

	ra_fix = cos(fix_angle(data->player.dir - ray.angle));
	while (ray.line_end < SCREEN_HEIGHT)
	{
		dy = ray.line_end - data->horizon_line;
		text_x = (int)(data->player.x - (cos(ray.angle) * (SCREEN_HEIGHT / 2)
					* (text->width * 2 * data->camera_height) / dy / ra_fix));
		text_y = (int)(data->player.y - (sin(ray.angle) * (SCREEN_HEIGHT / 2)
					* (text->height * 2 * data->camera_height) / dy / ra_fix));
		data->frame.imgptr[ray.line_end * (data->frame.size_line / 4) + x]
			= shadow_color(text->imgptr[(text_y & (text->height - 1))
				* (text->size_line / 4) + (text_x & (text->width - 1))],
				(CUBE_SIZE / ((ray.line_end - data->horizon_line) * 0.005))
				* 2 * data->camera_height);
		(ray.line_end)++;
	}
}

void	draw_sky(int x, int start, int end, t_data *data)
{
	int	x1;

	x1 = x + (data->img[CEILING].width / (2 * M_PI) * data->player.dir);
	if (x1 >= data->img[CEILING].width)
		x1 -= data->img[CEILING].width;
	while (start < end)
	{
		data->frame.imgptr[start * (data->frame.size_line / 4) + x]
			= data->img[CEILING].imgptr[(int)(data->img[CEILING].height - 350
				- (data->horizon_line - start))
			* (data->img[CEILING].size_line / 4) + x1];
		start++;
	}
}
