/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** The conversion character decides: which C type to fetch from the va_list,
** and which printer to call. That is the whole dispatcher.
*/
int	dispatch_conversion(t_format *spec, va_list *args)
{
	char	unknown;

	if (spec->conversion == 'c')
		return (print_char(spec, va_arg(*args, int)));
	if (spec->conversion == 's')
		return (print_str(spec, va_arg(*args, char *)));
	if (spec->conversion == 'p')
		return (print_ptr(spec, va_arg(*args, void *)));
	if (spec->conversion == 'd' || spec->conversion == 'i')
		return (print_signed(spec, (long)va_arg(*args, int)));
	if (spec->conversion == 'u')
		return (print_unsigned(spec, va_arg(*args, unsigned int), 10, 0));
	if (spec->conversion == 'x')
		return (print_unsigned(spec, va_arg(*args, unsigned int), 16, 0));
	if (spec->conversion == 'X')
		return (print_unsigned(spec, va_arg(*args, unsigned int), 16, 1));
	if (spec->conversion == '%')
		return (emit_text("%", 1));
	unknown = (char)spec->conversion;
	return (emit_text(&unknown, 1));
}

/*
** Walk the format string.
**   normal character      -> copy it, count 1
**   '%' + something       -> reset the spec, parse it, dispatch
**   '%' at the very end   -> nothing follows, so print it literally
*/
int	ft_printf(const char *format, ...)
{
	va_list		args;
	t_format	spec;
	int			count;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			parse_format(&format, &spec);
			count += dispatch_conversion(&spec, &args);
			if (*format)
				format++;
		}
		else
			count += emit_text(format++, 1);
	}
	va_end(args);
	return (count);
}
