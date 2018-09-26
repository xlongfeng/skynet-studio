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

#ifndef __UTILITY_H
#define __UTILITY_H

#ifdef	__cplusplus
extern "C" {
#endif // __cplusplus

#define CMD_BUF_SIZE        32
#define CMD_BUF_OK          0x00
#define CMD_BUF_ERR         0xff

typedef struct {
    char buf[CMD_BUF_SIZE + 1];
    uint8_t rpos;
    uint8_t wpos;
} CmdBuf, *pCmdBuf;

void cmdBufReset(pCmdBuf pbuf);
void cmdBufInit(pCmdBuf pbuf);
void cmdBufPushEnd(pCmdBuf pbuf);
void cmdBufPushByte(pCmdBuf pbuf, char cbyte);
#ifdef DEBUG
void cmdBufPushString(pCmdBuf pbuf, const char *str);
#endif
uint8_t cmdBufSize(pCmdBuf pbuf);
char *cmdBufStr(pCmdBuf pbuf);
void cmdBufGetAddr(pCmdBuf pbuf, uint8_t *pAddr);
void cmdBufGetAction(pCmdBuf pbuf, char *action, uint8_t nsize);
void cmdBufGetArg(pCmdBuf pbuf, uint16_t *pArg);
void cmdBufGetCrc(pCmdBuf pbuf, uint8_t *pCrc);
uint8_t cmdBufValidation(pCmdBuf pbuf);
void cmdBufBuild(pCmdBuf pbuf, uint8_t addr, const char *action, uint16_t arg);
void cmdBufBuildInvalidCrc(pCmdBuf pbuf, uint8_t addr, const char *action, uint16_t arg);

#ifdef	__cplusplus
}
#endif // __cplusplus

#endif // __UTILITY_H
