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
// conversionservice.cpp
// Implements ConversionService, a high-level interface for conversion operations.
#include "conversionservice.h"

ConversionService::ConversionService(QObject *parent)
    : QObject(parent), converter(new Converter(this)), converting(false)
{
    connect(converter, &Converter::progressChanged, this, &ConversionService::conversionProgress);
    connect(converter, &Converter::finished, this, [this](bool success, const QString &msg) {
        converting = false;
        emit conversionFinished(success, msg);
    });
}

ConversionService::~ConversionService() {}

void ConversionService::startConversion(const ConversionSettings &settings)
{
    if (converting) return;
    converting = true;
    emit conversionStarted();
    // Decide which conversion to run based on settings
    if (!settings.inputPath.isEmpty() && !settings.outputPath.isEmpty()) {
        if (!settings.videoFormat.isEmpty()) {
            converter->convertSequenceToVideo(settings);
        } else if (!settings.imageFormat.isEmpty()) {
            converter->convertVideoToSequence(settings);
        } else {
            converting = false;
            emit conversionFinished(false, "Invalid conversion settings.");
        }
    } else {
        converting = false;
        emit conversionFinished(false, "Input or output path is empty.");
    }
}

void ConversionService::cancelConversion()
{
    if (converting) {
        converter->cancel();
        converting = false;
    }
}

bool ConversionService::isConverting() const
{
    return converting;
}

QString ConversionService::buildFFmpegCommandPreview(const ConversionSettings &settings, bool isSequenceToVideo) const {
    QStringList args = converter->buildFFmpegArguments(settings, isSequenceToVideo);
    if (converter->isFFmpegAvailable()) {
        return converter->findFFmpegPath() + " " + args.join(" ");
    } else {
        return "ffmpeg not found";
    }
}

bool ConversionService::isFFmpegAvailable() const {
    return converter->isFFmpegAvailable();
}

QString ConversionService::findFFmpegPath() const {
    return converter->findFFmpegPath();
}
