/***************************************************************************
 * This file is part of NUSspli.                                           *
 * Copyright (c) 2019-2020 Pokes303                                        *
 * Copyright (c) 2020-2022 V10lator <v10lator@myway.de>                    *
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

#include <config.h>
#include <filesystem.h>
#include <input.h>
#include <renderer.h>
#include <state.h>
#include <titles.h>
#include <utils.h>
#include <menu/download.h>
#include <menu/predownload.h>
#include <menu/titlebrowser.h>
#include <menu/utils.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <coreinit/mcp.h>
#include <coreinit/memdefaultheap.h>

#include <algorithm>
#include <vector>

#define MAX_TITLEBROWSER_LINES (MAX_LINES - 4)

static TitleEntry **filteredTitleEntries;
static size_t filteredTitleEntrySize;
static size_t oldPos;

std::vector <TitleEntry *> titleQueue;
static void drawTBMenuFrame(const TITLE_CATEGORY tab, const size_t pos, const size_t cursor, char *search)
{
	startNewFrame();

	// Games, Updates, DLC, Demos, All
	static const char *tabLabels[5] = { "Games", "Updates", "DLC", "Demos", "All" };
	for(int i = 0; i < 5; ++i)
		tabToFrame(0, i, tabLabels[i], i == tab);

	boxToFrame(1, MAX_LINES - 2);

	textToFrame(MAX_LINES - 1, ALIGNED_CENTER, "Press " BUTTON_A " to select || " BUTTON_B " to return || " BUTTON_X " to enter a title ID || " BUTTON_Y " to search");

	size_t j;
	size_t max;
	size_t l;
	if(pos != oldPos)
	{
		filteredTitleEntrySize = getTitleEntriesSize(tab);
		const TitleEntry *titleEntrys = getTitleEntries(tab);
		MCPRegion currentRegion = getRegion();
		l = 0;

		if(search[0] != '\0')
		{
			do
				search[l] = tolower(search[l]);
			while(search[l++]);

			l = 0;
			char *ptr[2];
			bool found;
			char tmpName[MAX_TITLENAME_LENGTH];
			for(size_t i = 0 ; i < filteredTitleEntrySize; ++i)
			{
				if(!(currentRegion & titleEntrys[i].region))
					continue;

				max = strlen(titleEntrys[i].name);
				for(j = 0; j < max; ++j)
					tmpName[j] = tolower(titleEntrys[i].name[j]);

				tmpName[j] = '\0';
				ptr[0] = search;
				ptr[1] = strstr(ptr[0], " ");
				while(true)
				{
					if(ptr[1] != NULL)
						ptr[1][0] = '\0';

					found = strstr(tmpName, ptr[0]) != NULL;

					if(ptr[1] != NULL)
					{
						ptr[1][0] = ' ';
						if(found)
						{
							ptr[0] = ptr[1];
							ptr[1] = strstr(++ptr[0], " ");
						}
						else
							break;
					}
					else
						break;
				}

				if(found)
					filteredTitleEntries[l++] = (TitleEntry *)titleEntrys + i;
			}
		}
		else
			for(size_t i = 0; i < filteredTitleEntrySize; ++i)
				if(currentRegion & titleEntrys[i].region)
					filteredTitleEntries[l++] = (TitleEntry *)titleEntrys + i;

		filteredTitleEntrySize = l;
	}

	j = filteredTitleEntrySize - pos;
	max = j < MAX_TITLEBROWSER_LINES ? j : MAX_TITLEBROWSER_LINES;
	MCPTitleListType titleList;
	char *toFrame = getToFrameBuffer();
	for(size_t i = 0; i < max; ++i)
	{
		l = i + 2;
		if(cursor == i)
			arrowToFrame(l, 1);

		j = i + pos;
		if(MCP_GetTitleInfo(mcpHandle, filteredTitleEntries[j]->tid, &titleList) == 0)
			checkmarkToFrame(l, 4);

		flagToFrame(l, 7, filteredTitleEntries[j]->region);

		if(tab == TITLE_CATEGORY_ALL)
		{
			if(isDLC(filteredTitleEntries[j]))
				strcpy(toFrame, "[DLC] ");
			else if(isUpdate(filteredTitleEntries[j]))
				strcpy(toFrame, "[UPD] ");
			else
			{
				textToFrameCut(l, 10, filteredTitleEntries[j]->name, (1280 - (FONT_SIZE << 1)) - (getSpaceWidth() * 11));
				continue;
			}

			strcat(toFrame, filteredTitleEntries[j]->name);
			textToFrameCut(l, 10, toFrame, (1280 - (FONT_SIZE << 1)) - (getSpaceWidth() * 11));
		}
		else
			textToFrameCut(l, 10, filteredTitleEntries[j]->name, (1280 - (FONT_SIZE << 1)) - (getSpaceWidth() * 11));
	}
	drawFrame();
}

void titleBrowserMenu()
{
	filteredTitleEntrySize = getTitleEntriesSize(TITLE_CATEGORY_ALL);
	filteredTitleEntries = (TitleEntry **)MEMAllocFromDefaultHeap(filteredTitleEntrySize * sizeof(uintptr_t));
	if(filteredTitleEntries == NULL)
	{
		debugPrintf("Titlebrowser: OUT OF MEMORY!");
		return;
	}

	TITLE_CATEGORY tab = TITLE_CATEGORY_GAME;
	size_t cursor = 0;
	size_t pos = 0;
	char search[129];
	search[0] = u'\0';
	oldPos = 99;

	drawTBMenuFrame(tab, pos, cursor, search);

	bool mov = filteredTitleEntrySize >= MAX_TITLEBROWSER_LINES;
	bool redraw = false;
	const TitleEntry *entry;
	uint32_t oldHold = 0;
	size_t frameCount = 0;
	bool dpadAction;
	while(AppRunning())
	{
		if(app == APP_STATE_BACKGROUND)
			continue;
		if(app == APP_STATE_RETURNING)
			drawTBMenuFrame(tab, pos, cursor, search);
		
		showFrame();
		if(vpad.trigger & VPAD_BUTTON_A)
		{
			entry = filteredTitleEntries[cursor + pos];
			break;
		}
		
		if(vpad.trigger & VPAD_BUTTON_B)
		{
			MEMFreeToDefaultHeap(filteredTitleEntries);
			return;
		}
		
		if(vpad.hold & VPAD_BUTTON_UP)
		{
			if(oldHold != VPAD_BUTTON_UP)
			{
				oldHold = VPAD_BUTTON_UP;
				frameCount = 30;
				dpadAction = true;
			}
			else if(frameCount == 0)
				dpadAction = true;
			else
			{
				--frameCount;
				dpadAction = false;
			}

			if(dpadAction)
			{
				if(cursor)
					cursor--;
				else
				{
					if(mov)
					{
						if(pos)
							pos--;
						else
						{
							cursor = MAX_TITLEBROWSER_LINES - 1;
							pos = filteredTitleEntrySize - MAX_TITLEBROWSER_LINES;
						}
					}
					else
						cursor = filteredTitleEntrySize - 1;
				}

				redraw = true;
			}
		}
		else if(vpad.hold & VPAD_BUTTON_DOWN)
		{
			if(oldHold != VPAD_BUTTON_DOWN)
			{
				oldHold = VPAD_BUTTON_DOWN;
				frameCount = 30;
				dpadAction = true;
			}
			else if(frameCount == 0)
				dpadAction = true;
			else
			{
				--frameCount;
				dpadAction = false;
			}

			if(dpadAction)
			{
				if(cursor + pos >= filteredTitleEntrySize - 1 || cursor >= MAX_TITLEBROWSER_LINES - 1)
				{
					if(!mov || ++pos + cursor >= filteredTitleEntrySize)
						cursor = pos = 0;
				}
				else
					++cursor;

				redraw = true;
			}
		}
		else if(mov)
		{
			if(vpad.hold & VPAD_BUTTON_RIGHT)
			{
				if(oldHold != VPAD_BUTTON_RIGHT)
				{
					oldHold = VPAD_BUTTON_RIGHT;
					frameCount = 30;
					dpadAction = true;
				}
				else if(frameCount == 0)
					dpadAction = true;
				else
				{
					--frameCount;
					dpadAction = false;
				}

				if(dpadAction)
				{
					pos += MAX_TITLEBROWSER_LINES;
					if(pos >= filteredTitleEntrySize)
						pos = 0;
					cursor = 0;
					redraw = true;
				}
			}
			else if(vpad.hold & VPAD_BUTTON_LEFT)
			{
				if(oldHold != VPAD_BUTTON_LEFT)
				{
					oldHold = VPAD_BUTTON_LEFT;
					frameCount = 30;
					dpadAction = true;
				}
				else if(frameCount == 0)
					dpadAction = true;
				else
				{
					--frameCount;
					dpadAction = false;
				}

				if(dpadAction)
				{
					if(pos >= MAX_TITLEBROWSER_LINES)
						pos -= MAX_TITLEBROWSER_LINES;
					else
						pos = filteredTitleEntrySize - MAX_TITLEBROWSER_LINES;
					cursor = 0;
					redraw = true;
				}
			}
		}
		
		if(vpad.trigger & VPAD_BUTTON_X)
		{
			MEMFreeToDefaultHeap(filteredTitleEntries);
			if(!downloadMenu())
				titleBrowserMenu();
			return;
		}
		if(vpad.trigger & VPAD_BUTTON_MINUS)
		{
			titleQueue.emplace_back(filteredTitleEntries[cursor + pos]);
			std::sort(titleQueue.begin(), titleQueue.end());
    		titleQueue.erase(std::unique(titleQueue.begin(), titleQueue.end()), titleQueue.end());				
		}
		if(vpad.trigger & VPAD_BUTTON_PLUS)
		{
			for(TitleEntry *e : titleQueue) {
				debugPrintf("%s", e->name);
				predownloadMenu(e, false);
			}
				
		}
		if(vpad.trigger & VPAD_BUTTON_Y)
		{
			char oldSearch[sizeof(search)];
			strcpy(oldSearch, search);
			showKeyboard(KEYBOARD_LAYOUT_NORMAL, KEYBOARD_TYPE_NORMAL, search, CHECK_NONE, 128, false, search, "Search");
			if(strcmp(oldSearch, search) != 0)
			{
				cursor = pos = 0;
				redraw = true;
			}
		}
		
		if(vpad.trigger & VPAD_BUTTON_R || vpad.trigger & VPAD_BUTTON_ZR || vpad.trigger & VPAD_BUTTON_PLUS)
		{
			size_t tt = (size_t)tab;
			if(++tt > TITLE_CATEGORY_ALL)
				tt = (size_t)TITLE_CATEGORY_GAME;
			
			tab = (TITLE_CATEGORY)tt;
			cursor = pos = 0;
			redraw = true;
		}
		else if(vpad.trigger & VPAD_BUTTON_L || vpad.trigger & VPAD_BUTTON_ZL || vpad.trigger & VPAD_BUTTON_MINUS)
		{
			if(tab == TITLE_CATEGORY_GAME)
				tab = TITLE_CATEGORY_ALL;
			else
			{
				size_t tt = (size_t)tab;
				tt--;
				tab = (TITLE_CATEGORY)tt;
			}
			
			cursor = pos = 0;
			redraw = true;
		}
		
		if(oldHold && !(vpad.hold & (VPAD_BUTTON_UP | VPAD_BUTTON_DOWN | VPAD_BUTTON_LEFT | VPAD_BUTTON_RIGHT)))
			oldHold = 0;
		
		if(redraw)
		{
			drawTBMenuFrame(tab, pos, cursor, search);
			mov = filteredTitleEntrySize > MAX_TITLEBROWSER_LINES;
			redraw = false;
		}
	}
	if(!AppRunning())
	{
		MEMFreeToDefaultHeap(filteredTitleEntries);
		return;
	}
	
	predownloadMenu(entry, true);
	MEMFreeToDefaultHeap(filteredTitleEntries);
}
