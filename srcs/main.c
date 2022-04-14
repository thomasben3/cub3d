/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbensem <tbensem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:08:45 by tbensem           #+#    #+#             */
/*   Updated: 2022/04/14 03:50:05 by tbensem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <sys/time.h>
#include <time.h>

long	get_timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

int	is_point_cub(char *str)
{
	int	end;

	end = ft_strlen(str) - 1;
	return (str[end - 3] == '.' && str[end - 2] == 'c'
		&& str[end - 1] == 'u' && str[end] == 'b');
}

int	is_in_map(t_data *data, int x, int y)
{
	return (x >= 0 && y >= 0
		&& x < data->vars.map_width && y < data->vars.map_height);
}

int	init_all(t_data *d, char **argv)
{
	d->pause = -1;
	d->vars.win
		= mlx_new_window(d->vars.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "CUB3D");
	if (parse_all(d, argv[1], -1))
	{
		mlx_destroy_window(d->vars.mlx, d->vars.win);
		free(d->vars.mlx);
		return (1);
	}
	srand(time(NULL));
	load_img(d, &d->door_text, "assets/door.xpm");
	init_hands(d);
	load_chopper_imgs(d, &d->sprite_text[CHOPPER]);
	load_luffy_imgs(d, &d->sprite_text[LUFFY]);
	load_teach_imgs(d, &d->sprite_text[TEACH]);
	d->camera_height = 0.5;
	d->horizon_line = SCREEN_HEIGHT / 2;
	init_keys(&d->keys);
	d->frame.img = mlx_new_image(d->vars.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	d->frame.imgptr = (int *)mlx_get_data_addr(d->frame.img,
			&d->frame.bit_per_px, &d->frame.size_line, &d->frame.endian);
	d->last_frame_time = get_timestamp();
	d->fps = 50;
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (ft_putstr_error("Error\nbad argument.\n"));
	else if (!is_point_cub(argv[1]))
		return (ft_putstr_error("Error\nmap description must end by '.cub'.\n"));
	data.vars.config_file = argv[1];
	data.vars.mlx = mlx_init();
	if (!data.vars.mlx || init_all(&data, argv))
		return (1);
	make_frame(&data);
	mlx_hook(data.vars.win, 02, 1L << 0, key_press, &data);
	mlx_hook(data.vars.win, 03, 1L << 1, key_release, &data.keys);
	mlx_loop_hook(data.vars.mlx, render_next_frame, &data);
	mlx_hook(data.vars.win, 17, 0L, ft_exit, &data);
	mlx_mouse_hide(data.vars.mlx, data.vars.win);
	mlx_hook(data.vars.win, 06, 1L << 6, mouse_move, &data);
	mlx_mouse_hook(data.vars.win, mouse_click, &data);
	mlx_loop(data.vars.mlx);
	return (0);
}
