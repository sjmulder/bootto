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

#ifndef noreturn
#define noreturn __declspec(noreturn)
#endif
 
#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define EFI_ACTIVE 1

struct bootentry {
	uint16_t id;	
	size_t len;
	char data[512];
};

/* to be implemented by user */
noreturn void	 fatal(const char *, ...);
noreturn void	 fatal_errno(int, const char *, ...);
noreturn void	 fatal_win32(DWORD, const char *, ...);

void		 os_setup(void);
void		 os_reboot(wchar_t *desc);

size_t		 efi_getentries(struct bootentry *, size_t);
uint16_t	 efi_getflags(struct bootentry *);
wchar_t		*efi_getdesc(struct bootentry *);
uint16_t	 efi_getbootcur(void);
void		 efi_setbootnext(uint32_t);
