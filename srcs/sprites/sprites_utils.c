/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 03:59:41 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:53:10 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
	if ((sprite->life <= 0 && sprite->dead_position == 1) || (sprite->life > 0
			&& (((distx < disty) && ((data->player.y < sprite->y
							&& sprite->angle > M_PI / 2)
						|| (data->player.y > sprite->y
							&& sprite->angle > -(M_PI / 2))))
				|| ((distx > disty)
					&& ((data->player.x < sprite->x && sprite->angle > 0)
						|| (data->player.x > sprite->x
							&& sprite->angle < 0))))))
		return (1);
	return (0);
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
		if (distance(data->player.x, data->player.y,
				curr->sprite.x, curr->sprite.y) < distance(data->player.x,
				data->player.y, curr->next->sprite.x, curr->next->sprite.y))
		{
			tmp = curr->sprite;
			curr->sprite = curr->next->sprite;
			curr->next->sprite = tmp;
		}
		curr = curr->next;
	}
}

double	calculate_sprite_angle(t_data *data, t_sprite *sprite)
{
	double	tmpangle;

	tmpangle = data->player.dir + ((sprite->x_on_screen - SCREEN_WIDTH / 2)
			* (DEGREE / (SCREEN_WIDTH / FOV)));
	if (tmpangle < data->player.dir - ((SCREEN_WIDTH / 2 + 10)
			* (DEGREE / (SCREEN_WIDTH / FOV))))
		tmpangle = data->player.dir - ((SCREEN_WIDTH / 2 + 10)
				* (DEGREE / (SCREEN_WIDTH / FOV)));
	else if (tmpangle > data->player.dir + ((SCREEN_WIDTH / 2 + 10)
			* (DEGREE / (SCREEN_WIDTH / FOV))))
		tmpangle = data->player.dir + ((SCREEN_WIDTH / 2 + 10)
				* (DEGREE / (SCREEN_WIDTH / FOV)));
	return (tmpangle);
}
