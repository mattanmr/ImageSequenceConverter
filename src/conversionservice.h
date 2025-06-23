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
