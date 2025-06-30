// This code is provided as is, without warranty of any kind.
// constants.h
// Centralized application-wide constants and supported format lists for Image Sequence Converter.
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QStringList>

namespace Constants {
    // Maximum allowed video width (in pixels)
    constexpr int MaxVideoWidth = 7680;
    // Maximum allowed video height (in pixels)
    constexpr int MaxVideoHeight = 4320;
    // Maximum width for spinboxes in the UI
    constexpr int MaxSpinWidth = 80;
    // Video quality range (lower is better for most codecs)
    constexpr int MaxQuality = 51;
    constexpr int MinQuality = 1;
    constexpr int DefaultQuality = 23;
    // Frame rate limits
    constexpr int MaxFrameRate = 60;
    constexpr int DefaultFrameRate = 24;

    // Supported formats for video, codecs, and images
    const QStringList SupportedVideoFormats = {"MP4", "AVI", "MOV", "MKV", "WebM"};
    const QStringList SupportedCodecs = {"H.264", "H.265", "VP9", "ProRes"};
    const QStringList SupportedImageFormats = {"PNG", "JPEG", "TIFF", "BMP", "EXR"};
}

#endif // CONSTANTS_H
