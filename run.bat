@echo off
cls
gcc main.c -Iinclude -Llib -lraylib -lgdi32 -limm32 -lwinmm -o main.exe
main.exe
