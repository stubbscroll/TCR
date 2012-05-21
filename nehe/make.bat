@REM to build a program: MAKE src. example: MAKE 01
gcc %1.c -o %1.exe -lmingw32 -lsdlmain -lsdl -lopengl32 -lglu32
