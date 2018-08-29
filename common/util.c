/*
 * util.c
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
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <windows.h>
#include "common.h"

__declspec(noreturn) void
err(int status, const char *fmt, ...)
{
	va_list ap;
	int errno_saved;

	errno_saved = errno;

	if (fmt) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
		fputs(": ", stderr);
	}

	fprintf(stderr, "%s\n", strerror(errno_saved));
	exit(status);
}

__declspec(noreturn) void
errx(int status, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verrx(status, fmt, ap);
	va_end(ap);
}

__declspec(noreturn) void
errw(int status, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verrwc(status, GetLastError(), fmt, ap);
	va_end(ap);
}

__declspec(noreturn) void
errwc(int status, DWORD errcode, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verrwc(status, errcode, fmt, ap);
	va_end(ap);
}

__declspec(noreturn) void
verrx(int status, const char *fmt, va_list ap)
{
	if (fmt) {
		vfprintf(stderr, fmt, ap);
		fputc('\n', stderr);
	} else {
		fputs("error\n", stderr);
	}

	exit(status);
}

__declspec(noreturn) void
verrwc(int status, DWORD errcode, const char *fmt, va_list ap)
{
	DWORD dw;
	char buf[512];

	if (fmt) {
		vfprintf(stderr, fmt, ap);
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

	exit(status);
}

int
getline(char **s, size_t *cap, FILE *f)
{
	return getdelim(s, cap, '\n', f);
}

int
getdelim(char **s, size_t *cap, int delim, FILE *f)
{
	size_t newcap;
	char *news;
	size_t len = 0;
	int c;

	if (!*s || !*cap) {
		if (!(news = malloc(newcap = 128)))
			return -1;
		*cap = newcap;
		*s = news;
	}

	while ((c = fgetc(f)) != EOF) {
		if (len >= *cap - 2) {
			for (newcap = *cap; len >= newcap - 2; newcap *= 2)
				;
			if (!(news = realloc(*s, newcap)))
				return -1;
			*cap = newcap;
			*s = news;
		}

		(*s)[len++] = (char)c;
		if (c == delim)
			break;
	}

	return 0;
}

void
hexdump(FILE *f, void *buf, size_t n)
{
	size_t i, j;
	uint8_t *bytes;

	if (!n)
		return;

	bytes = buf;

	for (i = 0; i < n; i += 16) {
		for (j = i; j < n && j < i+16; j++) {
			if (j % 8 == 7)
				fprintf(f, "%02x  ", (unsigned)bytes[j]);
			else if (j % 2 == 1)
				fprintf(f, "%02x ", (unsigned)bytes[j]);
			else
				fprintf(f, "%02x", (unsigned)bytes[j]);
		}

		for (j = n; j < i+16; j++) {
			if (j % 8 == 7)
				fputs("    ", f);
			else if (j % 2 == 1)
				fputs("   ", f);
			else
				fputs("  ", f);
		}

		fputs("  ", f);

		for (j = i; j < n && j < i+16; j++) {
			if (isprint(bytes[j]))	
				fputc(bytes[j], f);
			else
				fputc('.', f);
		}

		fputc('\n', f);
	}
}
