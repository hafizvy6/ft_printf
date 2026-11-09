/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** %p = base 16, lowercase, with a mandatory "0x" prefix.
** A NULL pointer is the one exception: glibc prints "(nil)" with no prefix,
** and ignores the '0' flag while doing it.
*/
int	print_ptr(t_format *spec, void *pointer)
{
	char	*prefix;
	char	*digits;

	if (!pointer)
	{
		spec->zero_pad = 0;
		return (emit_text_with_padding(spec, "(nil)", 5));
	}
	prefix = malloc(3);
	if (!prefix)
		return (0);
	prefix[0] = '0';
	prefix[1] = 'x';
	prefix[2] = '\0';
	digits = render_digits((unsigned long)pointer, spec, 16, 0);
	return (emit_and_free(spec, prefix, digits));
}
