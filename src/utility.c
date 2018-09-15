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
#include <errno.h>

#include "os.h"
#include "utility.h"


void cmdBufReset(pCmdBuf pbuf)
{
    memset(pbuf->buf, '\0', sizeof(pbuf->buf));
    pbuf->rpos = 0;
    pbuf->wpos = 0;
}

void cmdBufInit(pCmdBuf pbuf)
{
    cmdBufReset(pbuf);
}

void cmdBufPushEnd(pCmdBuf pbuf)
{
    pbuf->buf[pbuf->wpos++] = '\0';
}

void cmdBufPushByte(pCmdBuf pbuf, char cbyte)
{
    if (cbyte == '@') {
        cmdBufReset(pbuf);
    } else if (pbuf->wpos > 0 && pbuf->buf[0] != '@') {
        cmdBufReset(pbuf);
        return;
    }
    pbuf->buf[pbuf->wpos++] = cbyte;
    if (pbuf->wpos == CMD_BUF_SIZE)
        cmdBufReset(pbuf);
}

#ifdef DEBUG
void cmdBufPushString(pCmdBuf pbuf, const char *str)
{
    const char *p = str;
    while (*p)
        cmdBufPushByte(pbuf, *p++);
}
#endif

uint8_t cmdBufSize(pCmdBuf pbuf)
{
    uint8_t size = strlen(pbuf->buf);
    return size < pbuf->wpos ? size : pbuf->wpos;
}

char *cmdBufStr(pCmdBuf pbuf)
{
    return pbuf->buf;
}

void cmdBufGetAddr(pCmdBuf pbuf, uint8_t *pAddr)
{
    char *startptr;
    char *endptr;
    uint8_t n;

    if (pbuf->wpos > pbuf->rpos && pbuf->buf[0] == '@') {
        ++pbuf->rpos;
        startptr = pbuf->buf + pbuf->rpos;
        endptr = strchr(startptr, ',');
        if (endptr != 0) {
            n = endptr - startptr;
            pbuf->rpos += n;
            *pAddr = (uint8_t)(uint8_t)strtoul(startptr, 0, 16);
            return;
        }
    }

    *pAddr = 0xff;
}

void cmdBufGetAction(pCmdBuf pbuf, char *action, uint8_t nsize)
{
    char *startptr;
    char *endptr;
    uint8_t n;

    memset(action, '\0', nsize);
    if (pbuf->wpos > pbuf->rpos) {
        ++pbuf->rpos;
        startptr = pbuf->buf + pbuf->rpos;
        endptr = strchr(startptr, ',');
        if (endptr != 0) {
            n = endptr - startptr;
            pbuf->rpos += n;
            n = n < nsize ? n : nsize;
            strncpy(action, startptr, n);
        }
    }
}

void cmdBufGetArg(pCmdBuf pbuf, uint16_t *pArg)
{
    char *startptr;
    char *endptr;
    uint8_t n;

    if (pbuf->wpos > pbuf->rpos) {
        ++pbuf->rpos;
        startptr = pbuf->buf + pbuf->rpos;
        endptr = strchr(startptr, ',');
        if (endptr != 0) {
            n = endptr - startptr;
            pbuf->rpos += n;
            *pArg = (uint16_t)strtoul(startptr, 0, 16);
            return;
        }
    }

    *pArg = 0xff;
}

void cmdBufGetCrc(pCmdBuf pbuf, uint8_t *pCrc)
{
    char *startptr;

    if (pbuf->wpos > pbuf->rpos) {
        ++pbuf->rpos;
        startptr = pbuf->buf + pbuf->rpos;
        *pCrc = (uint8_t)strtoul(startptr, 0, 16);
        return;
    }

    *pCrc = 0xff;
}

uint8_t cmdBufCalcCrc(pCmdBuf pbuf, uint8_t n)
{
    uint8_t crc = 0;
    uint8_t i;

    crc = 0;
    for (i = 0; i < n; i++) {
        crc += pbuf->buf[i];
    }

    return crc;
}

uint8_t cmdBufValidation(pCmdBuf pbuf)
{
    uint8_t oCrc, vCrc;
    char *endptr;
    uint8_t n;

    cmdBufGetCrc(pbuf, &oCrc);

    endptr = strrchr(pbuf->buf, ',');
    if (!endptr) {
        return CMD_BUF_ERR;
    }
    n = endptr - pbuf->buf;
    vCrc = cmdBufCalcCrc(pbuf, n);

    if (oCrc == vCrc) {
        return CMD_BUF_OK;
    } else {
#ifdef DEBUG
        hdprintf("oCrc %x != vCrc %x\n", oCrc, vCrc);
#endif
        return CMD_BUF_ERR;
    }
}

void cmdBufBuild(pCmdBuf pbuf, uint8_t addr, const char *action, uint16_t arg)
{
    uint8_t n;
    n = sprintf_small(pbuf->buf, "@%x,%s,%x", addr, action, arg);
    n += sprintf_small(pbuf->buf + n, ",%x\n", cmdBufCalcCrc(pbuf, n));
}
