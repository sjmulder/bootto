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
#include "../common/common.h"

__declspec(noreturn) void
fatal(const char *fmt, ...)
{
	va_list ap;

	if (fmt) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
		fputc('\n', stderr);
	} else {
		fputs("error\n", stderr);
	}

	fputs("press any key to quit\n", stderr);
	getchar();
	exit(1);
}

__declspec(noreturn) void
fatal_errno(int errcode, const char *fmt, ...)
{
	va_list ap;

	if (fmt) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
		fputs(": ", stderr);
	}

	fprintf(stderr, "%s\n", strerror(errcode));
	fputs("press any key to quit\n", stderr);
	getchar();
	exit(1);
}

__declspec(noreturn) void
fatal_win32(DWORD errcode, const char *fmt, ...)
{
	DWORD dw;
	char buf[512];
	va_list ap;

	if (fmt) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
		fputs(": ", stderr);
	}

	dw = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buf,
	    sizeof(buf), NULL);
	if (dw)
		fputs(buf, stderr);
	else
		fprintf(stderr, "error %d\n", (int)dw);

	fputs("press any key to quit\n", stderr);
	getchar();
	exit(1);
}
