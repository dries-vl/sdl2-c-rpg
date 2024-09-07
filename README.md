# tcc compile
tcc -o sdl2-c-rpg-tcc.exe src/main.c -Iinclude -L. -lSDL2 -lmylib -DSDLCALL=""

# g++ compile
g++ -o sdl2-c-rpg-g++.exe src/main.c -Iinclude -Llib -lSDL2 -lmylib

