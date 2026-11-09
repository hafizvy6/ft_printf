/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_number.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/* The sign is the prefix of a signed number: "-", "+", " " or nothing.
   '+' wins over ' ' when both are given.                             */
static char	*make_sign_prefix(long value, t_format *spec)
{
	char	*prefix;

	prefix = malloc(2);
	if (!prefix)
		return (NULL);
	prefix[0] = '\0';
	prefix[1] = '\0';
	if (value < 0)
		prefix[0] = '-';
	else if (spec->show_plus)
		prefix[0] = '+';
	else if (spec->show_space)
		prefix[0] = ' ';
	return (prefix);
}

/* "0x" / "0X" for hex only, and never for zero: "%#x" of 0 is just "0". */
static char	*make_hex_prefix(t_format *spec, unsigned long value, int base,
		int uppercase)
{
	char	*prefix;

	prefix = malloc(3);
	if (!prefix)
		return (NULL);
	prefix[0] = '\0';
	prefix[1] = '\0';
	prefix[2] = '\0';
	if (spec->alt_form && base == 16 && value != 0)
	{
		prefix[0] = '0';
		prefix[1] = 'x';
		if (uppercase)
			prefix[1] = 'X';
	}
	return (prefix);
}

/* Digits, then the precision is applied to them: "%.0d" of 0 has no digits
   at all, and "%.5d" of 42 is "00042". Width is NOT applied here.        */
char	*render_digits(unsigned long value, t_format *spec, int base,
		int uppercase)
{
	char	*digits;
	int		length;

	digits = digits_in_base(value, base, uppercase);
	if (!digits)
		return (NULL);
	if (spec->has_precision && spec->precision == 0 && value == 0)
		digits[0] = '\0';
	length = ft_strlen(digits);
	if (spec->has_precision && spec->precision > length)
		digits = pad_digits_with_zeros(digits, length, spec->precision);
	return (digits);
}

/* %d and %i.
   INT_MIN cannot be negated as an int, so the magnitude is built as an
   unsigned long: -(value + 1) + 1 is the same number, without overflow. */
int	print_signed(t_format *spec, long value)
{
	unsigned long	magnitude;

	if (spec->has_precision)
		spec->zero_pad = 0;
	if (value < 0)
		magnitude = (unsigned long)(-(value + 1)) + 1UL;
	else
		magnitude = (unsigned long)value;
	return (emit_and_free(spec, make_sign_prefix(value, spec),
			render_digits(magnitude, spec, 10, 0)));
}

/* %u, %x, %X: the same renderer, another base. A precision cancels '0'. */
int	print_unsigned(t_format *spec, unsigned long value, int base,
		int uppercase)
{
	if (spec->has_precision)
		spec->zero_pad = 0;
	return (emit_and_free(spec, make_hex_prefix(spec, value, base, uppercase),
			render_digits(value, spec, base, uppercase)));
}
