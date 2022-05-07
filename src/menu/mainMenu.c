/***************************************************************************
 * This file is part of NUSspli.                                           *
 * Copyright (c) 2019-2020 Pokes303                                        *
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

#include <wut-fixups.h>

#include <input.h>
#include <installer.h>
#include <iosuhaxx.h>
#include <renderer.h>
#include <state.h>
#include <ticket.h>
#include <utils.h>
#include <menu/config.h>
#include <menu/filebrowser.h>
#include <menu/installer.h>
#include <menu/insttitlebrowser.h>
#include <menu/titlebrowser.h>
#include <menu/main.h>
#include <menu/utils.h>

#include <coreinit/memdefaultheap.h>

#include <string.h>

static bool cfwWarningShown = false;

static void drawMainMenuFrame()
{
	startNewFrame();
	boxToFrame(0, 5);
	textToFrame(1, ALIGNED_CENTER, "NUSspli");
	textToFrame(3, ALIGNED_CENTER, "NUS simple packet loader/installer [" NUSSPLI_VERSION "]");
	
	textToFrame(4, ALIGNED_CENTER, NUSSPLI_COPYRIGHT);
	
	textToFrame(10, 0, "Press " BUTTON_A " to download content");
	textToFrame(11, 0, "Press " BUTTON_X " to install content");
	textToFrame(12, 0, "Press " BUTTON_RIGHT " to uninstall a title");
	textToFrame(13, 0, "Press " BUTTON_LEFT " for options");
	textToFrame(14, 0, "Press " BUTTON_HOME " or " BUTTON_B " to exit");
	
	textToFrame(7, MAX_CHARS - 27, "Thanks to:");
	textToFrame(8, MAX_CHARS - 26, "• E1ite007");
	textToFrame(9, MAX_CHARS - 26, "• Pokes303");
    textToFrame(10, MAX_CHARS - 26, "• SDL");
	textToFrame(11, MAX_CHARS - 26, "• Simone Z.");
	textToFrame(12, MAX_CHARS - 26, "• WUP installer");
	
	textToFrame(14, MAX_CHARS - 27, "Beta testers:");
	textToFrame(15, MAX_CHARS - 26, "• carcaschoi");
	textToFrame(16, MAX_CHARS - 26, "• DaThinkingChair");
	textToFrame(17, MAX_CHARS - 26, "• huma_dawii");
	textToFrame(18, MAX_CHARS - 26, "• jacobsson");
	textToFrame(19, MAX_CHARS - 26, "• LuckyDingo");
	textToFrame(20, MAX_CHARS - 26, "• pirate");
	textToFrame(21, MAX_CHARS - 26, "• Vague Rant");
	
	textToFrame(MAX_LINES - 3, 0, "WARNING:");
	textToFrame(MAX_LINES - 2, 1, "• Don't eject the SD Card / USB drive or the application will crash!");
	textToFrame(MAX_LINES - 1, 1, "• You are unable to exit while installing a game");
	drawFrame();
}

void mainMenu()
{
	drawMainMenuFrame();

	int ovl;
	if(cfwWarningShown || openIOSUhax())
		ovl = -1;
	else
	{
		ovl = addErrorOverlay(
			"No CFW detected!\n"
			"\n"
			"NUSspli won't work correctly without a CFW.\n"
			"Have a look at https://wiiu.hacks.guide\n"
			"\n"
			"Press any button to close this warning."
		);

		if(ovl >= 0)
			cfwWarningShown = true;
	}
	
	while(AppRunning())
	{
		if(app == APP_STATE_BACKGROUND)
			continue;
		if(app == APP_STATE_RETURNING)
			drawMainMenuFrame();
		
		showFrame();
		
		if(ovl < 0)
		{
			if(vpad.trigger & VPAD_BUTTON_A)
			{
				titleBrowserMenu();
				drawMainMenuFrame();
			}
			else if(vpad.trigger & VPAD_BUTTON_X)
			{
				char *dir = fileBrowserMenu();
				if(dir != NULL)
				{
					installerMenu(dir);
					MEMFreeToDefaultHeap(dir);
				}
				drawMainMenuFrame();
			}
			else if(vpad.trigger & VPAD_BUTTON_LEFT)
			{
				configMenu();
				drawMainMenuFrame();
			}
			else if(vpad.trigger & VPAD_BUTTON_RIGHT)
			{
				ititleBrowserMenu();
				drawMainMenuFrame();
			}
			else if(vpad.trigger & VPAD_BUTTON_B)
				return;
		}
		else if(vpad.trigger)
		{
			removeErrorOverlay(ovl);
			ovl = -1;
		}
	}
}
