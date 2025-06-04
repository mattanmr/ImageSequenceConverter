# Image Sequence Converter

A macOS application for converting between image sequences and videos, built with Qt6 and FFmpeg.

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

- macOS 15.5 or later (Intel or Apple Silicon)
- Dependencies: Qt6, FFmpeg, CMake

## Installation

### Recommended: One-Line Setup
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

## Project Structure

ImageSequenceConverter/
├── mainwindow.*        # UI
├── converter.*         # FFmpeg logic
├── main.cpp            # App entry
├── droplineedit.*      # Drag-and-drop widgets
├── CMakeLists.txt
└── build_and_run.sh

## License
MIT License

-- Contributions are welcome via issues or pull requests. --