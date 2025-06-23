// src/constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QStringList>

namespace Constants {
    constexpr int MaxVideoWidth = 7680;
    constexpr int MaxVideoHeight = 4320;
    constexpr int MaxSpinWidth = 80;
    constexpr int MaxQuality = 51;
    constexpr int MinQuality = 1;
    constexpr int DefaultQuality = 23;
    constexpr int MaxFrameRate = 60;
    constexpr int DefaultFrameRate = 24;

    const QStringList SupportedVideoFormats = {"MP4", "AVI", "MOV", "MKV", "WebM"};
    const QStringList SupportedCodecs = {"H.264", "H.265", "VP9", "ProRes"};
    const QStringList SupportedImageFormats = {"PNG", "JPEG", "TIFF", "BMP", "EXR"};
}

#endif // CONSTANTS_H
