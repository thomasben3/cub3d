/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attack_and_move_sprite.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 03:56:29 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/13 03:57:40 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_free_x(t_data *data, t_sprite *sprite, int deplacement, int marge)
{
	int	x;
	int	y;

	x = sprite->x;
	y = sprite->y;
	if (deplacement >= 0)
		return (is_in_map(data, (x + deplacement + marge) / CUBE_SIZE,
				y / CUBE_SIZE)
			&& (data->vars.map[y / CUBE_SIZE][(x + deplacement + marge)
				/ CUBE_SIZE] == '0'
			|| (data->vars.map[y / CUBE_SIZE][(x + deplacement + marge)
				/ CUBE_SIZE] == 'D'
			&& !there_is_door(data, x + deplacement + marge, y, EAST)))
			&& (!there_is_sprite(data, sprite, x + deplacement + marge, y)
				|| sprite->attack_x != -1000 || sprite->attack_y != -1000));
	else
		return (is_in_map(data, (x + deplacement - marge) / CUBE_SIZE,
				y / CUBE_SIZE)
			&& ((data->vars.map[y / CUBE_SIZE][(x + deplacement - marge)
					/ CUBE_SIZE] == '0'
				|| (data->vars.map[y / CUBE_SIZE][(x + deplacement - marge)
					/ CUBE_SIZE] == 'D'
				&& !there_is_door(data, x + deplacement - marge, y, EAST))))
			&& (!there_is_sprite(data, sprite, x + deplacement - marge, y)
				|| sprite->attack_x != -1000 || sprite->attack_y != -1000));
}

int	is_free_y(t_data *data, t_sprite *sprite, int deplacement, int marge)
{
	int	x;
	int	y;

	x = sprite->x;
	y = sprite->y;
	if (deplacement >= 0)
		return (is_in_map(data, x / CUBE_SIZE, (y + deplacement + marge)
				/ CUBE_SIZE)
			&& (data->vars.map[(y + deplacement + marge) / CUBE_SIZE]
				[x / CUBE_SIZE] == '0'
				|| (data->vars.map[(y + deplacement + marge) / CUBE_SIZE]
					[x / CUBE_SIZE] == 'D'
					&& !there_is_door(data, x, y + deplacement + marge, NORTH)))
			&& (!there_is_sprite(data, sprite, x, y + deplacement + marge)
				|| sprite->attack_x != -1000 || sprite->attack_y != -1000));
	else
		return (is_in_map(data, x / CUBE_SIZE, (y + deplacement - marge)
				/ CUBE_SIZE)
			&& (data->vars.map[(y + deplacement - marge) / CUBE_SIZE]
				[x / CUBE_SIZE] == '0'
				|| (data->vars.map[(y + deplacement - marge) / CUBE_SIZE]
					[x / CUBE_SIZE] == 'D'
					&& !there_is_door(data, x, y + deplacement - marge, NORTH)))
			&& (!there_is_sprite(data, sprite, x, y + deplacement - marge)
				|| sprite->attack_x != -1000 || sprite->attack_y != -1000));
}

void	move_sprite(
		t_data *data, t_sprite *sprite, double scale, t_img **texture)
{
	if (sprite->status != 2 && sprite->life > 0 && data->player.life > 0
		&& (sprite->attack_x == -1000 && sprite->attack_y == -1000)
		&& distance(data->player.x, data->player.y, sprite->x, sprite->y)
		< CUBE_SIZE * 25)
	{
		if (is_free_x(data, sprite, -cos(sprite->angle)
				* sprite->speed * data->fps, ((*texture)->width) * scale))
			sprite->x -= cos(sprite->angle) * (sprite->speed * (data->fps));
		if (is_free_y(data, sprite, -sin(sprite->angle)
				* sprite->speed * data->fps, ((*texture)->width) * scale))
			sprite->y += -sin(sprite->angle) * (sprite->speed * (data->fps));
		if (distance(data->player.x, data->player.y, sprite->x, sprite->y)
			< (CUBE_SIZE * 0.8))
			*texture = &sprite->text->attack_text[0];
	}
}

void	manage_sprite_dash(t_data *data, t_sprite *sprite)
{
	if (sprite->calls < 5)
	{
		sprite->power--;
		sprite->z += (CUBE_SIZE * 0.0035) * sprite->power;
	}
	else if (sprite->calls > 5 && sprite->calls < 11)
	{
		sprite->z -= (CUBE_SIZE * 0.0035) * sprite->power;
		sprite->power++;
		if (sprite->z < -(0.45 * CUBE_SIZE))
			sprite->z = -(0.45 * CUBE_SIZE);
	}
	if (is_free_x(data, sprite, -sprite->attack_x
			* (CUBE_SIZE * 0.08), sprite->text->text[0].width / 2))
			sprite->x -= sprite->attack_x * (CUBE_SIZE * 0.08);
	if (is_free_y(data, sprite, sprite->attack_y
			* (CUBE_SIZE * 0.08), sprite->text->text[0].width / 2))
		sprite->y += sprite->attack_y * (CUBE_SIZE * 0.08);
	if (++sprite->calls == 20)
	{
		sprite->calls = 0;
		sprite->count = 0;
		sprite->attack_x = -1000;
		sprite->attack_y = -1000;
	}
}

void	attack_sprite(t_data *data, t_sprite *sprite)
{
	if ((sprite->attack_x != -1000 || sprite->attack_y != -1000
			|| sprite->status == 2) && (sprite->last_frame == -1
			|| get_timestamp() - sprite->last_frame > 10))
	{
		if ((sprite->attack_x != -1000 || sprite->attack_y != -1000))
			manage_sprite_dash(data, sprite);
		else if (sprite->status == 2)
		{
			sprite->calls++;
			if (sprite->calls == 15)
			{
				sprite->calls = 0;
				sprite->status = 1;
			}
		}
		sprite->last_frame = get_timestamp();
	}
}
