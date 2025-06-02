# Image Sequence Converter

A powerful and user-friendly macOS application for converting between image sequences and videos using Qt6 and FFmpeg.

## Features

### Image Sequence → Video
- **Multiple Input Formats**: JPEG, PNG, TIFF, BMP, EXR, HDR
- **Video Output Formats**: MP4, AVI, MOV, MKV, WebM
- **Codec Options**: H.264, H.265, VP9, ProRes
- **Quality Control**: Adjustable CRF values for optimal file size/quality balance
- **Frame Rate Control**: 1-60 FPS with slider control
- **Resolution Settings**: Custom width/height with aspect ratio maintenance
- **Real-time Progress**: Visual progress bar and detailed logging

### Video → Image Sequence
- **Multiple Output Formats**: PNG, JPEG, TIFF, BMP, EXR
- **Flexible Extraction**: Extract all frames or specify frame range
- **Batch Processing**: Automatic frame numbering and organization

### User Interface
- **Modern Dark Theme**: Professional appearance optimized for macOS
- **Tabbed Interface**: Easy switching between conversion modes
- **Intuitive Controls**: Sliders, dropdowns, and clear labeling
- **Real-time Feedback**: Progress indicators and conversion logs
- **Error Handling**: Clear error messages and troubleshooting guidance

## Requirements

- **macOS**: 15.5 or later
- **Architecture**: Intel x64 or Apple Silicon (M1/M2/M3)
- **Dependencies**: Qt6, CMake, FFmpeg (automatically installed by setup script)

## Installation & Setup

### Automatic Setup (Recommended)

1. **Download the project files** and extract them to a folder
2. **Open Terminal** and navigate to the project directory
3. **Run the setup script**:
   ```bash
   chmod +x build_and_run.sh
   ./build_and_run.sh
   ```

The script will automatically:
- Install Homebrew (if not present)
- Install Qt6, CMake, and FFmpeg
- Build the application
- Offer to launch it immediately

### Manual Setup

If you prefer manual installation:

```bash
# Install dependencies via Homebrew
brew install qt6 cmake ffmpeg

# Build the application
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
make -j$(sysctl -n hw.ncpu)

# Run the application
open ImageSequenceConverter.app
```

## Usage

### Converting Image Sequence to Video

1. **Select Input Directory**: Choose the folder containing your image sequence
2. **Choose Output Location**: Specify where to save the video file
3. **Configure Settings**:
   - **Format**: MP4, AVI, MOV, MKV, or WebM
   - **Codec**: H.264 (recommended), H.265, VP9, or ProRes
   - **Frame Rate**: Adjust the playback speed (1-60 FPS)
   - **Quality**: Balance between file size and visual quality
   - **Resolution**: Set custom dimensions or maintain aspect ratio
4. **Start Conversion**: Click "Convert to Video" and monitor progress

### Converting Video to Image Sequence

1. **Select Input Video**: Choose your source video file
2. **Choose Output Directory**: Specify where to save extracted frames
3. **Configure Settings**:
   - **Format**: PNG (recommended), JPEG, TIFF, BMP, or EXR
   - **Frame Range**: Extract all frames or specify start/end points
4. **Start Extraction**: Click "Convert to Image Sequence"

## Supported Formats

### Input Image Formats
- JPEG (.jpg, .jpeg)
- PNG (.png)
- TIFF (.tiff, .tif)
- BMP (.bmp)
- OpenEXR (.exr)
- HDR (.hdr)
- PPM (.ppm)

### Video Formats
- MP4 (H.264, H.265)
- AVI (various codecs)
- MOV (QuickTime)
- MKV (Matroska)
- WebM (VP9)

## Tips for Best Results

### Image Sequences
- **Naming Convention**: Use sequential numbering (e.g., `frame_0001.jpg`, `frame_0002.jpg`)
- **Consistent Resolution**: Ensure all images have the same dimensions
- **File Organization**: Keep all sequence images in a dedicated folder
- **Format Choice**: Use PNG for transparency, JPEG for smaller files

### Video Settings
- **H.264**: Best compatibility across devices and platforms
- **H.265**: Better compression but requires modern hardware
- **CRF 18-23**: High quality suitable for most purposes
- **CRF 28-33**: Good quality with smaller file sizes
- **Frame Rate**: Match your source material's frame rate when possible

## Troubleshooting

### Common Issues

**FFmpeg Not Found**
- Restart the application after running the setup script
- Manually verify FFmpeg installation: `which ffmpeg`

**Build Errors**
- Ensure Xcode Command Line Tools are installed: `xcode-select --install`
- Check that Qt6 is properly installed: `brew list qt6`

**Conversion Failures**
- Check the conversion log for detailed error messages
- Verify input file formats are supported
- Ensure sufficient disk space for output files

**Performance Issues**
- For large sequences, allow adequate processing time
- Close other applications to free up system resources
- Consider reducing output resolution for faster processing

## Technical Details

### Architecture
- **Frontend**: Qt6 with modern C++17
- **Backend**: FFmpeg for media processing
- **Build System**: CMake with automatic dependency management
- **Threading**: Asynchronous processing with progress reporting

### File Structure
```
ImageSequenceConverter/
├── main.cpp              # Application entry point
├── mainwindow.h/.cpp     # Main UI implementation
├── converter.h/.cpp      # FFmpeg integration
├── CMakeLists.txt        # Build configuration
├── build_and_run.sh      # Automated setup script
└── README.md            # This documentation
```

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## Acknowledgments

- **Qt Framework**: For the excellent cross-platform UI toolkit
- **FFmpeg**: For powerful media processing capabilities
- **Homebrew**: For simplified dependency management on macOS
