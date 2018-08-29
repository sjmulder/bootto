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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "common.h"

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
