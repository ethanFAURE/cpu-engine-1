mkdir cpulib
mkdir cpulib\include
mkdir cpulib\lib
del cpulib\include\*.h
del cpulib\lib\*.lib
copy vs\cpu-core\*.h cpulib\include\
copy vs\cpu-render\*.h cpulib\include\
copy vs\cpu-engine\*.h cpulib\include\
del cpulib\include\pch.h
copy vs\x64\Debug\cpu-core.lib cpulib\lib\cpu-core-debug.lib
copy vs\x64\Debug\cpu-render.lib cpulib\lib\cpu-render-debug.lib
copy vs\x64\Debug\cpu-engine.lib cpulib\lib\cpu-engine-debug.lib
copy vs\x64\Release\cpu-core.lib cpulib\lib\
copy vs\x64\Release\cpu-render.lib cpulib\lib\
copy vs\x64\Release\cpu-engine.lib cpulib\lib\
