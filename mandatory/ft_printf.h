#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
int		ft_printf(const char *format, ...);
int		ft_strlen(const char *text);
int		emit_text(const char *text, int length);
int		print_char(int character);
int		print_str(char *text);
int		print_signed(long value);
int		print_unsigned(unsigned long value, int base, int uppercase);
int		print_ptr(void *pointer);
char	*digits_in_base(unsigned long value, int base, int uppercase);
#endif
