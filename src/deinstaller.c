/***************************************************************************
 * This file is part of NUSspli.                                           *
 * Copyright (c) 2021-2022 V10lator <v10lator@myway.de>                    *
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

#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#include <coreinit/ios.h>
#include <coreinit/mcp.h>
#include <coreinit/memdefaultheap.h>
#include <coreinit/memory.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>

#include <crypto.h>
#include <deinstaller.h>
#include <file.h>
#include <filesystem.h>
#include <input.h>
#include <ioQueue.h>
#include <notifications.h>
#include <osdefs.h>
#include <renderer.h>
#include <state.h>
#include <utils.h>
#include <menu/utils.h>

bool deinstall(MCPTitleListType *title, const char *name, bool channelHaxx)
{
	startNewFrame();
	textToFrame(0, 0, "Uninstalling");
	textToFrame(0, 19, name);
	textToFrame(1, 0, "Preparing...");
	writeScreenLog(2);
	drawFrame();
	showFrame();

	MCPInstallTitleInfo *info = MEMAllocFromDefaultHeapEx(sizeof(MCPInstallTitleInfo), 0x40);
	if(info == NULL)
		return false;

	McpData data;
	glueMcpData(info, &data);
	
	if(!channelHaxx)
		disableShutdown();
	
	//err = MCP_UninstallTitleAsync(mcpHandle, title->path, info);
	// The above crashes MCP, so let's leave WUT:
	debugPrintf("Deleting %s", title->path);
	OSTick t = OSGetTick();
	MCPError err = MCP_DeleteTitleAsync(mcpHandle, title->path, info);
	if(err != 0)
	{
		debugPrintf("Err1: %#010x (%d)", err, err);
		if(!channelHaxx)
			enableShutdown();
		return false;
	}
	
	if(!channelHaxx)
		showMcpProgress(&data, name, false);

	t = OSGetTick() - t;
	addEntropy(&t, sizeof(OSTick));
	addToScreenLog("Deinstallation finished!");
	
	if(channelHaxx)
		return true;

	enableShutdown();
	startNotification();

	colorStartNewFrame(SCREEN_COLOR_D_GREEN);
	textToFrame(0, 0, name);
	textToFrame(1, 0, "Uninstalled successfully!");
	writeScreenLog(2);
	drawFrame();
	
	while(AppRunning())
	{
		if(app == APP_STATE_BACKGROUND)
			continue;
		if(app == APP_STATE_RETURNING)
		{
			colorStartNewFrame(SCREEN_COLOR_D_GREEN);
			textToFrame(0, 0, name);
			textToFrame(1, 0, "Uninstalled successfully!");
			writeScreenLog(2);
			drawFrame();
		}
		
		showFrame();
		
		if(vpad.trigger)
			break;
	}

	stopNotification();
	return true;
}
