/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 01:24:52 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 01:26:49 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_list	*listlast(t_list *list)
{
	t_list	*curr;

	curr = list;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

void	push_back(t_list **list, t_sprite sprite)
{
	t_list	*new_cell;

	new_cell = (t_list *)malloc(sizeof(t_list));
	if (!new_cell)
		return ;
	new_cell->next = NULL;
	new_cell->sprite = sprite;
	if (!*list)
		*list = new_cell;
	else
		listlast(*list)->next = new_cell;
}

void	list_clear(t_list **list)
{
	t_list	*tmp;

	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
	}
}
