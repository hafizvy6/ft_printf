/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

/*
** One parsed conversion. "% +08.5d" becomes:
**   show_plus = 1, show_space = 0, zero_pad = 1, left_align = 0,
**   min_width = 8, has_precision = 1, precision = 5, conversion = 'd'
** -  '-'  left_align   : pad on the right, with spaces
** 0  '0'  zero_pad     : pad on the left, with zeros (ignored if a precision
**                        is given, or if left_align is set)
** +  '+'  show_plus    : sign for positive numbers too
** ( ) ' ' show_space   : a space instead of '+' for positive numbers
** #  '#'  alt_form     : 0x / 0X in front of hex numbers (never for base 10)
** .  '.'  has_precision: the '.' was there (so precision 0 is meaningful)
*/
typedef struct s_format
{
	int	left_align;
	int	zero_pad;
	int	show_plus;
	int	show_space;
	int	alt_form;
	int	min_width;
	int	has_precision;
	int	precision;
	int	conversion;
}	t_format;

int		ft_printf(const char *format, ...);
int		ft_strlen(const char *text);
int		emit_text(const char *text, int length);
int		emit_padding(int amount, char fill);
void	reset_format(t_format *spec);
void	parse_format(const char **cursor, t_format *spec);
int		emit_with_padding(t_format *spec, char *prefix, char *payload,
			int payload_len);
int		emit_and_free(t_format *spec, char *prefix, char *payload);
int		emit_text_with_padding(t_format *spec, char *text, int length);
int		dispatch_conversion(t_format *spec, va_list *args);
int		print_char(t_format *spec, int character);
int		print_str(t_format *spec, char *text);
int		print_ptr(t_format *spec, void *pointer);
int		print_signed(t_format *spec, long value);
int		print_unsigned(t_format *spec, unsigned long value, int base,
			int uppercase);
char	*render_digits(unsigned long value, t_format *spec, int base,
			int uppercase);
char	*digits_in_base(unsigned long value, int base, int uppercase);
char	*pad_digits_with_zeros(char *digits, int length, int target_length);

#endif
