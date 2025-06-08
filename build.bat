@echo off
echo Building Wave OS Installer...

REM Check if GTK4 development libraries are available
pkg-config --exists gtk4
if %errorlevel% neq 0 (
    echo Error: GTK4 development libraries not found.
    echo Please install GTK4 development libraries first.
    echo.
    echo On Windows with MSYS2:
    echo   pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-toolchain
    echo.
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" mkdir build
if not exist "build\screens" mkdir build\screens

REM Get GTK4 compile flags
for /f "delims=" %%i in ('pkg-config --cflags gtk4') do set GTK_CFLAGS=%%i
for /f "delims=" %%i in ('pkg-config --libs gtk4') do set GTK_LIBS=%%i

REM Compile object files
echo Compiling source files...
gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c main.c -o build\main.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c installer.c -o build\installer.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\welcome.c -o build\screens\welcome.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\language.c -o build\screens\language.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\timezone.c -o build\screens\timezone.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\keyboard.c -o build\screens\keyboard.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\disk.c -o build\screens\disk.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\network.c -o build\screens\network.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\user.c -o build\screens\user.o
if %errorlevel% neq 0 goto error

gcc -Wall -Wextra -std=c11 -g %GTK_CFLAGS% -c screens\install.c -o build\screens\install.o
if %errorlevel% neq 0 goto error

REM Link executable
echo Linking executable...
gcc build\main.o build\installer.o build\screens\welcome.o build\screens\language.o build\screens\timezone.o build\screens\keyboard.o build\screens\disk.o build\screens\network.o build\screens\user.o build\screens\install.o -o wave-installer.exe %GTK_LIBS%
if %errorlevel% neq 0 goto error

echo.
echo Build successful! Run wave-installer.exe to start the installer.
echo.
pause
exit /b 0

:error
echo.
echo Build failed with errors.
echo.
pause
exit /b 1
