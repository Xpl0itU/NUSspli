/***************************************************************************
 * This file is part of NUSspli.                                           *
 * Copyright (c) 2020-2022 V10lator <v10lator@myway.de>                    *
 * Copyright (c) 2022 Xpl0itU <DaThinkingChair@protonmail.com>             *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 ***************************************************************************/

#pragma once

#include <wut-fixups.h>

#include <stdbool.h>

#include <swkbd_wrapper.h>

#include <coreinit/mcp.h>

#define CONFIG_PATH "fs:/vol/external01/NUSspli.txt"
#define TITLE_KEY_URL_MAX_SIZE 1024

#ifdef __cplusplus
	extern "C" {
#endif

typedef enum
{
	NOTIF_METHOD_NONE	= 0x00,
	NOTIF_METHOD_RUMBLE	= 0x01,
	NOTIF_METHOD_LED	= 0x02,
} NOTIF_METHOD;

bool initConfig();
bool saveConfig(bool force);
bool updateCheckEnabled();
void setUpdateCheck(bool enabled);
bool autoResumeEnabled();
void setAutoResume(bool enabled);
Swkbd_LanguageType getKeyboardLanguage();
Swkbd_LanguageType getUnfilteredLanguage();
MCPRegion getRegion();
void setRegion(MCPRegion region);
const char *getFormattedRegion(MCPRegion region);
void setKeyboardLanguage(Swkbd_LanguageType language);
const char *getLanguageString(Swkbd_LanguageType language);
const char *getNotificationString(NOTIF_METHOD method);
bool dlToUSBenabled();
void setDlToUSB(bool toUSB);
NOTIF_METHOD getNotificationMethod();
void setNotificationMethod(NOTIF_METHOD method);

#ifdef __cplusplus
	}
#endif
