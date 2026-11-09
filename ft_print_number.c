#include "ft_printf.h"

int	print_unsigned(unsigned long value, int base, int uppercase)
{
	char	*digits;
	int		count;

	digits = digits_in_base(value, base, uppercase);
	if (!digits)
		return (0);
	count = emit_text(digits, ft_strlen(digits));
	free(digits);
	return (count);
}

int	print_signed(long value)
{
	unsigned long	magnitude;
	int				count;

	count = 0;
	if (value < 0)
	{
		count += emit_text("-", 1);
		magnitude = (unsigned long)(-(value + 1)) + 1UL;
	}
	else
		magnitude = (unsigned long)value;
	return (count + print_unsigned(magnitude, 10, 0));
}

int	print_ptr(void *pointer)
{
	char	prefix[2];
	int		count;

	if (!pointer)
		return (emit_text("(nil)", 5));
	prefix[0] = '0';
	prefix[1] = 'x';
	count = emit_text(prefix, 2);
	return (count + print_unsigned((unsigned long)pointer, 16, 0));
}
