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
