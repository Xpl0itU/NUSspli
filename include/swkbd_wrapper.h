/***************************************************************************
 * This file is part of NUSspli.                                           *
 * Copyright (c) 2020-2021 V10lator <v10lator@myway.de>                    *
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

#include <stdint.h>
#include <uchar.h>

#include <nn/swkbd.h>

#ifdef __cplusplus
	extern "C" {
#endif

// Enums copied from https://github.com/devkitPro/wut/blob/master/include/nn/swkbd/swkbd_cpp.h
typedef enum
{
   Swkbd_LanguageType__Japanese  = 0,
   Swkbd_LanguageType__English   = 1,
   Swkbd_LanguageType__French    = 2,
   Swkbd_LanguageType__German    = 3,
   Swkbd_LanguageType__Italian   = 4,
   Swkbd_LanguageType__Spanish   = 5,
   Swkbd_LanguageType__Chinese1  = 6,
   Swkbd_LanguageType__Korean    = 7,
   Swkbd_LanguageType__Dutch     = 8,
   Swkbd_LanguageType__Potuguese = 9,
   Swkbd_LanguageType__Russian   = 10,
   Swkbd_LanguageType__Chinese2  = 11,
   Swkbd_LanguageType__Invalid   = 12
} Swkbd_LanguageType;

typedef enum
{
	Swkbd_RegionType__Japan    = 0,
	Swkbd_RegionType__USA      = 1,
	Swkbd_RegionType__Europe   = 2,
   Swkbd_RegionType__China    = 3,
   Swkbd_RegionType__Korea    = 4,
   Swkbd_RegionType__Taiwan   = 5,
} Swkbd_RegionType;

typedef enum
{
   Swkbd_LanguageType2__Japanese  = 0,
   Swkbd_LanguageType2__English   = 1,
   Swkbd_LanguageType2__French    = 6,
   Swkbd_LanguageType2__German    = 7,
   Swkbd_LanguageType2__Italian   = 8,
   Swkbd_LanguageType2__Spanish   = 9,
   Swkbd_LanguageType2__Dutch     = 10,
   Swkbd_LanguageType2__Portuguese = 11,
   Swkbd_LanguageType2__Russian   = 12,
} Swkbd_LanguageType2;

//Structs also from the link above
typedef struct
{
   Swkbd_LanguageType languageType;
   uint32_t unk_0x04;
   uint32_t unk_0x08;
   uint32_t unk_0x0C;
   Swkbd_LanguageType2 languageType2;
   int32_t unk_0x14;
   bool unk_0x18;
   char16_t *str;
   WUT_UNKNOWN_BYTES(0x9C - 0x20);
   uint32_t framerate;
   bool showCursor;
   int32_t unk_0xA4;
} Swkbd_ConfigArg;
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x00, languageType);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x04, unk_0x04);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x08, unk_0x08);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x0C, unk_0x0C);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x10, languageType2);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x14, unk_0x14);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x18, unk_0x18);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x1C, str);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0x9C, framerate);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0xA0, showCursor);
WUT_CHECK_OFFSET(Swkbd_ConfigArg, 0xA4, unk_0xA4);
WUT_CHECK_SIZE(Swkbd_ConfigArg, 0xA8);

typedef struct
{
	uint32_t unk_0x00;
	uint32_t unk_0x04;
	uint32_t unk_0x08;
	int32_t unk_0x0C;
	uint32_t unk_0x10;
	int32_t unk_0x14;
} Swkbd_ReceiverArg;
WUT_CHECK_OFFSET(Swkbd_ReceiverArg, 0x00, unk_0x00);
WUT_CHECK_OFFSET(Swkbd_ReceiverArg, 0x04, unk_0x04);
WUT_CHECK_OFFSET(Swkbd_ReceiverArg, 0x08, unk_0x08);
WUT_CHECK_OFFSET(Swkbd_ReceiverArg, 0x0C, unk_0x0C);
WUT_CHECK_OFFSET(Swkbd_ReceiverArg, 0x10, unk_0x10);
WUT_CHECK_OFFSET(Swkbd_ReceiverArg, 0x14, unk_0x14);
WUT_CHECK_SIZE(Swkbd_ReceiverArg, 0x18);

typedef struct
{
   Swkbd_ConfigArg configArg;
   Swkbd_ReceiverArg receiverArg;
} Swkbd_KeyboardArg;
WUT_CHECK_SIZE(Swkbd_KeyboardArg, 0xC0);

typedef struct
{
   uint32_t unk_0x00;
   int32_t unk_0x04;
   uint32_t unk_0x08;
   uint32_t unk_0x0C;
   int32_t maxTextLength;
   uint32_t unk_0x14;
   uint32_t unk_0x18;
   bool unk_0x1C;
   bool unk_0x1D;
   bool unk_0x1E;
   WUT_PADDING_BYTES(1);
} Swkbd_InputFormArg;
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x00, unk_0x00);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x04, unk_0x04);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x08, unk_0x08);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x0C, unk_0x0C);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x10, maxTextLength);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x14, unk_0x14);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x18, unk_0x18);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x1C, unk_0x1C);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x1D, unk_0x1D);
WUT_CHECK_OFFSET(Swkbd_InputFormArg, 0x1E, unk_0x1E);
WUT_CHECK_SIZE(Swkbd_InputFormArg, 0x20);

typedef struct
{
   Swkbd_KeyboardArg keyboardArg;
   Swkbd_InputFormArg inputFormArg;
} Swkbd_AppearArg;
WUT_CHECK_SIZE(Swkbd_AppearArg, 0xE0);

typedef struct
{
   void *workMemory;
   Swkbd_RegionType regionType;
   uint32_t unk_0x08;
   FSClient *fsClient;
} Swkbd_CreateArg;
WUT_CHECK_OFFSET(Swkbd_CreateArg, 0x00, workMemory);
WUT_CHECK_OFFSET(Swkbd_CreateArg, 0x04, regionType);
WUT_CHECK_OFFSET(Swkbd_CreateArg, 0x08, unk_0x08);
WUT_CHECK_OFFSET(Swkbd_CreateArg, 0x0C, fsClient);
WUT_CHECK_SIZE(Swkbd_CreateArg, 0x10);

typedef struct
{
   VPADStatus *vpad;
   KPADStatus *kpad[4];
} Swkbd_ControllerInfo;
WUT_CHECK_OFFSET(Swkbd_ControllerInfo, 0x00, vpad);
WUT_CHECK_OFFSET(Swkbd_ControllerInfo, 0x04, kpad);
WUT_CHECK_SIZE(Swkbd_ControllerInfo, 0x14);

//Wrapper functions
uint32_t Swkbd_GetWorkMemorySize(uint32_t unk);
bool Swkbd_AppearInputForm(const Swkbd_AppearArg *args);
bool Swkbd_Create(const Swkbd_CreateArg *args);
void Swkbd_SetEnableOkButton(bool enable);
char *Swkbd_GetInputFormString();
const char16_t *Swkbd_GetInputFormString16();
void Swkbd_SetInputFormString(const char *str);
void Swkbd_SetInputFormString16(const char16_t *str);
void Swkbd_Calc(const Swkbd_ControllerInfo *controllerInfo);
bool Swkbd_IsNeedCalcSubThreadFont();
void Swkbd_CalcSubThreadFont();
bool Swkbd_IsNeedCalcSubThreadPredict();
void Swkbd_CalcSubThreadPredict();
void Swkbd_DrawTV();
void Swkbd_DrawDRC();
bool Swkbd_IsDecideOkButton(bool *outIsSelected);
bool Swkbd_IsDecideCancelButton(bool *outIsSelected);
bool Swkbd_DisappearInputForm();
void Swkbd_Destroy();
bool Swkbd_IsHidden();
bool Swkbd_IsReady();

#ifdef __cplusplus
	}
#endif
