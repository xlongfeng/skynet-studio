/*
 * skynet sensor
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os.h"

uint8_t device_id = 0x01, device_type = 0xff;

void write_device_id(uint8_t id)
{
    device_id = id;
}

uint8_t read_device_id(void)
{
    return device_id;
}

void write_device_type(uint8_t type)
{
    device_type = type;
}

uint8_t read_device_type(void)
{
    return device_type;
}

#ifdef DEBUG
void display_banner(void)
{
    hdprintf("------------------------------\n");
    hdprintf("| STM8 Skynet Sensor Program |\n");
    hdprintf("------------------------------\n");
}
#endif

void device_switch(DeviceType type)
{

}

void os_init(void)
{
#ifdef DEBUG
    display_banner();
#endif
}

void os_schedule(void)
{

}

uint16_t query_device(void)
{
    return 0x1234;
}

/*-------------------------------------------------------------------------
 usage:

 _ultoa(unsigned long value, char* string, int radix)
 _ltoa(long value, char* string, int radix)

 value  ->  Number to be converted
 string ->  Result
 radix  ->  Base of value (e.g.: 2 for binary, 10 for decimal, 16 for hex)
---------------------------------------------------------------------------*/

/* "11110000111100001111000011110000" base 2 */
/* "37777777777" base 8 */
/* "4294967295" base 10 */
#define NUMBER_OF_DIGITS 32	/* eventually adapt if base 2 not needed */

#if NUMBER_OF_DIGITS < 32
# warning _ltoa() and _ultoa() are not save for radix 2
#endif

#if defined (__SDCC_mcs51) && defined (__SDCC_MODEL_SMALL) && !defined (__SDCC_STACK_AUTO)
# define MEMSPACE_BUFFER __idata	/* eventually __pdata or __xdata */
# pragma nogcse
#else
# define MEMSPACE_BUFFER
#endif

void _ultoa(unsigned long value, char* string, unsigned char radix)
{
    char MEMSPACE_BUFFER buffer[NUMBER_OF_DIGITS];  /* no space for '\0' */
    unsigned char index = NUMBER_OF_DIGITS;

    do {
        unsigned char c = '0' + (value % radix);
        if (c > (unsigned char)'9')
            c += 'A' - '9' - 1;
        buffer[--index] = c;
        value /= radix;
    } while (value);

    do {
        *string++ = buffer[index];
    } while ( ++index != NUMBER_OF_DIGITS );

    *string = 0;  /* string terminator */
}

void _ltoa(long value, char* string, unsigned char radix)
{
    if (value < 0 && radix == 10) {
        *string++ = '-';
        value = -value;
    }
    _ultoa(value, string, radix);
}
