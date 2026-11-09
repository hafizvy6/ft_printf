/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_render_digits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/* 0..9 -> '0'..'9', 10..15 -> 'a'..'f' or 'A'..'F'. */
static char	base_digit(unsigned long digit_value, int uppercase)
{
	if (digit_value < 10)
		return ((char)('0' + digit_value));
	if (uppercase)
		return ((char)('A' + digit_value - 10));
	return ((char)('a' + digit_value - 10));
}

static void	copy_forward(char *destination, char *source, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		destination[i] = source[i];
		i++;
	}
}

/* digits_in_base() collects the digits backwards; this flips them. */
static void	fill_reversed(char *destination, char *reversed, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		destination[i] = reversed[length - 1 - i];
		i++;
	}
}

/*
** 255 in base 16: 255 % 16 = 15 -> 'f', 255 / 16 = 15 -> 'f', 15 / 16 = 0.
** We got 'f','f','2' - the number backwards - so it is reversed at the end.
** A value of 0 still produces one digit: "0".
*/
char	*digits_in_base(unsigned long value, int base, int uppercase)
{
	char	reversed[70];
	char	*result;
	int		length;

	length = 0;
	reversed[length++] = base_digit(value % (unsigned long)base, uppercase);
	value /= (unsigned long)base;
	while (value)
	{
		reversed[length++] = base_digit(value % (unsigned long)base, uppercase);
		value /= (unsigned long)base;
	}
	result = malloc((size_t)length + 1);
	if (!result)
		return (NULL);
	fill_reversed(result, reversed, length);
	result[length] = '\0';
	return (result);
}

/* "42" with target 5 -> "00042". Frees the old string, returns the new one. */
char	*pad_digits_with_zeros(char *digits, int length, int target_length)
{
	char	*result;
	int		zeros;

	result = malloc((size_t)target_length + 1);
	if (!result)
	{
		free(digits);
		return (NULL);
	}
	result[target_length] = '\0';
	zeros = 0;
	while (zeros < target_length - length)
	{
		result[zeros] = '0';
		zeros++;
	}
	copy_forward(result + zeros, digits, length);
	free(digits);
	return (result);
}
