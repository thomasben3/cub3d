/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 03:54:30 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:53:09 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	attack_player(t_player *player, double angle, t_sprite *sprite)
{
	player->in_jump = 1;
	player->attack_dir_x = cos(angle);
	player->attack_dir_y = -sin(angle);
	player->life -= sprite->attack_damage;
	player->mana -= 5;
	if (player->mana < 0)
		player->mana = 0;
	sprite->status = 2;
}

t_img	*get_sprite_texture(t_player *player, t_sprite *sprite, double angle)
{
	if (sprite->attack_x != -1000 || sprite->attack_y != -1000)
		return (&sprite->text->dead_text[0]);
	else if (sprite->life <= 0)
	{
		if (sprite->count > 3)
			return (&sprite->text->dead_text[3]);
		else
			return (&sprite->text->dead_text[sprite->count]);
	}
	else if (sprite->status == 2)
		return (&sprite->text->attack_text[1]);
	else if (distance(player->x, player->y, sprite->x, sprite->y)
		< (CUBE_SIZE * 0.8) && sprite->status != 2 && player->life > 0)
	{
		attack_player(player, angle, sprite);
		return (&sprite->text->attack_text[0]);
	}
	return (&sprite->text->text[sprite->count]);
}

void	get_sprite_pixel(
	t_data *data, t_sprite *sprite, t_norm *norm)
{
	int	color;

	if (sprite_is_facing_left(data, sprite))
		color = sprite->text_in_use->imgptr[
			(int)((sprite->text_in_use->height - 1)
				- (int)((sprite->text_in_use->height - 1) * norm->text_y))
			*(sprite->text_in_use->size_line / 4)
			+ ((sprite->text_in_use->width - 1)
				- (int)(((sprite->text_in_use->width) - 1) * norm->text_x))];
	else
		color = sprite->text_in_use->imgptr[
			(int)((sprite->text_in_use->height - 1)
				* (1 - norm->text_y)) *(sprite->text_in_use->size_line / 4)
			+ ((int)((sprite->text_in_use->width - 1) * norm->text_x))];
	if (color != create_trgb(255, 0, 0, 0))
		data->frame.imgptr[norm->y * data->frame.size_line / 4 + norm->x]
			= shadow_color(color, distance(data->player.x,
					data->player.y, sprite->x, sprite->y));
}

int	need_to_draw_sprite_px(t_data *data, t_sprite *sprite, t_norm *norm)
{
	return (distance(data->player.x - data->player.dx, data->player.y
			+ data->player.dy, sprite->x, sprite->y)
		< distance(data->player.x, data->player.y, sprite->x, sprite->y)
		&& sprite->x_on_screen + norm->tmp >= 0
		&& sprite->x_on_screen + norm->tmp < SCREEN_WIDTH
		&& (distance(data->player.x, data->player.y, sprite->x,
				sprite->y) * cos(fix_angle(data->player.dir
					- data->ray_save[(int)sprite->x_on_screen
						+ norm->tmp].angle))
			<= data->ray_save[(int)sprite->x_on_screen + norm->tmp].length)
		&& norm->x >= 0 && norm->y >= 0 && norm->x < SCREEN_WIDTH
		&& norm->y < SCREEN_HEIGHT);
}

void	draw_sprite(
		t_data *data, t_sprite *s, t_img *t, double scale)
{
	t_norm	norm;

	norm.tmp = -((t->width * scale) / 2);
	norm.x = s->x_on_screen - ((t->width * scale) / 2);
	norm.end_x = s->x_on_screen + ((t->width * scale) / 2);
	while (norm.x < norm.end_x && norm.x < SCREEN_WIDTH)
	{
		norm.y = s->y_on_screen - (t->height * (scale));
		if (norm.y < 0)
			norm.y = 0;
		while (norm.y < s->y_on_screen && norm.y < SCREEN_HEIGHT - 1)
		{
			if (need_to_draw_sprite_px(data, s, &norm))
			{
				norm.text_x = (double)(norm.end_x - 1 - norm.x)
					/ (double)((s->text_in_use->width) * (scale));
				norm.text_y = (double)(s->y_on_screen - 1 - norm.y)
					/ (double)((s->text_in_use->height) * (scale));
				get_sprite_pixel(data, s, &norm);
			}
			(norm.y)++;
		}
		(norm.tmp)++;
		(norm.x)++;
	}
}
