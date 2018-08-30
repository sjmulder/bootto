/*
 * fatal.c
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

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <windows.h>
#include <tchar.h>
#include "../common/common.h"

#define LEN(a) (sizeof(a)/sizeof(*(a)))

/* static to spare the stack if it ran out */
static char errstr[512];	/* e.g. "No such file or directory" */
static char reason[512];	/* e.g. "Error reading foo.txt" */
static char message[512];	/* e.g. "Error reading foo.txt: No such..." */

noreturn void
fatal(const char *fmt, ...)
{
	va_list ap;

	if (fmt) {
		va_start(ap, fmt);
		vsnprintf(reason, LEN(reason), fmt, ap);
		va_end(ap);

		snprintf(message, LEN(message), "A critical error occured:"
		    "\r\n\r\n%s", reason);

		MessageBoxA(NULL, message, "Boot To", MB_OK |
		    MB_ICONEXCLAMATION);
	} else {
		MessageBoxA(NULL, "A critical internal error occured.",
		    "Boot To", MB_OK | MB_ICONEXCLAMATION);
	}

	exit(1);
}

noreturn void
fatal_errno(int errcode, const char *fmt, ...)
{
	va_list ap;

	if (fmt) {
		va_start(ap, fmt);
		vsnprintf(reason, LEN(reason), fmt, ap);
		va_end(ap);

		snprintf(message, LEN(message), "A critical error occured:"
		    "\r\n\r\n%s: %s", reason, strerror(errcode));

	} else {
		snprintf(message, LEN(message), "A critical error occured:"
		    "\r\n\r\n%s", strerror(errcode));
	}

	MessageBoxA(NULL, message, "Boot To", MB_OK |
	    MB_ICONEXCLAMATION);
	exit(1);
}

noreturn void
fatal_win32(DWORD errcode, const char *fmt, ...)
{
	va_list ap;
	DWORD dw;

	dw = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0,
	    errstr, LEN(errstr), NULL);
	if (!dw)
		snprintf(errstr, LEN(errstr), "Error %d", (int)dw);

	if (fmt) {
		va_start(ap, fmt);
		vsnprintf(reason, LEN(reason), fmt, ap);
		va_end(ap);

		snprintf(message, LEN(message), "A critical error occured:"
		    "\r\n\r\n%s: %s", reason, errstr);

	} else {
		snprintf(message, LEN(message), "A critical error occured:"
		    "\r\n\r\n%s", errstr);
	}

	MessageBoxA(NULL, message, "Boot To", MB_OK |
	    MB_ICONEXCLAMATION);

	exit(1);
}
