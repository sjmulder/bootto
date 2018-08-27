/*
 * bootto.c
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
#include <windows.h>
#include "bootto.h"

/* returns a 0-based index [0..<num] */
static size_t
prompt_bootnext(size_t num, size_t defval)
{
	char *line = NULL;
	char *end;
	size_t cap;
	long l;

	while (1) {
		printf("boot to? [%zd] ", defval+1);
		if (getline(&line, &cap, stdin) == -1)
			err(1, NULL);
		if (!line[0] || line[0] == '\n') {
			free(line);
			return defval;
		}

		l = strtol(line, &end, 10);
		if (*end && *end != '\n')
			puts("please enter a number");
		else if (l < 1 || (size_t)l > num)
			puts("invalid index");
		else {
			free(line);
			return (size_t)l-1;
		}
	}
}

/* returns 1 if confirmed, 0 if not */
static int
prompt_reboot(wchar_t *selection)
{
	char *line = NULL;
	size_t cap;

	while (1) {
		printf("reboot to \"%S\"? (y/n) ", selection);
		if (getline(&line, &cap, stdin) == -1)
			err(1, NULL);

		if (line[0] == 'y') {
			free(line);
			return 1;
		} else if (line[0] == 'n') {
			free(line);
			return 0;
		} else
			puts("please enter y or n");
	}
}

int
main(int argc, char **argv)
{
	struct bootentry entries[32];
	size_t num;
	size_t i;
	uint32_t cur_id;
	size_t cur_idx = 0;
	size_t next_idx;

	os_setup();

	num = efi_getentries(entries, LEN(entries));
	if (!num)
		errx(1, "no boot options");

	for (i = 0; i < num; i++) {
		printf("%zd) ", i+1);
		if (!(efi_getflags(&entries[i]) & EFI_ACTIVE))
			printf("(Inactive) ");
		printf("%S\n", efi_getdesc(&entries[i]));
	}

	cur_id = efi_getbootcur();

	while (entries[cur_idx].id != cur_id)
		if (++cur_idx >= num)
			errx(1, "current boot option missing");

	do {
		next_idx = prompt_bootnext(num, cur_idx);
	} while (!prompt_reboot(efi_getdesc(&entries[next_idx])));

	efi_setbootnext(entries[next_idx].id);
	os_reboot(efi_getdesc(&entries[next_idx]));

	return 0;
}
