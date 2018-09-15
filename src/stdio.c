/*
 * stdio.c - reduced version of sprintf printf
 *
 * Copyright (c) 2018, longfeng.xiao <xlongfeng@126.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* following formats are supported :-
   format     output type       argument-type
     %d        decimal             int
     %ld       decimal             long
     %hd       decimal             char
     %x        hexadecimal         int
     %lx       hexadecimal         long
     %hx       hexadecimal         char
     %o        octal               int
     %lo       octal               long
     %ho       octal               char
     %c        character           char
     %s        character           generic pointer
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "os.h"

static pfn_outputchar output_char;
static void* p;
static int charsOutputted;
static char radix ;
static unsigned char long_flag = 0;
static unsigned char string_flag =0;
static unsigned char char_flag = 0;
static char * str ;
static long val;

#define OUTPUT_CHAR(c, p)   _output_char(c)
static void _output_char(unsigned char c)
{
    output_char( c, p );
    charsOutputted++;
}

int _print_format (pfn_outputchar pfn, void* pvoid, const char *fmt, va_list ap)
{
    output_char = pfn;
    p = pvoid;
    charsOutputted = 0;

    for (; *fmt ; fmt++ ) {
        if (*fmt == '%') {
            long_flag = string_flag = char_flag = 0;
            fmt++ ;
            switch (*fmt) {
            case 'l':
                long_flag = 1;
                fmt++;
                break;
            case 'h':
                char_flag = 1;
                fmt++;
            }

            switch (*fmt) {
            case 's':
                string_flag = 1;
                break;
            case 'd':
                radix = 10;
                break;
            case 'x':
                radix = 16;
                break;
            case 'c':
                radix = 0;
                break;
            case 'o':
                radix = 8;
                break;
            }

            if (string_flag) {
                str = va_arg(ap, char *);
                while (*str) OUTPUT_CHAR(*str++, p);
                continue ;
            }

            if (long_flag)
                val = va_arg(ap,long);
            else
                if (char_flag)
                    val = va_arg(ap,char);
                else
                    val = va_arg(ap,int);

            if (radix)
            {
              static char buffer[12]; /* 37777777777(oct) */
              char * stri;

              _ltoa (val, buffer, radix);
              stri = buffer;
              while (*stri)
                {
                  OUTPUT_CHAR(*stri, p);
                  stri++;
                }
            }
            else
              OUTPUT_CHAR((char)val, p);
        } else
            OUTPUT_CHAR(*fmt, p);
    }

    return charsOutputted;
}

static void put_char_to_string (char c, void* p)
{
    char **buf = (char **)p;
    *(*buf)++ = c;
}

int vsprintf_small(char *buf, const char *fmt, va_list ap)
{
    int i;
    i = _print_format(put_char_to_string, &buf, fmt, ap);
    *buf = 0;
    return i;
}

int sprintf_small(char *buf, const char *fmt, ...)
{
    int i;
    va_list ap ;
    va_start(ap,fmt);
    i = _print_format(put_char_to_string, &buf, fmt, ap);
    *buf = 0;
    va_end(ap);
    return i;
}

static void put_char_to_stdout (char c, void* p)
{
    p;  //make compiler happy
    putchar(c);
}

int vprintf_small (const char *format, va_list ap)
{
  return _print_format (put_char_to_stdout, NULL, format, ap);
}

void printf_small(char *fmt, ...)
{
    va_list ap ;
    va_start(ap,fmt);
    _print_format(put_char_to_stdout, NULL, fmt, ap);
    va_end(ap);
}
