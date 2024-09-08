tcc -o sdl2-c-rpg-tcc.exe src/main.c -Iinclude -L. -lSDL2 -lmylib -DSDLCALL="" && (
    sdl2-c-rpg-tcc.exe
) || (
    echo Compilation failed. Please check for errors.
    pause
)