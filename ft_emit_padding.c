/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_emit_padding.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <login> <login@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:00:00 by <login>           #+#    #+#             */
/*   Updated: 2026/10/31 12:00:00 by <login>          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** Every conversion is three pieces:
**   prefix  - sign or 0x          ("-", "+", " ", "0x", "0X", "")
**   payload - the characters      (digits, or the string itself)
**   padding - fillers to reach spec->min_width
** The three emit_* helpers below differ only in WHERE the padding goes.
*/

/* "-" : payload first, fillers on the right, always with spaces. */
static int	emit_left_aligned(char *prefix, char *payload,
		int payload_len, int padding)
{
	int	prefix_len;

	prefix_len = ft_strlen(prefix);
	emit_text(prefix, prefix_len);
	emit_text(payload, payload_len);
	emit_padding(padding, ' ');
	return (prefix_len + payload_len + padding);
}

/* "0" : prefix first, THEN zeros, then payload. Never "000-42". */
static int	emit_zero_padded(char *prefix, char *payload, int payload_len,
		int padding)
{
	int	prefix_len;

	prefix_len = ft_strlen(prefix);
	emit_text(prefix, prefix_len);
	emit_padding(padding, '0');
	emit_text(payload, payload_len);
	return (prefix_len + payload_len + padding);
}

/* default: spaces on the left, then prefix, then payload. */
static int	emit_space_padded(char *prefix, char *payload, int payload_len,
		int padding)
{
	int	prefix_len;

	prefix_len = ft_strlen(prefix);
	emit_padding(padding, ' ');
	emit_text(prefix, prefix_len);
	emit_text(payload, payload_len);
	return (prefix_len + payload_len + padding);
}

int	emit_with_padding(t_format *spec, char *prefix, char *payload,
		int payload_len)
{
	int	padding;

	padding = spec->min_width - ft_strlen(prefix) - payload_len;
	if (padding < 0)
		padding = 0;
	if (spec->left_align)
		return (emit_left_aligned(prefix, payload, payload_len, padding));
	if (spec->zero_pad)
		return (emit_zero_padded(prefix, payload, payload_len, padding));
	return (emit_space_padded(prefix, payload, payload_len, padding));
}

/* Same as emit_with_padding, but owns the two buffers and frees them.
   Used by the number printers, which build their pieces on the heap. */
int	emit_and_free(t_format *spec, char *prefix, char *payload)
{
	int	count;

	if (!prefix || !payload)
	{
		free(prefix);
		free(payload);
		return (0);
	}
	count = emit_with_padding(spec, prefix, payload, ft_strlen(payload));
	free(prefix);
	free(payload);
	return (count);
}
