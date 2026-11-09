#include "ft_printf.h"

int	print_char(int character)
{
	char	letter;

	letter = (char)character;
	return (emit_text(&letter, 1));
}

int	print_str(char *text)
{
	if (!text)
		text = "(null)";
	return (emit_text(text, ft_strlen(text)));
}
