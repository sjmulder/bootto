/*
 * os.c
 * Copyright (c) 2018, Sijmen J. Mulder <ik@sjmulder.nl>
 *
 * This file is part of Boot To.
 *
 * Boot To is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Boot To is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Boot To. If not, see <https://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "common.h"

void
os_setup(void)
{
	BOOL ok;
	DWORD errcode;
	TOKEN_PRIVILEGES *tp;
	HANDLE token;
	
	tp = malloc(sizeof(TOKEN_PRIVILEGES) + sizeof(LUID_AND_ATTRIBUTES));
	if (!tp)
		err(1, NULL);

	tp->PrivilegeCount = 2;
	tp->Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp->Privileges[1].Attributes = SE_PRIVILEGE_ENABLED;

	ok = LookupPrivilegeValue(NULL, SE_SYSTEM_ENVIRONMENT_NAME,
	    &tp->Privileges[0].Luid);
	if (!ok)
		errw(1, "LookupPrivilege(SE_SYSTEM_ENVIRONMENT_NAME)");

	ok = LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
	    &tp->Privileges[1].Luid);
	if (!ok)
		errw(1, "LookupPrivilege(SE_SHUTDOWN_NAME)");

	ok = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
	    TOKEN_QUERY, &token);
	if (!ok)
		errw(1, "OpenProcessToken()");

	ok = AdjustTokenPrivileges(token, FALSE, tp, 0, NULL, NULL);
	if (!ok)
		errw(1, "AdjustTokenPrivileges()");
	else if ((errcode = GetLastError()) == ERROR_NOT_ALL_ASSIGNED)
		errwc(1, errcode, "AdjustTokenPrivileges()");

	CloseHandle(token);
	free(tp);
}

void
os_reboot(wchar_t *desc)
{
	char msg[512];

	snprintf(msg, sizeof(msg), "Boot To is rebooting into %S", desc);

	if (!(InitiateSystemShutdownA(NULL, msg, 0, FALSE, TRUE)))
		errw(1, "InitiateSystemShutdownA()");
}
