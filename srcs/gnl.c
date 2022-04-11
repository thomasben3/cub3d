/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 00:56:11 by tbensem           #+#    #+#             */
/*   Updated: 2022/03/25 18:13:57 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	ft_strlen(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup_gnl(char *str)
{
	char	*new;
	int		i;

	new = (char *)malloc(ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	free(str);
	return (new);
}

char	*ft_join_gnl(char *s1, char s2[2])
{
	char	*new;
	int		i;
	int		j;

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
	if (s1)
		free(s1);
	return (new);
}

int	has_nl(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] == '\n')
			return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	char	*line;
	char	buffer[2];
	int		ret;

	if (fd < 0)
		return (NULL);
	ret = 1;
	line = NULL;
	while (!has_nl(line) && ret != 0)
	{
		ret = read(fd, buffer, 1);
		if (ret == -1)
			return (NULL);
		buffer[ret] = '\0';
		line = ft_join_gnl(line, buffer);
	}
	return (line);
}
