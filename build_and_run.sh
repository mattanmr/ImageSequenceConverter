#!/bin/bash

# ImageSequenceConverter - Build and Run Script for macOS 15.5
# This script installs dependencies, builds, and runs the application

set -e  # Exit on any error

echo "🚀 Image Sequence Converter - Setup Script"
echo "=========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    print_error "This script is designed for macOS only."
    exit 1
fi

# Check macOS version
MACOS_VERSION=$(sw_vers -productVersion)
print_status "Detected macOS version: $MACOS_VERSION"

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
    print_warning "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    # Add Homebrew to PATH for Apple Silicon Macs
    if [[ $(uname -m) == 'arm64' ]]; then
        echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi
else
    print_success "Homebrew is already installed."
fi

read -p "Update Homebrew? [y/n]" answer
if [[ $answer = y ]] ; then
  # Update Homebrew
    print_status "Updating Homebrew..."
    brew update
else
    print_status "Skipping Homebrew update"
fi

# Install Qt6 if not already installed
if ! brew list qt6 &> /dev/null; then
    print_status "Installing Qt6..."
    brew install qt6
else
    print_success "Qt6 is already installed."
fi

# Install CMake if not already installed
if ! command -v cmake &> /dev/null; then
    print_status "Installing CMake..."
    brew install cmake
else
    print_success "CMake is already installed."
fi

# Install FFmpeg if not already installed
if ! command -v ffmpeg &> /dev/null; then
    print_status "Installing FFmpeg..."
    brew install ffmpeg
else
    print_success "FFmpeg is already installed."
fi

# Verify installations
print_status "Verifying installations..."

# Check Qt6
QT6_PATH=$(brew --prefix qt6)
if [[ -d "$QT6_PATH" ]]; then
    print_success "Qt6 found at: $QT6_PATH"
    export CMAKE_PREFIX_PATH="$QT6_PATH:$CMAKE_PREFIX_PATH"
else
    print_error "Qt6 installation not found!"
    exit 1
fi

# Check CMake
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    print_success "CMake version: $CMAKE_VERSION"
else
    print_error "CMake not found!"
    exit 1
fi

# Check FFmpeg
if command -v ffmpeg &> /dev/null; then
    FFMPEG_VERSION=$(ffmpeg -version 2>/dev/null | head -n1 | cut -d' ' -f3)
    print_success "FFmpeg version: $FFMPEG_VERSION"
else
    print_error "FFmpeg not found!"
    exit 1
fi

# Create build directory
BUILD_DIR="build"
if [[ -d "$BUILD_DIR" ]]; then
    print_warning "Build directory exists. Cleaning..."
    rm -rf "$BUILD_DIR"
fi

print_status "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
print_status "Configuring project with CMake..."
cmake .. -DCMAKE_PREFIX_PATH="$QT6_PATH" \
         -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15

# Build the project
print_status "Building the application..."
make -j$(sysctl -n hw.ncpu)

# Check if build was successful
if [[ -f "ImageSequenceConverter.app/Contents/MacOS/ImageSequenceConverter" ]]; then
    print_success "Build completed successfully!"
    
    # Ask user if they want to run the application
    echo ""
    read -p "Do you want to run the application now? (y/N): " -n 1 -r
    echo ""
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        print_status "Launching Image Sequence Converter..."
        open ImageSequenceConverter.app
        print_success "Application launched!"
    else
        print_status "You can run the application later by executing:"
        echo "  open $(pwd)/ImageSequenceConverter.app"
    fi
    
    # Provide usage information
    echo ""
    echo "📝 Usage Notes:"
    echo "==============="
    echo "• The application supports common image formats: JPEG, PNG, TIFF, BMP, EXR"
    echo "• Supported video formats: MP4, AVI, MOV, MKV, WebM"
    echo "• Make sure your image sequence is numbered sequentially"
    echo "• FFmpeg is used for all conversions"
    echo ""
    echo "🔧 Troubleshooting:"
    echo "==================="
    echo "• If FFmpeg is not detected, restart the application after running this script"
    echo "• For large image sequences, allow sufficient time for processing"
    echo "• Check the conversion log for detailed error messages"
    
else
    print_error "Build failed! Check the output above for errors."
    exit 1
fi

print_success "Setup completed successfully!"

# Create a simple run script for future use
cd ..
cat > run_app.sh << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"
if [[ -f "build/ImageSequenceConverter.app/Contents/MacOS/ImageSequenceConverter" ]]; then
    open build/ImageSequenceConverter.app
else
    echo "Application not built. Please run build_and_run.sh first."
    exit 1
fi
EOF

chmod +x run_app.sh
print_status "Created run_app.sh for future launches"