#include "ft_printf.h"

static char	base_digit(unsigned long digit_value, int uppercase)
{
	if (digit_value < 10)
		return ((char)('0' + digit_value));
	if (uppercase)
		return ((char)('A' + digit_value - 10));
	return ((char)('a' + digit_value - 10));
}

char	*digits_in_base(unsigned long value, int base, int uppercase)
{
	char	reversed[70];
	char	*result;
	int		length;
	int		i;

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
	i = 0;
	while (i < length)
	{
		result[i] = reversed[length - 1 - i];
		i++;
	}
	result[length] = '\0';
	return (result);
}
