/***************************************************************************
 * This file is part of NUSspli.                                           *
 * Copyright (c) 2020-2022 V10lator <v10lator@myway.de>                    *
 * Copyright (c) 2016 dimok789                                             *
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

#include <coreinit/memory.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <iosuhax.h>

#include <crypto.h>
#include <iosuhaxx.h>
#include <otp.h>
#include <utils.h>

static uint8_t otp_common_key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t *getCommonKey()
{
	if(otp_common_key[0] == 0x00)
	{
		OSTime t = OSGetSystemTime();
		if(openIOSUhax())
		{
			uint8_t buf[(0x38 * 4) + 16];
			if(IOSUHAX_read_otp(buf, (0x38 * 4) + 16) >= 0)
			{
				OSBlockMove(otp_common_key, buf + (0x38 * 4), 16, false);

				t = OSGetSystemTime() - t;
				addEntropy(&t, sizeof(OSTime));

#ifdef NUSSPLI_DEBUG
				char ret[33];
				char *tmp = &ret[0];
				for(int i = 0; i < 16; i++, tmp += 2)
					sprintf(tmp, "%02x", otp_common_key[i]);
				debugPrintf("Common key: %s", ret);
#endif
			}
			else
				debugPrintf("Common key: IOSUHAX_read_otp() failed!");
		}
		else
			debugPrintf("Common key: openIOSUhax() failed!");
	}

	return otp_common_key;
}
