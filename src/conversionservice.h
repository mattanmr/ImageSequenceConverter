/*
MIT License

Copyright (c) 2025 Mattan Ram

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
// conversionservice.h
// Provides a high-level service interface for conversion operations, decoupling UI from conversion logic.
#ifndef CONVERSIONSERVICE_H
#define CONVERSIONSERVICE_H

#include <QObject>
#include "converter.h"

// ConversionService: wraps Converter and exposes signals for UI
// Maps Converter's progressChanged to conversionProgress, finished to conversionFinished
// Calls convertSequenceToVideo or convertVideoToSequence based on settings
class ConversionService : public QObject {
    Q_OBJECT
public:
    explicit ConversionService(QObject *parent = nullptr);
    ~ConversionService();

    // Start a conversion with the given settings
    void startConversion(const ConversionSettings &settings);
    // Cancel the current conversion
    void cancelConversion();
    // Returns true if a conversion is in progress
    bool isConverting() const;
    // Returns the FFmpeg command line for preview (sequenceToVideo or videoToSequence)
    QString buildFFmpegCommandPreview(const ConversionSettings &settings, bool isSequenceToVideo) const;
    // Returns true if FFmpeg is available
    bool isFFmpegAvailable() const;
    // Returns the FFmpeg executable path
    QString findFFmpegPath() const;

signals:
    void conversionStarted();
    void conversionProgress(int percent);
    void conversionFinished(bool success, const QString &message);

private:
    Converter *converter;
    bool converting;
};

#endif // CONVERSIONSERVICE_H
