# Image Sequence Converter

A cross-platform (macOS & Windows) application for converting between image sequences and videos, built with Qt6 and FFmpeg.

## Features

### Image Sequence → Video
- Supports JPG, PNG, TIFF, EXR, HDR, BMP
- Output formats: MP4, AVI, MOV, MKV, WebM
- Codec options: H.264, H.265, VP9, ProRes
- Adjustable frame rate, resolution, and CRF quality
- Aspect ratio preservation
- Export Audio option
- Real-time progress logging

### Video → Image Sequence
- Extract frames as PNG, JPEG, TIFF, BMP, or EXR
- Extract all frames or a custom range
- Auto-numbered frame output

### Presets & Usability
- Save/load/delete conversion presets (stored in a cross-platform location)
- Improved error dialogs for file and directory operations
- Command preview: see the full FFmpeg command before running
- Modern dark-themed UI with tabbed workflow
- Real-time log and progress bar

## Requirements

- macOS 15.5+ (Intel or Apple Silicon) or Windows 10+
- Qt6 (must be discoverable by CMake)
- FFmpeg (must be installed and in your PATH)
- CMake
- NMake (Windows, comes with Visual Studio Build Tools)

## Installation

### Recommended: One-Line Setup (macOS)
```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

### Manual Setup (macOS)
```bash
brew install qt6 cmake ffmpeg
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
make -j$(sysctl -n hw.ncpu)
open ImageSequenceConverter.app
```

### Windows Build & Run

#### Requirements
- Windows 10 or later
- Qt6, CMake, FFmpeg (all must be in your PATH)
- NMake (comes with Visual Studio Build Tools)

#### One-Line Setup (Windows)
```bat
build_and_run_windows.bat
```

#### Manual Setup (Windows)
Open a Visual Studio Developer Command Prompt:
```bat
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake
start ImageSequenceConverter.exe
```

## Testing

To build and run tests (macOS/Linux):
```bash
cd build
ctest --output-on-failure
```
Or run the test binary directly:
```bash
./tests/test_presetmanager
```

## License

MIT License

Copyright (c) 2025 [Your Name or Organization]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---
Contributions are welcome via issues or pull requests.
