// converter.h
#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QThread>

struct ConversionSettings {
    QString inputPath;
    QString outputPath;
    QString videoFormat;
    QString videoCodec;
    int frameRate;
    int quality;
    int width;
    int height;
    bool maintainAspectRatio;
    
    // Video to sequence settings
    QString imageFormat;
    int startFrame;
    int endFrame;
    bool extractAllFrames;
};

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

    QString buildFFmpegCommand(const ConversionSettings &settings, bool isSequenceToVideo);

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
    QString findFFmpegPath();
    void parseProgress(const QString &output);
    
    QProcess *ffmpegProcess;
    ConversionSettings currentSettings;
    bool isProcessing;
    int totalFrames;
    QString ffmpegPath;
};

#endif // CONVERTER_H