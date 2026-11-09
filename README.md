*This project has been created as part of the 42 curriculum by `<login>`.*

# ft_printf

## Description

A re-implementation of the C standard library's `printf()`, packaged as a static
library `libftprintf.a`:

```c
int	ft_printf(const char *format, ...);
```

Mandatory conversions: `%c %s %p %d %i %u %x %X %%`.
Bonus, **fully implemented**: every combination of `-` `0` `.` (precision),
field width, `#`, `+` and space, on every conversion.

The return value is the number of characters written, exactly like the real
`printf` — this is the part that is easiest to get wrong, and the first thing an
evaluator checks. No buffer management: every piece is written straight to
`fd 1` and counted.

## Instructions

```bash
make                    # builds libftprintf.a at the root (ar, not libtool)
make clean / fclean / re
```

```bash
cc -Wall -Wextra -Werror main.c libftprintf.a -o test
```

## The algorithm

`ft_printf` walks the format string:

* a normal character → write it, add 1 to the count;
* `%` → `parse_format` fills a `t_format`, then `print_conversion` dispatches
  on the conversion character.

```c
typedef struct s_format
{
	int	left_align;     /* '-' : pad on the right, with spaces            */
	int	zero_pad;       /* '0' : pad on the left, with zeros              */
	int	show_plus;      /* '+' : sign for positive numbers too            */
	int	show_space;     /* ' ' : a space instead of '+'                   */
	int	alt_form;       /* '#' : 0x / 0X in front of hex numbers          */
	int	min_width;      /* digits before the '.', e.g. 08                 */
	int	has_precision;  /* '.' was present (so precision 0 is meaningful) */
	int	precision;      /* digits after the '.', e.g. .5                  */
	int	conversion;     /* c s p d i u x X %                              */
}	t_format;
```

Every conversion is built from **three pieces**:

| piece | what it holds | built by |
|---|---|---|
| **prefix** | sign (`-` `+` space) or base marker (`0x` `0X`) | `make_sign_prefix`, `make_hex_prefix` |
| **payload** | the digits, or the string itself | `render_digits`, `print_str` |
| **padding** | fillers that bring the field up to `min_width` | `emit_padded` |

and the padding is decided in exactly one place:

```c
emit_padded(spec, prefix, payload, payload_len)
      ├── spec->left_align -> prefix + payload + spaces
      ├── spec->zero_pad   -> prefix + zeros   + payload
      └── otherwise        -> spaces + prefix  + payload
```

Keeping padding in **one** function is why the bonus is cheap: a new flag
changes one branch, not eleven conversions.

Numbers all go through `digits_in_base(value, base, uppercase)`: `%u` is base
10, `%x`/`%X` is base 16, `%p` is base 16 with a `"0x"` prefix. One renderer,
four callers.

## Reading the variable names

| name | meaning |
|---|---|
| `spec` | the parsed conversion (`t_format`) |
| `cursor` | a `const char **` walking the format string inside `parse_format` |
| `prefix` | sign or `0x`, possibly empty `""` |
| `payload` | the characters to print after the prefix |
| `padding` | how many fill characters are still needed |
| `magnitude` | the absolute value of a signed number, as `unsigned long` |
| `conversion` | the letter after `%` |
| `uppercase` | 1 for `%X`, 0 for `%x` and `%p` |
| `fill` | the character used for padding: `' '` or `'0'` |
| `target_length` | the length the digits must reach (`precision`) |

## The traps

1. **`INT_MIN`.** `-(-2147483648)` overflows. The magnitude is built as
   `(unsigned long)(-(value + 1)) + 1`, which is safe for every signed value.
2. **Precision kills the `0` flag.** `%08.3d` of 42 is `"     042"`, not
   `"00000042"`: with a precision, zeros belong to the digits, and the field is
   padded with spaces.
3. **`-` beats `0`.** `%-05d` of 42 is `"42   "` — left-aligned, spaces on the
   right.
4. **`#` is hex only.** `%#x` of 0 is `"0"` (no `0x` prefix for zero), and
   `%#u` / `%#d` print **no** prefix at all — base 10 has no alternative form.
   This one was found by the diff harness, not by reading the standard.
5. **The return value.** Every character counts: `%%`, the `0x` of `%p`, all
   padding.
