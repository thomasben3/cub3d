/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbensem <thbensem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 02:41:47 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/06 15:43:47 by thbensem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_keys(t_keys *keys)
{
	keys->z = 0;
	keys->q = 0;
	keys->s = 0;
	keys->d = 0;
	keys->left = 0;
	keys->right = 0;
	keys->right = 0;
	keys->esp = 0;
	keys->left_click = 0;
	keys->enter = 0;
}

void	init_hands(t_data *data)
{
	data->hands[0].img
		= mlx_xpm_file_to_image(data->vars.mlx, "assets/hands/katana0.xpm",
			&data->hands[0].width, &data->hands[0].height);
	data->hands[0].imgptr
		= (int *)mlx_get_data_addr(data->hands[0].img, &data->hands[0].bitPerPx,
			&data->hands[0].sizeLine, &data->hands[0].endian);
	data->hands[1].img
		= mlx_xpm_file_to_image(data->vars.mlx, "assets/hands/katana1.xpm",
			&data->hands[1].width, &data->hands[1].height);
	data->hands[1].imgptr
		= (int *)mlx_get_data_addr(data->hands[1].img, &data->hands[1].bitPerPx,
			&data->hands[1].sizeLine, &data->hands[1].endian);
	data->hands[2].img
		= mlx_xpm_file_to_image(data->vars.mlx, "assets/hands/katana2.xpm",
			&data->hands[2].width, &data->hands[2].height);
	data->hands[2].imgptr
		= (int *)mlx_get_data_addr(data->hands[2].img, &data->hands[2].bitPerPx,
			&data->hands[2].sizeLine, &data->hands[2].endian);
	data->hands[3].img
		= mlx_xpm_file_to_image(data->vars.mlx, "assets/hands/katana3.xpm",
			&data->hands[3].width, &data->hands[3].height);
	data->hands[3].imgptr
		= (int *)mlx_get_data_addr(data->hands[3].img, &data->hands[3].bitPerPx,
			&data->hands[3].sizeLine, &data->hands[3].endian);
	data->hands_to_display = 0;
}
