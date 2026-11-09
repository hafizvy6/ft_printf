#include "ft_printf.h"

static int	dispatch_conversion(char conversion, va_list *args)
{
	if (conversion == 'c')
		return (print_char(va_arg(*args, int)));
	if (conversion == 's')
		return (print_str(va_arg(*args, char *)));
	if (conversion == 'p')
		return (print_ptr(va_arg(*args, void *)));
	if (conversion == 'd' || conversion == 'i')
		return (print_signed((long)va_arg(*args, int)));
	if (conversion == 'u')
		return (print_unsigned(va_arg(*args, unsigned int), 10, 0));
	if (conversion == 'x')
		return (print_unsigned(va_arg(*args, unsigned int), 16, 0));
	if (conversion == 'X')
		return (print_unsigned(va_arg(*args, unsigned int), 16, 1));
	return (emit_text("%", 1));
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%' && *(format + 1))
			count += dispatch_conversion(*++format, &args);
		else
			count += emit_text(format, 1);
		format++;
	}
	va_end(args);
	return (count);
}
