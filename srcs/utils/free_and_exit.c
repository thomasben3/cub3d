/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 02:45:27 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:23:24 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	free_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < 6)
	{
		if (data->img[i].img)
			mlx_destroy_image(data->vars.mlx, data->img[i].img);
		data->img[i].img = NULL;
	}
}

void	free_all(t_data *data)
{
	int		i;

	free_double_array(data->vars.map);
	free_textures(data);
	list_clear(&data->sprites);
	i = -1;
	while (data->generators[++i])
		free(data->generators[i]);
	free(data->generators);
	i = -1;
	while (data->doors[++i])
		free(data->doors[i]);
	free(data->doors);
}

void	free_sprites_imgs(t_data *data)
{
	int	j;
	int	i;

	j = -1;
	while (++j < 3)
	{
		i = -1;
		while (++i < 6)
			mlx_destroy_image(data->vars.mlx, data->sprite_text[j].text[i].img);
		mlx_destroy_image(data->vars.mlx,
			data->sprite_text[j].attack_text[0].img);
		mlx_destroy_image(data->vars.mlx,
			data->sprite_text[j].attack_text[1].img);
		i = -1;
		while (++i < 4)
			mlx_destroy_image(data->vars.mlx,
				data->sprite_text[j].dead_text[i].img);
	}
}

int	ft_exit(t_data *data)
{
	int	i;

	free_all(data);
	free_sprites_imgs(data);
	i = -1;
	while (++i < 4)
		mlx_destroy_image(data->vars.mlx, data->hands[i].img);
	mlx_destroy_image(data->vars.mlx, data->door_text.img);
	mlx_destroy_image(data->vars.mlx, data->frame.img);
	mlx_destroy_window(data->vars.mlx, data->vars.win);
	free(data->vars.mlx);
	exit(EXIT_SUCCESS);
}
