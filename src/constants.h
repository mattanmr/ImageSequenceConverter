/*
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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
