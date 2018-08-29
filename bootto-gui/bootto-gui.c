/*
 * bootto-gui.c
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
#include <commctrl.h>
#include "../common/common.h"

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int cmdshow)
{
	struct bootentry entries[32];
	size_t num;
	size_t i;
	TASKDIALOGCONFIG dialog;
	TASKDIALOG_BUTTON radios[32];
	TASKDIALOG_BUTTON buttons[1];
	int button;
	int radio;
	HRESULT hr;

	os_setup();

	num = efi_getentries(entries, LEN(entries));
	if (!num)
		fatal("No boot options are configured.");

	for (i = 0; i < num; i++) {
		radios[i].nButtonID = (int)i;
		radios[i].pszButtonText = efi_getdesc(&entries[i]);
	}

	buttons[0].nButtonID = IDOK;
	buttons[0].pszButtonText = L"&Reboot";
	
	memset(&dialog, 0, sizeof(dialog));
	dialog.cbSize = sizeof(dialog);
	dialog.dwFlags = TDF_SIZE_TO_CONTENT;
	dialog.dwCommonButtons = TDCBF_CANCEL_BUTTON;
	dialog.pszWindowTitle = L"Boot To";
	dialog.pszMainInstruction = L"Choose a boot option";
	dialog.pszContent =
	    L"Your computer will reboot into the selected boot option\r\n"
	    L"once. No permanent changes are made.";
	dialog.cButtons = 1;
	dialog.pButtons = buttons;
	dialog.cRadioButtons = (UINT)num;
	dialog.pRadioButtons = radios;

	hr = TaskDialogIndirect(&dialog, &button, &radio, NULL);
	if (!SUCCEEDED(hr))
		fatal("TaskDialogIndirect() failed");

	if (button == IDOK) {
		efi_setbootnext(entries[radio].id);
		os_reboot(efi_getdesc(&entries[radio]));
	}

	return 0;
}
