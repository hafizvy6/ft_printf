#include "ft_printf.h"

int	ft_strlen(const char *text)
{
	int	length;

	length = 0;
	while (text && text[length])
		length++;
	return (length);
}

int	emit_text(const char *text, int length)
{
	if (length > 0)
		write(1, text, (size_t)length);
	return (length);
}
