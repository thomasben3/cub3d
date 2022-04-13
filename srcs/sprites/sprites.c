/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 00:50:54 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/13 04:09:16 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	there_is_sprite(t_data *data, t_sprite *sprite, int x, int y)
{
	t_list	*curr;

	curr = data->sprites;
	while (curr)
	{
		if (&curr->sprite == sprite)
		{
			curr = curr->next;
			continue ;
		}
		if (curr->sprite.life > 0
			&& distance(x, y, curr->sprite.x, curr->sprite.y) < CUBE_SIZE * 0.5)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	manage_one_sprite(t_data *data, t_sprite *sprite, double scale)
{
	attack_sprite(data, sprite);
	sprite->angle
		= atan2(sprite->y - data->player.y, sprite->x - data->player.x);
	sprite->text_in_use
		= get_sprite_texture(&data->player, sprite, sprite->angle);
	move_sprite(data, sprite, scale, &sprite->text_in_use);
	draw_sprite(data, sprite, sprite->text_in_use, scale);
	if (sprite->last_frame != -1 && get_timestamp() - sprite->last_frame > 80)
	{
		sprite->count++;
		sprite->last_frame = get_timestamp();
	}
	if (sprite->count == 6 && sprite->life > 0)
		sprite->count = 0;
	else if (sprite->life <= 0 && sprite->attack_x == -1000
		&& sprite->attack_y == -1000 && sprite->count == 100)
		sprite->status = -1;
	if (sprite->last_frame == -1)
		sprite->last_frame = get_timestamp();
}

void	get_sprite_pos_on_screen(t_data *data, t_sprite *sprite)
{
	float	sz;
	float	sy;
	float	sx;
	float	a;
	float	b;

	sx = sprite->x - data->player.x;
	sy = sprite->y - data->player.y;
	sz = (sprite->z * 2) * data->camera_height;
	a = sy * (data->player.dx) + sx * (data->player.dy);
	b = sx * (data->player.dx) - sy * (data->player.dy);
	sx = a;
	sy = b;
	sprite->x_on_screen = sx * ((CUBE_SIZE * 0.80) / sy) + (SCREEN_WIDTH / 2);
	sprite->y_on_screen = sz * ((CUBE_SIZE) / sy) + (data->horizon_line);
}

void	set_attack_on_sprite(t_data *data, t_sprite *sprite)
{
	double	tmpangle;
	int		power;

	data->player.attack = 0;
	tmpangle = atan2(sprite->y - data->player.y,
			sprite->x - data->player.x);
	power = 1;
	if (data->player.in_jump && data->player.attack_dir_x == 0
		&& data->player.attack_dir_y == 0)
	{
		power = 2;
		data->player.mana += 5;
	}
	else
		data->player.mana += 2;
	sprite->life -= power;
	sprite->attack_x = -cos(tmpangle) * power;
	sprite->attack_y = sin(tmpangle) * power;
	if (sprite->life <= 0
		&& sprite_is_facing_left(data, sprite))
		sprite->dead_position = 1;
	if (data->player.mana > 100)
		data->player.mana = 100;
	sprite->calls = 0;
	sprite->status = 1;
}

void	draw_sprites(t_data *data)
{
	t_list	*curr;
	t_list	*tmp;
	double	tmpangle;

	sort_list(data, &data->sprites);
	tmp = data->sprites;
	while (tmp)
	{
		curr = tmp;
		tmp = tmp->next;
		get_sprite_pos_on_screen(data, &curr->sprite);
		if (data->player.attack && distance(data->player.x, data->player.y,
				curr->sprite.x, curr->sprite.y) < CUBE_SIZE
			&& curr->sprite.x_on_screen > SCREEN_WIDTH * 0.3
			&& curr->sprite.x_on_screen < SCREEN_WIDTH * 0.7
			&& curr->sprite.life > 0)
			set_attack_on_sprite(data, &curr->sprite);
		tmpangle = calculate_sprite_angle(data, &curr->sprite);
		manage_one_sprite(data, &curr->sprite, (SCREEN_WIDTH * 2)
			/ (distance(data->player.x, data->player.y,
					curr->sprite.x, curr->sprite.y)
				* cos(fix_angle(data->player.dir - tmpangle))));
	}
	free_dead_sprites(&data->sprites);
}
