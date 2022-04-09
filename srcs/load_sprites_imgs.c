/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_sprites_imgs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 01:55:22 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/09 02:27:10 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	load_img(t_data *data, t_img *img, char *path)
{
	img->img = mlx_xpm_file_to_image(data->vars.mlx, path,
			&img->width, &img->height);
	img->imgptr = (int *)mlx_get_data_addr(img->img,
			&img->bitPerPx, &img->sizeLine,
			&img->endian);
}

void	load_teach_imgs(t_data *data, t_sprite_text *teach)
{
	load_img(data, &teach->text[0], "assets/teach/teach0.xpm");
	load_img(data, &teach->text[1], "assets/teach/teach1.xpm");
	load_img(data, &teach->text[2], "assets/teach/teach2.xpm");
	load_img(data, &teach->text[3], "assets/teach/teach3.xpm");
	load_img(data, &teach->text[4], "assets/teach/teach4.xpm");
	load_img(data, &teach->text[5], "assets/teach/teach5.xpm");
	load_img(data, &teach->attack_text[0], "assets/teach/teach_attack0.xpm");
	load_img(data, &teach->attack_text[1], "assets/teach/teach_attack1.xpm");
	load_img(data, &teach->dead_text[0], "assets/teach/teach_dead0.xpm");
	load_img(data, &teach->dead_text[1], "assets/teach/teach_dead1.xpm");
	load_img(data, &teach->dead_text[2], "assets/teach/teach_dead2.xpm");
	load_img(data, &teach->dead_text[3], "assets/teach/teach_dead3.xpm");
}

void	load_chopper_imgs(t_data *data, t_sprite_text *chopper)
{
	load_img(data, &chopper->text[0], "assets/chopper/chopper0.xpm");
	load_img(data, &chopper->text[1], "assets/chopper/chopper1.xpm");
	load_img(data, &chopper->text[2], "assets/chopper/chopper2.xpm");
	load_img(data, &chopper->text[3], "assets/chopper/chopper3.xpm");
	load_img(data, &chopper->text[4], "assets/chopper/chopper4.xpm");
	load_img(data, &chopper->text[5], "assets/chopper/chopper5.xpm");
	load_img(data, &chopper->attack_text[0],
		"assets/chopper/chopper_attack0.xpm");
	load_img(data, &chopper->attack_text[1],
		"assets/chopper/chopper_attack1.xpm");
	load_img(data, &chopper->dead_text[0], "assets/chopper/chopper_dead0.xpm");
	load_img(data, &chopper->dead_text[1], "assets/chopper/chopper_dead1.xpm");
	load_img(data, &chopper->dead_text[2], "assets/chopper/chopper_dead2.xpm");
	load_img(data, &chopper->dead_text[3], "assets/chopper/chopper_dead3.xpm");
}

void	load_luffy_imgs(t_data *data, t_sprite_text *luffy)
{
	load_img(data, &luffy->text[0], "assets/luffy/luffy0.xpm");
	load_img(data, &luffy->text[1], "assets/luffy/luffy1.xpm");
	load_img(data, &luffy->text[2], "assets/luffy/luffy2.xpm");
	load_img(data, &luffy->text[3], "assets/luffy/luffy3.xpm");
	load_img(data, &luffy->text[4], "assets/luffy/luffy4.xpm");
	load_img(data, &luffy->text[5], "assets/luffy/luffy5.xpm");
	load_img(data, &luffy->attack_text[0], "assets/luffy/luffy_attack0.xpm");
	load_img(data, &luffy->attack_text[1], "assets/luffy/luffy_attack1.xpm");
	load_img(data, &luffy->dead_text[0], "assets/luffy/luffy_dead0.xpm");
	load_img(data, &luffy->dead_text[1], "assets/luffy/luffy_dead1.xpm");
	load_img(data, &luffy->dead_text[2], "assets/luffy/luffy_dead2.xpm");
	load_img(data, &luffy->dead_text[3], "assets/luffy/luffy_dead3.xpm");
}
