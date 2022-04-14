/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 19:43:00 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 02:05:05 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	free_double_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

int	ft_putstr_error(char *str)
{
	int	i;

	if (!str || !*str)
		return (1);
	i = -1;
	while (str[++i])
		if (write(2, &str[i], 1) == -1)
			return (1);
	return (1);
}

char	*ft_join(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		return (s1);
	new = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
	i = 0;
	if (s1)
	{
		new[i] = s1[i];
		while (s1[++i])
			new[i] = s1[i];
	}
	j = 0;
	while (s2[j])
		new[i++] = s2[j++];
	new[i] = '\0';
	return (new);
}

static int	ft_get_size(int n)
{
	unsigned int	x;
	int				size;

	size = 1;
	if (n < 0)
	{
		x = -n;
		size++;
	}
	else
		x = n;
	while (x > 9)
	{
		x /= 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char			*res;
	unsigned int	x;
	int				size;
	int				i;

	size = ft_get_size(n);
	res = (char *)malloc(size + 1);
	if (!res)
		return (NULL);
	i = 0;
	if (n < 0)
		x = -n;
	else
		x = n;
	res[size--] = '\0';
	if (n < 0)
		res[i++] = '-';
	while (i <= size)
	{
		res[size--] = (x % 10) + '0';
		x /= 10;
	}
	return (res);
}
