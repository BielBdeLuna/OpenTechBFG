astyle.exe -v --formatted --options=astyle-options.ini --recursive "../neo/*.h"
astyle.exe -v --formatted --options=astyle-options.ini --exclude="d3xp/gamesys/SysCvar.cpp" --exclude="d3xp/gamesys/Callbacks.cpp" --exclude="sys/win32/win_cpu.cpp" --exclude="sys/win32/win_main.cpp" --recursive "../neo/*.cpp"

pause