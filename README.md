# Image Sequence Converter

A macOS and Windows application for converting between image sequences and videos, built with Qt6 and FFmpeg.

## Features

### Image Sequence → Video
- Supports JPG, PNG, TIFF, EXR, HDR, BMP
- Output formats: MP4, AVI, MOV, MKV, WebM
- Codec options: H.264, H.265, VP9, ProRes
- Adjustable frame rate, resolution, and CRF quality
- Aspect ratio preservation and progress logging

### Video → Image Sequence
- Extract frames as PNG, JPEG, TIFF, BMP, or EXR
- Extract all frames or a custom range
- Auto-numbered frame output

### User Interface
- Dark theme with a tabbed workflow
- Real-time log and progress bar
- Button to preview the full FFmpeg command before execution

## Requirements

- macOS 15.5 or later (Intel or Apple Silicon) / Windows 10 or later
- Dependencies: Qt6, FFmpeg, CMake
- NMake (comes with Visual Studio Build Tools) - for Windows

### Installation

## Recommended: One-Line Setup
```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

## Manual Setup
```
brew install qt6 cmake ffmpeg
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
make -j$(sysctl -n hw.ncpu)
open ImageSequenceConverter.app
```

## Windows Build & Run

### Requirements
- Windows 10 or later
- Qt6, CMake, FFmpeg (all must be in your PATH)
- NMake (comes with Visual Studio Build Tools)

### One-Line Setup (Windows)
```bat
build_and_run_windows.bat
```

### Manual Setup (Windows)
```bat
REM Open a Visual Studio Developer Command Prompt
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake
start ImageSequenceConverter.exe
```

## License
MIT License

-- Contributions are welcome via issues or pull requests. --
