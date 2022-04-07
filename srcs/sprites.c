/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbensem <thbensem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 00:50:54 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/06 17:18:45 by thbensem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	else if (distance(player->x, player->y, sprite->x, sprite->y) < (CUBE_SIZE * 0.8) && sprite->status != 2 && player->life > 0)
	{
		attack_player(player, angle, sprite);
		return (&sprite->text->attack_text[0]);
	}
	return (&sprite->text->text[sprite->count]);
}

int	there_is_sprite(t_data *data, t_sprite *sprite, int x, int y)
{
	t_list *curr;

	curr = data->sprites;
	while (curr)
	{
		if (&curr->sprite == sprite)
		{
			curr = curr->next;
			continue ;
		}
		if (curr->sprite.life > 0 && distance(x, y, curr->sprite.x, curr->sprite.y) < CUBE_SIZE * 0.5)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	is_free_x(t_data *data, t_sprite *sprite, int deplacement, int marge)
{
	int	x;
	int	y;

	x = sprite->x;
	y = sprite->y;
	if (deplacement >= 0)
		return (is_in_map(data, (x + deplacement + marge) / CUBE_SIZE, y / CUBE_SIZE)
			&& (data->vars.map[y / CUBE_SIZE][(x + deplacement + marge) / CUBE_SIZE] == '0')
			&& (!there_is_sprite(data, sprite, x + deplacement + marge, y) || sprite->attack_x != -1000 || sprite->attack_y != -1000));
	else
		return (is_in_map(data, (x + deplacement - marge) / CUBE_SIZE, y / CUBE_SIZE)
			&& (data->vars.map[y / CUBE_SIZE][(x + deplacement - marge) / CUBE_SIZE] == '0')
			&& (!there_is_sprite(data, sprite, x + deplacement - marge, y) || sprite->attack_x != -1000 || sprite->attack_y != -1000));
}

int	is_free_y(t_data *data, t_sprite *sprite, int deplacement, int marge)
{
	int x;
	int y;

	x = sprite->x;
	y = sprite->y;
	if (deplacement >= 0)
		return (is_in_map(data, x / CUBE_SIZE, (y + deplacement + marge) / CUBE_SIZE)
			&& (data->vars.map[(y + deplacement + marge) / CUBE_SIZE][x / CUBE_SIZE] == '0')
			&& (!there_is_sprite(data, sprite, x, y + deplacement + marge) || sprite->attack_x != -1000 || sprite->attack_y != -1000));
	else
		return (is_in_map(data, x / CUBE_SIZE, (y + deplacement - marge) / CUBE_SIZE)
			&& (data->vars.map[(y + deplacement - marge) / CUBE_SIZE][x / CUBE_SIZE] == '0')
			&& (!there_is_sprite(data, sprite, x, y + deplacement - marge) || sprite->attack_x != -1000 || sprite->attack_y != -1000));
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void	move_sprite(t_data *data, t_sprite *sprite, double scale, t_img **texture)
{
	if (sprite->status != 2 && sprite->life > 0 && data->player.life > 0 && (sprite->attack_x == -1000 && sprite->attack_y == -1000)
		&& distance(data->player.x, data->player.y, sprite->x, sprite->y) < CUBE_SIZE * 25)
	{
		if (is_free_x(data, sprite, -cos(sprite->angle) * sprite->speed * data->fps, ((*texture)->width) * scale))
			sprite->x -= cos(sprite->angle) * (sprite->speed * (data->fps));		
		if (is_free_y(data, sprite, -sin(sprite->angle) * sprite->speed * data->fps, ((*texture)->width) * scale))
			sprite->y += -sin(sprite->angle) * (sprite->speed * (data->fps));
 		if (distance(data->player.x, data->player.y, sprite->x, sprite->y) < (CUBE_SIZE * 0.8))
 			*texture = &sprite->text->attack_text[0];
	}
}

int	sprite_is_facing_left(t_data *data, t_sprite *sprite)
{
	int		distx;
	int		disty;

	distx = data->player.x - sprite->x;
	if (distx < 0)
		distx = -distx;
	disty = data->player.y - sprite->y;
	if (disty < 0)
		disty = -disty;
	if ((sprite->life <= 0 && sprite->dead_position == 1) || (sprite->life > 0 && (((distx < disty) && ((data->player.y < sprite->y && sprite->angle > M_PI / 2) || (data->player.y > sprite->y && sprite->angle > -(M_PI / 2))))
			|| ((distx > disty) && ((data->player.x < sprite->x && sprite->angle > 0) || (data->player.x > sprite->x && sprite->angle < 0))))))
		return (1);
	return (0);
}

void	draw_sprite(t_data *data, t_sprite *sprite, int x1, int y1, double scale)//, int dist)
{
	int		x;
	int		y;
	int		color;
	
	t_img	*texture;

	sprite->angle = atan2(sprite->y - data->player.y, sprite->x - data->player.x);
	texture = get_sprite_texture(&data->player, sprite, sprite->angle);
	move_sprite(data, sprite, scale, &texture);

	double	text_x;
	double	text_y;
	int		tmp = -((texture->width * scale) / 2);
	
	int		end_x;
	
	x = x1 - ((texture->width * scale) / 2);
	end_x = x1 + ((texture->width * scale) / 2);
	if (DEBUG >= 2)
	{
		if (x1 >= 0 && y1>=0 && x1 < SCREEN_WIDTH && y1 < SCREEN_HEIGHT)
			draw_square(data, x1, y1, 10);
		if (DEBUG >= 3)
			return ;
	}
	while (x < end_x && x < SCREEN_WIDTH)
	{
		y = y1 - (texture->height * (scale));
		if (y < 0)
			y = 0;
		while (y < y1 && y < SCREEN_HEIGHT - 1)
		{
			if (distance(data->player.x - data->player.dx, data->player.y + data->player.dy, sprite->x, sprite->y) < distance(data->player.x, data->player.y, sprite->x, sprite->y)
				&& x1 + tmp >= 0 && x1 + tmp < SCREEN_WIDTH
				&& (distance(data->player.x, data->player.y, sprite->x, sprite->y) * cos(fix_angle(data->player.dir - data->ray_save[x1 + tmp].Angle)) <= data->ray_save[x1 + tmp].length)
				&& x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
			{
				
				text_x = (double)(end_x - 1 - x) / (double)((texture->width) * (scale));
				text_y = (double)(y1 - 1 - y) / (double)((texture->height) * (scale));
				if (text_x < 0 || text_x > 1.0 || text_y < 0 || text_y > 1.0)//A VIRER
					printf("x: %f, y: %f\n", text_x, text_y);
				if (sprite_is_facing_left(data, sprite))
					color = texture->imgptr[(int)((texture->height - 1) - (int)((texture->height - 1) * text_y)) * (texture->sizeLine / 4) + ((texture->width - 1) - (int)(((texture->width) - 1) * text_x))];
				else
					color = texture->imgptr[(int)((texture->height - 1) * (1 - text_y)) * (texture->sizeLine / 4) + ((int)(((texture->width - 1)) * text_x))];
				if (color != create_trgb(255, 0, 0, 0))
					data->frame.imgptr[y * data->frame.sizeLine / 4 + x] = shadow_color(color, distance(data->player.x, data->player.y, sprite->x, sprite->y));
			}
			y++;
		}
		tmp++;
		x++;
	}
	if (sprite->last_frame != -1 && get_timestamp() - sprite->last_frame > 80)
	{
		sprite->count++;
		sprite->last_frame = get_timestamp();
	}
	if (sprite->count == 6 && sprite->life > 0)
		sprite->count = 0;
	else if (sprite->life <= 0 && sprite->attack_x == -1000 && sprite->attack_y == -1000 && sprite->count == 100)
		sprite->status = -1;
	if (sprite->last_frame == -1)
		sprite->last_frame = get_timestamp();
}

void	attack_sprite(t_data *data, t_list *list)
{
	t_list	*curr;
	t_list	*tmp;
	t_sprite	*sprite;

	tmp = list;
	while (tmp)
	{
		curr = tmp;
		tmp = tmp->next;
		sprite = &curr->sprite;
		if ((sprite->attack_x != -1000 || sprite->attack_y != -1000 || sprite->status == 2) && (sprite->last_frame == -1 || get_timestamp() - sprite->last_frame > 10))
		{
			if ((sprite->attack_x != -1000 || sprite->attack_y != -1000))
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
				if (is_free_x(data, sprite, -sprite->attack_x * (CUBE_SIZE * 0.08), sprite->text->text[0].width / 2))
						sprite->x -= sprite->attack_x * (CUBE_SIZE * 0.08);

				if (is_free_y(data, sprite, sprite->attack_y * (CUBE_SIZE * 0.08), sprite->text->text[0].width / 2))
					sprite->y += sprite->attack_y * (CUBE_SIZE * 0.08);
				sprite->calls++;
				if (sprite->calls == 20)
				{
					sprite->calls = 0;
					sprite->count = 0;
					sprite->power = 9;
					sprite->attack_x = -1000;
					sprite->attack_y = -1000;
				}
			}
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
}

void	free_dead_sprites(t_list **list)
{
	t_list	*curr;
	t_list	*tmp;

	curr = *list;
	while (*list && (*list)->sprite.status == -1)
	{
		tmp = *list;
		(*list) = (*list)->next;
		free(tmp);
		curr = *list;
	}
	while (curr && curr->next)
	{
		if (curr->next->sprite.status == -1)
		{
			tmp = curr->next;
			curr->next = curr->next->next;
			free(tmp);
		}
		curr = curr->next;
	}
}

void	sort_list(t_data *data, t_list **list)
{
	t_list		*curr;
	t_sprite	tmp;

	curr = *list;
	while (curr && curr->next)
	{
		if (distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) < distance(data->player.x, data->player.y, curr->next->sprite.x, curr->next->sprite.y))
		{
			tmp = curr->sprite;
			curr->sprite = curr->next->sprite;
			curr->next->sprite = tmp;
		}
		curr = curr->next;
	}
}

void	draw_sprites(t_data *data)
{
	t_list *curr;
	t_list *tmp;

	sort_list(data, &data->sprites);
	tmp = data->sprites;
	while (tmp)
	{
		curr = tmp;
		tmp = tmp->next;
		float sx=curr->sprite.x-data->player.x; //temp float variables
		float sy=curr->sprite.y-data->player.y;
		float sz=(curr->sprite.z * 2) * data->camera_height;	

		float a=sy*(data->player.dx)+sx*(data->player.dy); 
		float b=sx*(data->player.dx)-sy*(data->player.dy); 
		sx=a; sy=b;	

		sx=sx*((CUBE_SIZE * 0.80)/sy)+(SCREEN_WIDTH/2); //convert to screen x,y
		sy=sz*((CUBE_SIZE)/sy)+(data->horizon_line);

		if (data->player.attack && distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) < CUBE_SIZE && sx > SCREEN_WIDTH * 0.3 && sx < SCREEN_WIDTH * 0.7
			&& curr->sprite.life > 0)
		{
			data->player.attack = 0;
			double	tmpangle = atan2(curr->sprite.y - data->player.y, curr->sprite.x - data->player.x);
			if (data->player.in_jump && data->player.attack_dir_x == 0 && data->player.attack_dir_y == 0)
			{
				if (curr->sprite.life - 2 <= 0 && sprite_is_facing_left(data, &curr->sprite))
					curr->sprite.dead_position = 1;
				curr->sprite.life -= 2;
				curr->sprite.attack_x = -cos(tmpangle) * 2;
				curr->sprite.attack_y = sin(tmpangle) * 2;
				data->player.mana += 5;
			}
			else
			{
				if (curr->sprite.life - 1 <= 0 && sprite_is_facing_left(data, &curr->sprite))
					curr->sprite.dead_position = 1;
				curr->sprite.life--;
				curr->sprite.attack_x = -cos(tmpangle);
				curr->sprite.attack_y = sin(tmpangle);
				data->player.mana += 2;
			}
			if (data->player.mana > 100)
				data->player.mana = 100;
			curr->sprite.calls = 0;
			curr->sprite.status = 1;
			attack_sprite(data, curr);
			return ;
		}

		double	tmpAngle = data->player.dir + ((sx - SCREEN_WIDTH / 2) * (DEGREE / (SCREEN_WIDTH / FOV)));
		if (tmpAngle < data->player.dir - ((SCREEN_WIDTH / 2 + 10) * (DEGREE / (SCREEN_WIDTH / FOV))))
			tmpAngle = data->player.dir - ((SCREEN_WIDTH / 2 + 10) * (DEGREE / (SCREEN_WIDTH / FOV)));
		else if (tmpAngle > data->player.dir + ((SCREEN_WIDTH / 2 + 10) * (DEGREE / (SCREEN_WIDTH / FOV))))
			tmpAngle = data->player.dir + ((SCREEN_WIDTH / 2 + 10) * (DEGREE / (SCREEN_WIDTH / FOV)));
		//if (distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) < CUBE_SIZE * 15)
			//&& distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) > CUBE_SIZE * 0.1) pour le lag piste a explorer
		{
			//printf("orginal len: %f, then %f\n", distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y), (distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) * cos(fix_angle(data->player.dir + ((sx - SCREEN_WIDTH / 2) * DEGREE)))));
		//	if (sx>0 && sx < SCREEN_WIDTH)
		//		printf("true: %f, false: %f\n", cos(data->player.dir + ((sx - SCREEN_WIDTH / 2) * (DEGREE / (SCREEN_WIDTH / FOV)))));
			if (curr->sprite.life <= 0)
				draw_sprite(data, &curr->sprite, sx, sy, (SCREEN_WIDTH * 2) / (distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) * cos(fix_angle(data->player.dir - tmpAngle))));
			else
				draw_sprite(data, &curr->sprite, sx, sy, (SCREEN_WIDTH * 2) / (distance(data->player.x, data->player.y, curr->sprite.x, curr->sprite.y) * cos(fix_angle(data->player.dir - tmpAngle))));
		}
	}
	free_dead_sprites(&data->sprites);
}
