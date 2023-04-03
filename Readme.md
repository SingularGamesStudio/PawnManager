# PawnManager

## Overview
PawnManager is Real-Time Strategy game where you compete with other player via clever use of Tasks: the only tool to command your lazy Pawns to do something, you will start with only a few building and a couple of Pawns, but develop and soon you will find yourself in the middle of thriving civilization. But beware, other players want to have your items! 

#### Develop, build, craft, defend! And become the best PawnManager!

## Requirements
MinGW w64 (with MSVCRT, UCRT will not work)
## Building
Building is done using CMake. At first you should generate makefiles. This is done using the following command:
```
cmake . -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=path/to/make -DCMAKE_C_COMPILER=path/to/gcc -DCMAKE_CXX_COMPILER=path/to/g++
```
where path/to/make, path/to/gcc and path/to/g++ are your paths to make, gcc and g++

Example of command, where mingw is installed to C:/mingw64:
```
cmake . -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=C:/mingw64/bin/mingw32-make.exe -DCMAKE_C_COMPILER=C:/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/mingw64/bin/g++.exe
```
Next you should use make to build the project. You should run make in project directory.

Example of command, where mingw is installed to C:/mingw64:
```
C:/mingw64/bin/mingw32-make.exe
```

After running these commands the resulting build files should be in project root directory. Remember that all dlls which are put into directory as well as libgcc_s_seh-1.dll, libstdc++-6.dll and libwinpthread-1 are required to be in same directory as the game in order to run it.

## Structure
Entity <- Pawn, Building, ResourceEntity

Pawn <- WorkerPawn, FighterPawn

Building <- CraftBuilding 

RecipeEntity is a part of Recipe (not inheritance)

(That's it for CP1, but Server Part is currently developed in /dev)
