cmake_minimum_required(VERSION 3.16)

project(ImageSequenceConverter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC, UIC, and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set the minimum macOS version
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/converter.cpp
    src/droplineedit.cpp

)

# Header files
set(HEADERS
    src/mainwindow.h
    src/converter.h
)

# Create the executable
add_executable(ImageSequenceConverter ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(ImageSequenceConverter
    Qt6::Core
    Qt6::Widgets
)

# Set application properties for macOS
set_target_properties(ImageSequenceConverter PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.imageconverter.app
    MACOSX_BUNDLE_BUNDLE_NAME "Image Sequence Converter"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_INFO_STRING "Image Sequence to Video Converter"
)