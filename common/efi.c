/*
 * efi.c
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

const char ns_guid[] = "{8be4df61-93ca-11d2-aa0d-00e098032b8c}";

static uint16_t
getu16(void *p)
{
	uint8_t *bytes;

	bytes = p;
	return (uint16_t)bytes[0] | (uint16_t)bytes[1] << 8;
}

static uint32_t
getu32(void *p)
{
	uint8_t *bytes;

	bytes = p;
	return
	     (uint32_t)bytes[0] |
	     (uint32_t)bytes[1] << 8 |
	     (uint32_t)bytes[2] << 16 |
	     (uint32_t)bytes[3] << 24;
}

static void
setu32(void *p, uint32_t val)
{
	uint8_t *bytes;

	bytes = p;
	bytes[0] = (uint8_t)(val >> 24);
	bytes[1] = (uint8_t)(val >> 16 & 0xFF);
	bytes[2] = (uint8_t)(val >> 8 & 0xFF);
	bytes[3] = (uint8_t)(val & 0xFF);
}

size_t
efi_getentries(struct bootentry *arr, size_t sz)
{
	DWORD dw;
	char order[512];
	char entryvar[9];
	size_t num;
	size_t i;

	dw = GetFirmwareEnvironmentVariableA("BootOrder", ns_guid, order,
	    sizeof(order));
	if (!dw)
		fatal_win32(GetLastError(), "BootOrder");

	if ((num = dw / 2) > sz)
		num = sz;

	for (i = 0; i < num; i++) {
		arr[i].id = getu16(&order[i*2]);

		snprintf(entryvar, sizeof(entryvar), "Boot%04x", arr[i].id);
		dw = GetFirmwareEnvironmentVariableA(entryvar, ns_guid,
		    arr[i].data, sizeof(arr[i].data));
		if (!dw)
			fatal_win32(GetLastError(), "%s", entryvar);

		arr[i].len = (size_t)dw;
	}

	return num;
}

uint16_t
efi_getflags(struct bootentry *entry)
{
	return getu16(&entry->data[0]);	
}

wchar_t *
efi_getdesc(struct bootentry *entry)
{
	return (wchar_t *)&entry->data[6];
}

uint16_t
efi_getbootcur(void)
{
	char data[4];
	DWORD dw;

	dw = GetFirmwareEnvironmentVariableA("BootCurrent", ns_guid, data,
	    sizeof(data));
	if (!dw)
		fatal_win32(GetLastError(), "BootCurrent");

	return getu32(data);
}

void
efi_setbootnext(uint32_t idx)
{
	char data[4];
	DWORD dw;

	setu32(data, idx);
	dw = SetFirmwareEnvironmentVariableA("BootNext", ns_guid, data,
	    sizeof(data));
	if (!dw)
		fatal_win32(GetLastError(), "BootNext");
}
