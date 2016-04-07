/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/25 18:02:10 by rabougue          #+#    #+#             */
/*   Updated: 2016/04/07 18:22:28 by cattouma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	ft_pixel_put_to_image(t_pixel_to_image *img, t_point *p)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = (img->img_color & 0xFF0000) >> 16;
	g = (img->img_color & 0xFF00) >> 8;
	b = (img->img_color & 0xFF);
	if (img->endian == 0)
	{
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 2] = r;
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 1] = g;
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 0] = b;
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 3] = 0x00;
	}
	else
	{
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 2] = b;
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 1] = g;
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 0] = r;
		img->data[p->y * img->sizeline + p->x * img->bpp / 8 + 3] = 0x00;
	}
}

void	first_case(t_bres *b, t_pixel_to_image *img, t_point *p1)
{
	int i;

	i = 0;
	while (i <= b->c_dx)
	{
		ft_pixel_put_to_image(img, p1);
		i++;
		p1->x += b->x_incr;
		b->ex -= b->dy;
		if (b->ex < 0)
		{
			p1->y += b->y_incr;
			b->ex += b->dx;
		}
	}
}

void	second_case(t_bres *b, t_pixel_to_image *img, t_point *p1)
{
	int i;

	i = 0;
	while (i <= b->c_dy)
	{
		ft_pixel_put_to_image(img, p1);
		i++;
		p1->y += b->y_incr;
		b->ey -= b->dx;
		if (b->ey < 0)
		{
			p1->x += b->x_incr;;
			b->ey += b->dy;
		}
	}
}

void    draw_line(t_pixel_to_image *img, t_point *p1, t_point *p2)
{
	t_bres	b;

	b.ex = abs(p2->x - p1->x);
	b.ey = abs(p2->y - p1->y);
	b.dx = b.ex * 2;
	b.dy = b.ey * 2;
	b.c_dx = b.ex;
	b.c_dy = b.ey;
	b.x_incr = 1;
	b.y_incr = 1;

	if (p1->x > p2->x)
		b.x_incr = -1;
	if (p1->y > p2->y)
		b.y_incr = -1;
	if (b.c_dx > b.c_dy)
		first_case(&b, img, p1);
	if (b.c_dx < b.c_dy)
		second_case(&b, img, p1);
}

void	draw_x(t_coord *coord, t_pixel_to_image *img, int gap)
{
	int x;
	int y;
	t_point	p1;
	t_point	p2;

	x = 0;
	y = 0;
	p1.x = ((x * gap) - (y * gap)) + ORIGIN_X - gap;
	p1.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y + gap / 2;
	while (y < coord->y_point)
	{
		while (x < coord->x_point)
		{
			p2.x = ((x * gap) - (y * gap)) + ORIGIN_X;
			p2.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y - (coord->map[y][x] * 10);
			draw_line(img, &p1, &p2);
			x++;
			p1.x = p2.x; 
			p1.y = p2.y; 
		}
		x = 0;
		p1.x = ((x * gap) - (y * gap)) + ORIGIN_X - gap;
		if ((y + 1) < coord->y_point)
			p1.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y + gap / 2 - (coord->map[y + 1][x] * 10);
		else
			p1.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y + gap / 2 ;
		y++;
	}
}

void	draw_y(t_coord *coord, t_pixel_to_image *img, int gap)
{
	int x;
	int y;
	t_point	p1;
	t_point	p2;

	x = 0;
	y = 0;
	p1.x = ((x * gap) - (y * gap)) + ORIGIN_X - gap;
	p1.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y + gap / 2;
	while(x < coord->x_point)
	{
		while (y < coord->y_point)
		{
			p2.x = ((x * gap) - (y * gap)) + ORIGIN_X;
			p2.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y - (coord->map[y][x] * 10);
			draw_line(img, &p1, &p2);
			y++;
			p1.x = p2.x; 
			p1.y = p2.y; 
		}
		y = 0;
		x++;
		p1.x = ((x * gap) - (y * gap)) + ORIGIN_X - gap;
		if ((y + 1) < coord->y_point)
			p1.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y + gap / 2 - (coord->map[y + 1][x] * 10);
		else
			p1.y = ((x * gap) + (y * gap)) / 2 + ORIGIN_Y + gap / 2 ;
	}
}

void	print_point(t_coord *coord, t_pixel_to_image *img)
{
	int gap;

	gap = 60;
	draw_x(coord, img, gap);
	draw_y(coord, img, gap);
}


/* point_iso.x = ((point.x* zoom) - (point.y * zoom)) + ORIGIN_X; */
/* point_iso.y = (((point.x* zoom) + (point.y * zoom)) / 2) + ORIGIN_Y  - (ft_atoi(split_x[point.x]) * 10); */
/* p2.x = ((point.x + 1) * zoom) - (point.y * zoom) + ORIGIN_X; */
/* p2.y = (((point.x + 1) * zoom) + (point.y * zoom) / 2) + ORIGIN_Y - (ft_atoi(split_x[point.x + 1]) * 10); */