6. `%p` with `NULL` prints `(nil)`; `%s` with `NULL` prints `(null)`. Both
   match glibc, which is what the evaluator diffs against.

## Verification — differential test

The harness runs the same format strings twice, once through the real `printf`
and once through `ft_printf`, and prints both return values, so a mismatch in
**either** the text or the count shows up in a plain `diff`.

`tests/main.c` — 24 hand-picked cases (readable).
`tests/big.c` — 2004 generated cases: every flag combination × width ×
precision for `d i u x X`, plus `c s p %`.
`tests/big2.c` — 329 cases: flag **orderings**, degenerate precisions
(`%.0s`, `%.0d`, `%#.0x`, `%-+05.3d`, `%.20p`) and `%s` with `NULL` at every
precision (`%.1s` of `NULL` prints **nothing**: precision < 6 on glibc).

```bash
make
cc -w -I. tests/big.c libftprintf.a -o tests/big      # -w: the macro also
./tests/big 1 > real.txt && ./tests/big 2 > mine.txt  # feeds -Wformat through
diff real.txt mine.txt && echo IDENTICAL              # the real printf
```

Result: **2357 cases identical to glibc 2.41, including return values**, and
clean under `-fsanitize=address,undefined` (no leaks, no UB).

Also verified with an independent tester (paulo-santana/ft_printf_tester,
which diffs against the real `printf` and runs under AddressSanitizer):

| suite | result |
|---|---|
| mandatory (`cspdiuxX%`) | **101 / 101** |
| bonus 1 (`-` `0` `.` width) | **938 / 938** |
| bonus 2 (`#` `+` space) | **105 / 105** |
| **total** | **1142 / 1142 (100%)**, 0 leaks |

Speed: 2357 differential calls in 3 ms; 2000 mixed `%#012.5x %-20.10s %08.3d`
calls in 30 ms. No timeout risk anywhere.

⚠️ Run the harness on **Linux**. Apple's libc prints `0x0` for `%p` of `NULL`
where glibc prints `(nil)`; the moulinette runs glibc.

## Preparing for the defence modification test

The usual live request is *"add `%b` (binary)"*. Two lines, in
`print_conversion`:

```c
if (spec->conversion == 'b')
	return (print_unsigned(spec, va_arg(*args, unsigned int), 2, 0));
```

`digits_in_base` already takes the base as a parameter, and `%b` then inherits
width, precision, `-`, `0`, `#` and space for free.

## Files, in execution order

| # | file | functions | what it owns |
|---|---|---|---|
| 1 | `ft_printf.c` | `dispatch_conversion`, `ft_printf` | the loop over the format string, and the switch on the conversion |
| 2 | `ft_parse_format.c` | `reset_format`, `consume_flag`, `read_number`, `parse_format` | the grammar `%[flags][width][.precision]` |
| 3 | `ft_print_text.c` | `emit_text_with_padding`, `print_char`, `print_str` | `%c`, `%s` |
| 4 | `ft_print_number.c` | `make_sign_prefix`, `make_hex_prefix`, `render_digits`, `print_signed`, `print_unsigned` | `%d %i %u %x %X`: sign, prefix, precision |
| 5 | `ft_print_pointer.c` | `print_ptr` | `%p`, base 16 with a forced `0x` |
| 6 | `ft_render_digits.c` | `base_digit`, `copy_forward`, `fill_reversed`, `digits_in_base`, `pad_digits_with_zeros` | number → characters (knows nothing about printf) |
| 7 | `ft_emit_padding.c` | `emit_left_aligned`, `emit_zero_padded`, `emit_space_padded`, `emit_with_padding`, `emit_and_free` | **where the padding goes** — the only place |
| 8 | `ft_write_text.c` | `ft_strlen`, `emit_text`, `emit_padding` | the single funnel to `write` |

## Resources

* `man 3 stdarg`, `man 3 printf`, `man 3 write`
* <https://en.wikipedia.org/wiki/Variadic_function>
* *The C Programming Language*, Kernighan & Ritchie, §7.3 (variable-length
  argument lists)

### How AI was used

AI was used to generate the differential test harness, to review this
explanation, and to rename the internals so that every variable says what it
holds. The parser, the emit/padding logic and every conversion were written and
debugged by hand; the `prepend_zeros` bug (a reversed copy plus a missing
terminating `'\0'`) and the `%#u` bug were found by the diff test, not by AI.
