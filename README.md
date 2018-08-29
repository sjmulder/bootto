# Boot To

A reboot menu for Windows:

    1) Windows Boot Manager
    2) Fedora Linux 28
    3) UEFI: JetFlashTranscend 16GB 1.00, Partition 1
    boot to? [1]

When an option is selected and confirmed, the system reboots into the selected
operating system once. (The default operating system is left unchanged.)

This is a prerelease version. See _Known issues_ and _Future work_ below.

## Installation

Boot To requires Windows 64 bit in UEFI mode. This is the default on most
modern machines.
 
 1. Download and install the [Visual C++ 2017 redistributable (x86)](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).
 2. Download and unpack a release from the _Releases_ page.

To build from source, install the Visual Studio 2017 tools and type `nmake`
in the project root directory.

## Known issues

 * Only static UEFI entries are shown, not removable media and so on.

## Future work

 * Ports to Linux, FreeBSD and macOS. (It appears that NetBSD and OpenBSD do
   not yet have the necessary APIs.)
 * Improved CLI with curses.
 * GUIs for GTK and macOS also.
 * Windows UAP package.
 * Applets for the Windows task bar, the macOS menu bar, and common X desktop
   environments.

## Legal

Copyright (c) 2018, Sijmen J. Mulder <ik@sjmulder.nl>

Boot To is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

Boot To is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
more details.

You should have received a copy of the GNU Affero General Public License
along with Boot To. If not, see <https://www.gnu.org/licenses/>.
