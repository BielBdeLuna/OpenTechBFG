```
 _____                _____         _     _____            _            
|  _  |              |_   _|       | |   |  ___|          (_)           
| | | |_ __   ___ _ __ | | ___  ___| |__ | |__ _ __   __ _ _ _ __   ___ 
| | | | '_ \ / _ | '_ \| |/ _ \/ __| '_ \|  __| '_ \ / _` | | '_ \ / _ \
\ \_/ | |_) |  __| | | | |  __| (__| | | | |__| | | | (_| | | | | |  __/
 \___/| .__/ \___|_| |_\_/\___|\___|_| |_\____|_| |_|\__, |_|_| |_|\___|
      | |                                             __/ |             
      |_|                                            |___/ 
```

[![Build Status](https://travis-ci.org/OpenTechEngine/OpenTechBFG.svg?branch=projectLayout)](https://travis-ci.org/OpenTechEngine/OpenTechBFG)
[![Coverity Scan](https://scan.coverity.com/projects/4341/badge.svg)](https://scan.coverity.com/projects/4341)

Thank you for downloading OpenTechEngine.

OpenTechEngine is idTech based engine, forked from RBDOOM-3-BFG.

_______________________________________

CONTENTS
_______________________________



This file contains the following sections:

	1) SYSTEM REQUIREMENT

	2) GENERAL NOTES
	
	3) LICENSE
	
	4) GETTING THE SOURCE CODE

	5) COMPILING ON WIN32 WITH VISUAL C++ 2013 EXPRESS EDITION

	6) COMPILING ON GNU/LINUX
	
	7) INSTALLATION, GETTING THE GAMEDATA, RUNNING THE GAME
	
	8) CHANGES
	
	9) CONSOLE VARIABLES
	
	10) KNOWN ISSUES
	
	11) BUG REPORTS
	
	12) CODE LICENSE EXCEPTIONS



___________________________________

1) SYSTEM REQUIREMENTS
__________________________



Minimum system requirements:

	CPU: 2 GHz Intel compatible
	System Memory: 512MB
	Graphics card: Any graphics card that supports Direct3D 10 and OpenGL >= 3.2

Recommended system requirements:

	CPU: 3 GHz + Intel compatible
	System Memory: 1024MB+
	Graphics card: Geforce 9600 GT, ATI HD 5650 or higher. 




_______________________________

2) GENERAL NOTES
______________________

This release does not contain any game data, the game data is still
covered by the original EULA and must be obeyed as usual.

You must patch the game to the latest version.

Note that Doom 3 BFG Edition is available from the Steam store at
http://store.steampowered.com/app/208200/


Steam:
------
The Doom 3 BFG Edition GPL Source Code release does not include functionality for integrating with 
Steam.  This includes roaming profiles, achievements, leaderboards, matchmaking, the overlay, or
any other Steam features.


Bink:
-----
The OpenTechEngine GPL Source Code release includes functionality for rendering Bink Videos through FFmpeg.


Back End Rendering of Stencil Shadows:
--------------------------------------

The Doom 3 BFG Edition GPL Source Code release does not include functionality enabling rendering
of stencil shadows via the "depth fail" method, a functionality commonly known as "Carmack's Reverse".


Mods:
-----

The Doom 3 BFG Edition GPL Source Code release allow mod editing, in order for it to accept any change in your
mod directory, you should first specify your mod directory adding the following command to the launcher:

"+set fs_game modDirectoryName"

as well as force the content of your mod directory over the content of the game with the following command:

"+set fs_resourceLoadPriority 0"

so it would end up looking like: OpenTechEngine +set fs_resourceLoadPriority 0 +set fs_game mymod


_______________________________

3) LICENSE
______________________


See COPYING.txt for the GNU GENERAL PUBLIC LICENSE

ADDITIONAL TERMS:  The Doom 3 BFG Edition GPL Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU GPL which accompanied the Doom 3 BFG Edition GPL Source Code.  If not, please request a copy in writing from id Software at id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.


____________________________________________

4) GETTING THE SOURCE CODE
___________________________________

This project's GitHub.net Git repository can be checked out through Git with the following instruction set: 

	> git clone https://github.com/OpenTechEngine/OpenTechBFG.git

If you don't want to use git, you can download the source as a zip file at
	https://github.com/OpenTechEngine/OpenTechBFG/archive/master.zip



___________________________________________________________________

5) COMPILING ON WIN32 WITH VISUAL C++ 2013 EXPRESS EDITION
__________________________________________________________

1. Download and install the Visual C++ 2013 Express Edition.

2. Download the DirectX SDK (June 2010) here:
	http://www.microsoft.com/en-us/download/details.aspx?id=6812

3. Download and install the latest CMake.

4. Generate the VC13 projects using CMake by doubleclicking a matching configuration .bat file in the neo/ folder.

5. Use the VC13 solution to compile what you need:
	OpenTechEngine/build/OpenTechEngine.sln
	
6. Download ffmpeg-20140405-git-ec8789a-win32-shared.7z from ffmpeg.zeranoe.com/builds/win32/shared

7. Extract the FFmpeg DLLs to your current build directory under OpenTechEngine/build/


__________________________________

6) COMPILING ON GNU/LINUX
_________________________


1. You need the following dependencies in order to compile OpenTechEngine with all features:
 
	On Debian or Ubuntu:

		> apt-get install cmake libsdl1.2-dev libopenal-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
	
	On Fedora

		// TODO add ffmpeg libs for bink videos
		
		> yum install cmake SDL-devel openal-devel
	
	On openSUSE (tested in 13.1)
	
		> zypper in openal-soft-devel cmake libSDL-devel libffmpeg1-devel
	
		For SDL 2 replace "libSDL-devel" with "libSDL2-devel".
		"libffmpeg1-devel" requires the PackMan repository. If you don't have that repo, and don't want to add it, remove the "libffmpeg1-devel" option and compile without ffmpeg support.
		If you have the repo and compiles with ffmpeg support, make sure you download "libffmpeg1-devel", and not "libffmpeg-devel".
	
	Instead of SDL1.2 development files you can also use SDL2. Install SDL 2.0 and add to the cmake parameters -DSDL2=ON
	
	SDL2 has better input support (especially in the console) and better 
	support for multiple displays (especially in fullscreen mode).


2. Generate the Makefiles using CMake:

	> cd neo/
	> ./cmake-eclipse-linux-profile.sh
	
3. Compile OpenTechEngine targets with

	> cd ../build
	> make

___________________________________________________

7) INSTALLATION, GETTING THE GAMEDATA, RUNNING THE GAME
__________________________________________


If you use the prebuilt Win32 binaries then simply extract them to your
C:\Program Files (x86)\Steam\SteamApps\common\Doom 3 BFG Edition\ directory and run OpenTechEngine.exe.




The following instructions are primarily intented for Linux users and all hackers on other operating systems.

To play the game, you need the game data from a legal copy of the game, which 
unfortunately requires Steam for Windows - Steam for Linux or OSX won't do, because
(at least currently) the Doom 3 BFG game is only installable on Steam for Windows.
Even the DVD version of Doom 3 BFG only contains encrytped data that is decoded
by Steam on install.

Fortunately, you can run Steam in Wine to install Doom3 BFG and afterwards copy the 
game data somewhere else to use it with native executables.
Winetricks ( http://winetricks.org/ ) makes installing Windows Steam on Linux really easy.

Anyway:

1. Install Doom 3 BFG in Steam (Windows version), make sure it's getting 
   updated/patched.

2. Create your own Doom 3 BFG directory, e.g. /path/to/Doom3BFG/

3. Copy the game-data's base dir from Steam to that directory 
   (e.g. /path/to/Doom3BFG/), it's in
	/your/path/to/Steam/steamapps/common/DOOM 3 BFG Edition/base/

4. Copy your OpenTechEngine executable that you created in 5) or 6) and the FFmpeg DLLs to your own 
   OpenTechEngine directory (/path/to/OpenTechEngine/).
   
   Your own Doom 3 BFG directory now should look like:
	/path/to/Doom3BFG/
	 ->	OpenTechEngine (or OpenTechEngine.exe on Windows)
	 -> avcodec-55.dll
	 -> avdevice-55.dll
	 -> avfilter-4.dll
	 -> avformat-55.dll
	 -> avutil-52.dll
	 -> postproc-52.dll
	 -> swresample-0.dll
	 -> swscale-2.dll
	 ->	base/
		 ->	classicmusic/
		 ->	_common.crc
		 ->	(etc)

5. Run the game by executing the OpenTechEngine executable.

6. Enjoy

7. If you run into bugs, please report them, see 11)

___________________________________________________

8) CHANGES
__________________________________________

- Flexible build system using CMake

- Linux support (32 and 64 bit)

- Win64 support

- OS X support

- OpenAL Soft sound backend primarily developed for Linux but works on Windows as well

- Bink video support through FFmpeg

- Soft shadows using PCF hardware shadow mapping

	The implementation uses sampler2DArrayShadow and PCF which usually
	requires Direct3D 10.1 however it is in the OpenGL 3.2 core so it should
	be widely supported.
	All 3 light types are supported which means parallel lights (sun) use
	scene independent cascaded shadow mapping.
	The implementation is very fast with single taps (400 fps average per
	scene on a GTX 660 ti OC) however I defaulted it to 16 taps so the shadows look
	really good which should give you stable 100 fps on todays hardware (2014).

- Changed light interaction shaders to use Half-Lambert lighting like in Half-Life 2 to 
	make the game less dark. https://developer.valvesoftware.com/wiki/Half_Lambert


___________________________________________________

9) CONSOLE VARIABLES
__________________________________________

r_useShadowMapping 1 - Use soft shadow mapping instead of hard stencil shadows


___________________________________________________

10) KNOWN ISSUES
__________________________________________

Doom 3 wasn't designed to work with shadow maps so:

- Some lights cause shadow acne with shadow mapping
- Some shadows might almost disappear due to the shadow filtering

___________________________________________________

11) BUG REPORTS
__________________________________________

OpenTechEngine is not perfect, it is not bug free as every other software.
For fixing as much problems as possible we need as much bug reports as possible.
We cannot fix anything if we do not know about the problems.

The best way for telling us about a bug is by submitting a bug report at our GitHub bug tracker page:

	https://github.com/OpenTechEngine/OpenTechBFG/issues?state=open
	And our fork source at
	https://github.com/RobertBeckebans/RBDOOM-3-BFG/issues?state=open

The most important fact about this tracker is that we cannot simply forget to fix the bugs which are posted there. 
It is also a great way to keep track of fixed stuff.

If you want to report an issue with the game, you should make sure that your report includes all information useful to characterize and reproduce the bug.

    * Search on Google
    * Include the computer's hardware and software description ( CPU, RAM, 3D Card, distribution, kernel etc. )
    * If appropriate, send a console log, a screenshot, an strace ..
    * If you are sending a console log, make sure to enable developer output:

              OpenTechEngine.exe +set developer 1 +set logfile 2

NOTE: We cannot help you with OS-specific issues like configuring OpenGL correctly, configuring ALSA or configuring the network.
	

	
	
____________________________________________________________________________________

12) CODE LICENSE EXCEPTIONS - The parts that are not covered by the GPL:
_______________________________________________________________________


EXCLUDED CODE:  The code described below and contained in the Doom 3 BFG Edition GPL Source Code release
is not part of the Program covered by the GPL and is expressly excluded from its terms. 
You are solely responsible for obtaining from the copyright holder a license for such code and complying with the applicable license terms.

Base64 implementation
---------------------------------------------------------------------------
neo/idlib/Base64.cpp

Copyright (c) 1996 Lars Wirzenius.  All rights reserved.

June 14 2003: TTimo <ttimo@idsoftware.com>
	modified + endian bug fixes
	http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=197039

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

MD4 Message-Digest Algorithm
-----------------------------------------------------------------------------
neo/idlib/hashing/MD4.cpp
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD4 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD4 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

MD5 Message-Digest Algorithm
-----------------------------------------------------------------------------
neo/idlib/hashing/MD5.cpp
This code implements the MD5 message-digest algorithm.
The algorithm is due to Ron Rivest.  This code was
written by Colin Plumb in 1993, no copyright is claimed.
This code is in the public domain; do with it what you wish.

CRC32 Checksum
-----------------------------------------------------------------------------
neo/idlib/hashing/CRC32.cpp
Copyright (C) 1995-1998 Mark Adler

OpenGL headers
---------------------------------------------------------------------------
neo/renderer/OpenGL/glext.h
neo/renderer/OpenGL/wglext.h

Copyright (c) 2007-2012 The Khronos Group Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and/or associated documentation files (the
"Materials"), to deal in the Materials without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Materials, and to
permit persons to whom the Materials are furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Materials.

THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

Bundled libraries
---------------------------------------------------------------------------
Please see sub-directories under libs/ for bundled library license information.
