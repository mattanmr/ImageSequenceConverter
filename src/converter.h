// converter.h
// Conversion logic for image sequence <-> video using FFmpeg. Handles process management and settings.
#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QThread>

// Holds all user-configurable settings for a conversion operation
struct ConversionSettings {
    QString inputPath;         // Input file or folder
    QString outputPath;        // Output file or folder
    QString videoFormat;       // Target video format (e.g., MP4)
    QString videoCodec;        // Video codec (e.g., H.264)
    int frameRate;             // Frames per second
    int quality;               // Codec quality/CRF
    int width;                 // Output width
    int height;                // Output height
    bool maintainAspectRatio;  // Whether to preserve aspect ratio
    // Video to sequence settings
    QString imageFormat;       // Output image format (e.g., PNG)
    int startFrame;            // First frame to extract
    int endFrame;              // Last frame to extract
    bool extractAllFrames;     // Extract all frames flag
    QString customCommand;     // Optional raw ffmpeg command
    bool exportAudio = false;  // Export audio track (for video export)
};

// Converter: manages FFmpeg process and conversion logic
class Converter : public QObject
{
    Q_OBJECT

public:
    explicit Converter(QObject *parent = nullptr);
    ~Converter();
    
    void convertSequenceToVideo(const ConversionSettings &settings);
    void convertVideoToSequence(const ConversionSettings &settings);
    void cancel();
    
    bool isFFmpegAvailable();

    QStringList buildFFmpegArguments(const ConversionSettings &settings, bool isSequenceToVideo);
    QString findFFmpegPath() const;

signals:
    void progressChanged(int percentage);
    void finished(bool success, const QString &message);
    void logMessage(const QString &message);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onProcessOutput();

private:
    QString getVideoCodecName(const QString &codec);
    QString getVideoFormatExtension(const QString &format);
    QStringList findImageFiles(const QString &directory);
    void parseProgress(const QString &output);
    
    QProcess *ffmpegProcess;
    ConversionSettings currentSettings;
    bool isProcessing;
    int totalFrames;
    QString ffmpegPath;
};

#endif // CONVERTER_H