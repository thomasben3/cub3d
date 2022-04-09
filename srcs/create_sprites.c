/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_sprites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 01:48:05 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/09 01:49:53 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_sprite	new_sprite(int x, int y)
{
	t_sprite sprite;

	sprite.attack_y = -1000;
	sprite.attack_x = -1000;
	sprite.x = x * CUBE_SIZE + CUBE_SIZE / 2;
	sprite.y = y * CUBE_SIZE + CUBE_SIZE / 2;
	sprite.z = -(0.45 * CUBE_SIZE);
	sprite.count = 0;
	sprite.status = 1;
	sprite.power = 9;
	sprite.last_frame = -1;
	sprite.calls = 0;
	sprite.dead_position = 0;
	return (sprite);
}

t_sprite	new_teach(t_data *data, int x, int y)
{
	t_sprite teach;

	teach = new_sprite(x, y);
	teach.life = 5;
	teach.attack_damage = 10;
	teach.speed = (CUBE_SIZE * 0.00070);
	teach.text = &data->sprite_text[TEACH];
	return (teach);
}

t_sprite	new_chopper(t_data *data, int x, int y)
{
	t_sprite chopper;

	chopper = new_sprite(x, y);
	chopper.life = 1;
	chopper.attack_damage = 2;
	chopper.speed = (CUBE_SIZE * 0.0025);
	chopper.text = &data->sprite_text[CHOPPER];
	return (chopper);
}

t_sprite	new_luffy(t_data *data, int x, int y)
{
	t_sprite luffy;

	luffy = new_sprite(x, y);
	luffy.life = 3;
	luffy.attack_damage = 5;
	luffy.speed = (CUBE_SIZE * 0.0015);
	luffy.text = &data->sprite_text[LUFFY];
	return (luffy);
}
