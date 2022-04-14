/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:40:32 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 02:48:35 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

double	distance(double ax, double ay, double bx, double by)
{
	return (sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)));
}

double	fix_angle(double f)
{
	if (f < 0)
		return (f + 2 * M_PI);
	else if (f > 2 * M_PI)
		return (f - 2 * M_PI);
	return (f);
}

void	display_wave_nb(t_data *data)
{
	char	*tmp;
	char	*wave;

	tmp = ft_itoa(data->generators[0]->wave);
	wave = ft_join("WAVE: ", tmp);
	free(tmp);
	mlx_string_put(data->vars.mlx, data->vars.win,
		SCREEN_WIDTH / 2 - 40, 20, 0x00FFFFFF, wave);
	free(wave);
}

void	pause_pressed(t_data *data)
{
	data->pause *= -1;
	if (data->pause == 1)
	{
		mlx_string_put(data->vars.mlx, data->vars.win, SCREEN_WIDTH
			/ 2 - 20, SCREEN_HEIGHT / 2, 0x00FFFFFFFF, "PAUSE");
		mlx_mouse_show(data->vars.mlx, data->vars.win);
	}
	else
		mlx_mouse_hide(data->vars.mlx, data->vars.win);
}
