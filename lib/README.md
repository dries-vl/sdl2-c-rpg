Tiny C Compiler (tcc) uses the DLL file directly, and cannot use the .dll.a file
(gcc does need this file)
# Command to compile with tcc: 
tcc -o sdl2-c-rpg-tcc.exe src/main.c -Iinclude -L. -lSDL2 -DSDLCALL=""

# -L. => libs folder is this folder (sdl2 dll file is in there)
# -DSDLCALL="" => because tcc cannot handle some platform-dependent stuff this needs to be set (?)