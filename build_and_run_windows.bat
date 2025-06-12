@echo off
REM ImageSequenceConverter - Build and Run Script for Windows

REM Check for Qt6, CMake, and FFmpeg in PATH
where qt6core.dll >nul 2>nul || (
    echo [ERROR] Qt6 not found in PATH. Please install Qt6 and add it to your PATH.
    exit /b 1
)
where cmake >nul 2>nul || (
    echo [ERROR] CMake not found in PATH. Please install CMake and add it to your PATH.
    exit /b 1
)
where ffmpeg >nul 2>nul || (
    echo [ERROR] FFmpeg not found in PATH. Please install FFmpeg and add it to your PATH.
    exit /b 1
)

REM Create build directory
if exist build (
    echo [INFO] Cleaning build directory...
    rmdir /s /q build
)
mkdir build
cd build

REM Configure with CMake
cmake .. -G "NMake Makefiles" || (
    echo [ERROR] CMake configuration failed.
    exit /b 1
)

REM Build the project
nmake || (
    echo [ERROR] Build failed.
    exit /b 1
)

REM Run the application
if exist ImageSequenceConverter.exe (
    echo [SUCCESS] Build completed successfully!
    start ImageSequenceConverter.exe
) else (
    echo [ERROR] Executable not found after build.
    exit /b 1
)
