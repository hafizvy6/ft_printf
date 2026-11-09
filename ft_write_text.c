/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_text.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strlen(const char *text)
{
	int	length;

	length = 0;
	while (text && text[length])
		length++;
	return (length);
}

/* The single funnel: every character that reaches the screen passes here.
   It returns what it wrote, so the caller can add it to the total.          */
int	emit_text(const char *text, int length)
{
	if (length > 0)
		write(1, text, (size_t)length);
	return (length);
}

/* Write "amount" copies of "fill" in blocks of 64, so no huge buffer is
   needed and no malloc is involved.                                        */
int	emit_padding(int amount, char fill)
{
	char	chunk[64];
	int		written;
	int		i;

	written = 0;
	while (written < amount)
	{
		i = 0;
		while (i < 64 && written + i < amount)
		{
			chunk[i] = fill;
			i++;
		}
		write(1, chunk, (size_t)i);
		written += i;
	}
	return (amount);
}
