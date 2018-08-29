/*
 * common.h
 * Copyright (c) 2018, Sijmen J. Mulder <ik@sjmulder.nl>
 *
 * This file is part of Boot To.
 *
 * Boot To is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.

 * Boot To is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Boot To. If not, see <https://www.gnu.org/licenses/>.
 */
 
#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define EFI_ACTIVE 1

struct bootentry {
	uint16_t id;	
	size_t len;
	char data[512];
};

/* to be implemented by user */
__declspec(noreturn) void fatal(const char *, ...);
__declspec(noreturn) void fatal_errno(int, const char *, ...);
__declspec(noreturn) void fatal_win32(DWORD, const char *, ...);

/* no ssize_t on Windows, so return is -1 (error) or 0 */
int		 getline(char **, size_t *, FILE *);
int		 getdelim(char **, size_t *, int, FILE *);

void		 hexdump(FILE *, void *, size_t);

void		 os_setup(void);
void		 os_reboot(wchar_t *desc);

size_t		 efi_getentries(struct bootentry *, size_t);
uint16_t	 efi_getflags(struct bootentry *);
wchar_t		*efi_getdesc(struct bootentry *);
uint16_t	 efi_getbootcur(void);
void		 efi_setbootnext(uint32_t);
