/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 02:41:47 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/09 22:36:45 by tbensem          ###   ########.fr       */
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
	load_img(data, &data->hands[0], "assets/hands/katana0.xpm");
	load_img(data, &data->hands[1], "assets/hands/katana1.xpm");
	load_img(data, &data->hands[2], "assets/hands/katana2.xpm");
	load_img(data, &data->hands[3], "assets/hands/katana3.xpm");
	data->hands_to_display = 0;
}
