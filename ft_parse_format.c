/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_format.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/* Every conversion starts from a blank sheet. */
void	reset_format(t_format *spec)
{
	spec->left_align = 0;
	spec->zero_pad = 0;
	spec->show_plus = 0;
	spec->show_space = 0;
	spec->alt_form = 0;
	spec->min_width = 0;
	spec->has_precision = 0;
	spec->precision = 0;
	spec->conversion = 0;
}

/* Reads one flag character, records it, steps the cursor.
   Returns 1 if it actually was a flag, 0 otherwise.   */
static int	consume_flag(const char **cursor, t_format *spec)
{
	if (**cursor == '-')
		spec->left_align = 1;
	else if (**cursor == '0')
		spec->zero_pad = 1;
	else if (**cursor == '+')
		spec->show_plus = 1;
	else if (**cursor == ' ')
		spec->show_space = 1;
	else if (**cursor == '#')
		spec->alt_form = 1;
	else
		return (0);
	(*cursor)++;
	return (1);
}

/* "123" -> 123. Digits may be split by a '.' : read 12 from "12.3". */
static int	read_number(const char **cursor)
{
	int	number;

	number = 0;
	while (**cursor >= '0' && **cursor <= '9')
	{
		number = number * 10 + (**cursor - '0');
		(*cursor)++;
	}
	return (number);
}

/*
** grammar: % [flags] [width] [.precision] conversion
** leaves the cursor ON the conversion character (the caller steps past it)
*/
void	parse_format(const char **cursor, t_format *spec)
{
	reset_format(spec);
	while (consume_flag(cursor, spec))
		;
	if (**cursor >= '0' && **cursor <= '9')
		spec->min_width = read_number(cursor);
	if (**cursor == '.')
	{
		(*cursor)++;
		spec->has_precision = 1;
		spec->precision = read_number(cursor);
	}
	spec->conversion = **cursor;
}
