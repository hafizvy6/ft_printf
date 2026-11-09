/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_text.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** %s, %c and (nil) have no prefix and no zero padding: width and '-' are
** the only flags that mean anything for text. That is why this is not
** emit_with_padding.
*/
int	emit_text_with_padding(t_format *spec, char *text, int length)
{
	int	padding;

	padding = spec->min_width - length;
	if (padding < 0)
		padding = 0;
	if (!spec->left_align)
		emit_padding(padding, ' ');
	emit_text(text, length);
	if (spec->left_align)
		emit_padding(padding, ' ');
	return (length + padding);
}

/* %c : the argument arrives as an int (types smaller than int are promoted
   when passed through "..."), so we cut it back to a char here.           */
int	print_char(t_format *spec, int character)
{
	char	letter;

	letter = (char)character;
	return (emit_text_with_padding(spec, &letter, 1));
}

/*
** %s : NULL is not a crash. glibc prints "(null)", BUT with a precision
** smaller than 6 it prints nothing at all ("%.2s" of NULL is empty), while
** the width still pads. Measured on glibc 2.41, not guessed.
** Precision otherwise = maximum number of characters.
*/
int	print_str(t_format *spec, char *text)
{
	int	length;

	if (!text)
	{
		if (spec->has_precision && spec->precision < 6)
			text = "";
		else
			text = "(null)";
	}
	length = ft_strlen(text);
	if (spec->has_precision && spec->precision < length)
		length = spec->precision;
	return (emit_text_with_padding(spec, text, length));
}
