// converter.cpp
#include "converter.h"
#include <QApplication>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QDebug>

Converter::Converter(QObject *parent)
    : QObject(parent)
    , ffmpegProcess(nullptr)
    , isProcessing(false)
    , totalFrames(0)
{
    ffmpegPath = findFFmpegPath();
}

Converter::~Converter()
{
    if (ffmpegProcess && ffmpegProcess->state() != QProcess::NotRunning) {
        ffmpegProcess->kill();
        ffmpegProcess->waitForFinished(3000);
    }
}

bool Converter::isFFmpegAvailable()
{
    return !ffmpegPath.isEmpty();
}

QString Converter::findFFmpegPath() const
{
    // Check common installation paths
    QStringList possiblePaths = {
        "/usr/local/bin/ffmpeg",
        "/opt/homebrew/bin/ffmpeg",
        "/usr/bin/ffmpeg",
        QStandardPaths::findExecutable("ffmpeg")
    };
    
    for (const QString &path : possiblePaths) {
        if (!path.isEmpty() && QFileInfo::exists(path)) {
            return path;
        }
    }
    
    return QString();
}

void Converter::convertSequenceToVideo(const ConversionSettings &settings)
{
    if (isProcessing) {
        emit finished(false, "Another conversion is already in progress.");
        return;
    }
    
    if (ffmpegPath.isEmpty()) {
        emit finished(false, "FFmpeg not found. Please install FFmpeg and restart the application.");
        return;
    }
    
    currentSettings = settings;
    
    // Check for custom command first
    if (!settings.customCommand.isEmpty()) {
        emit logMessage("Running custom FFmpeg command:");
        emit logMessage(settings.customCommand);
        
        ffmpegProcess = new QProcess(this);
        connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &Converter::onProcessFinished);
        connect(ffmpegProcess, &QProcess::errorOccurred, this, &Converter::onProcessError);
        connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &Converter::onProcessOutput);
        
        isProcessing = true;
        ffmpegProcess->start(settings.customCommand);
        return;
    }
    
    // Find image files in the directory
    QStringList imageFiles = findImageFiles(settings.inputPath);
    if (imageFiles.isEmpty()) {
        isProcessing = false;
        emit finished(false, "No image files found in the selected directory.");
        return;
    }
    
    totalFrames = imageFiles.size();
    
    QStringList args = buildFFmpegArguments(settings, true);

    emit logMessage("Starting conversion...");
    emit logMessage("Command: " + ffmpegPath + " " + args.join(" "));
    
    ffmpegProcess = new QProcess(this);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Converter::onProcessFinished);
    connect(ffmpegProcess, &QProcess::errorOccurred, this, &Converter::onProcessError);
    connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &Converter::onProcessOutput);
    
    isProcessing = true;
    ffmpegProcess->start(ffmpegPath, args);
}

void Converter::convertVideoToSequence(const ConversionSettings &settings)
{
    if (isProcessing) {
        emit finished(false, "Another conversion is already in progress.");
        return;
    }
    
    if (ffmpegPath.isEmpty()) {
        emit finished(false, "FFmpeg not found. Please install FFmpeg and restart the application.");
        return;
    }
    
    currentSettings = settings;

    // Check for custom command first
    if (!settings.customCommand.isEmpty()) {
        emit logMessage("Running custom FFmpeg command:");
        emit logMessage(settings.customCommand);
        
        ffmpegProcess = new QProcess(this);
        connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &Converter::onProcessFinished);
        connect(ffmpegProcess, &QProcess::errorOccurred, this, &Converter::onProcessError);
        connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &Converter::onProcessOutput);
        
        isProcessing = true;
        ffmpegProcess->start(settings.customCommand);
        return;
    }
    
    QStringList args = buildFFmpegArguments(settings, false);
    
    emit logMessage("Starting video extraction...");
    emit logMessage("Command: " + ffmpegPath + " " + args.join(" "));
    
    ffmpegProcess = new QProcess(this);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Converter::onProcessFinished);
    connect(ffmpegProcess, &QProcess::errorOccurred, this, &Converter::onProcessError);
    connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &Converter::onProcessOutput);
    
    isProcessing = true;
    ffmpegProcess->start(ffmpegPath, args);
}

QStringList Converter::buildFFmpegArguments(const ConversionSettings &settings, bool isSequenceToVideo)
{
    QStringList args;

    if (isSequenceToVideo) {
        // Image sequence to video
        QStringList imageFiles = findImageFiles(settings.inputPath);
        if (!imageFiles.isEmpty()) {
            QFileInfo fileInfo(imageFiles.first());
            QString baseName = fileInfo.baseName();
            QString extension = fileInfo.suffix();

            QRegularExpression numberPattern("(\\d+)$");
            QRegularExpressionMatch match = numberPattern.match(baseName);

            QString inputPattern;
            if (match.hasMatch()) {
                QString numberPart = match.captured(1);
                QString nameWithoutNumber = baseName.left(baseName.length() - numberPart.length());
                inputPattern = QDir(settings.inputPath).absoluteFilePath(
                    QString("%1%2.%3").arg(nameWithoutNumber).arg("%04d").arg(extension));
            } else {
                inputPattern = QDir(settings.inputPath).absoluteFilePath(QString("*.%1").arg(extension));
            }

            args << "-framerate" << QString::number(settings.frameRate);
            args << "-i" << inputPattern;
        }

        QString codecName = getVideoCodecName(settings.videoCodec);
        args << "-c:v" << codecName;

        if (codecName.contains("libx264") || codecName.contains("libx265")) {
            args << "-crf" << QString::number(settings.quality);
        }

        if (settings.maintainAspectRatio) {
            args << "-vf" << QString("scale=%1:%2:force_original_aspect_ratio=decrease,pad=%1:%2:(ow-iw)/2:(oh-ih)/2")
                             .arg(settings.width).arg(settings.height);
        } else {
            args << "-s" << QString("%1x%2").arg(settings.width).arg(settings.height);
        }

        args << "-f" << settings.videoFormat.toLower();
        args << "-y";
        args << settings.outputPath;

    } else {
        // Video to sequence
        args << "-i" << settings.inputPath;

        if (!settings.extractAllFrames) {
            args << "-ss" << QString::number(settings.startFrame);
            args << "-frames:v" << QString::number(settings.endFrame - settings.startFrame + 1);
        }

        QString outputPattern = QDir(settings.outputPath).absoluteFilePath(
            QString("frame_%04d.%1").arg(settings.imageFormat.toLower()));
        args << outputPattern;
    }

    return args;
}

QString Converter::getVideoCodecName(const QString &codec)
{
    if (codec == "H.264") return "libx264";
    if (codec == "H.265") return "libx265";
    if (codec == "VP9") return "libvpx-vp9";
    if (codec == "ProRes") return "prores";
    return "libx264"; // Default
}

QString Converter::getVideoFormatExtension(const QString &format)
{
    QString lowerFormat = format.toLower();
    if (lowerFormat == "mp4") return "mp4";
    if (lowerFormat == "avi") return "avi";
    if (lowerFormat == "mov") return "mov";
    if (lowerFormat == "mkv") return "mkv";
    if (lowerFormat == "webm") return "webm";
    return "mp4"; // Default
}

QStringList Converter::findImageFiles(const QString &directory)
{
    QDir dir(directory);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.jpeg" << "*.png" << "*.tiff" << "*.tif" 
                << "*.bmp" << "*.exr" << "*.hdr" << "*.pic" << "*.ppm";
    
    QStringList imageFiles = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    
    // Convert to absolute paths
    QStringList absolutePaths;
    for (const QString &fileName : imageFiles) {
        absolutePaths.append(dir.absoluteFilePath(fileName));
    }
    
    return absolutePaths;
}

void Converter::cancel()
{
    if (ffmpegProcess && isProcessing) {
        ffmpegProcess->kill();
        isProcessing = false;
        emit logMessage("Conversion cancelled by user.");
    }
}

void Converter::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    isProcessing = false;
    
    if (ffmpegProcess) {
        ffmpegProcess->deleteLater();
        ffmpegProcess = nullptr;
    }
    
    if (exitStatus == QProcess::CrashExit) {
        emit finished(false, "FFmpeg process crashed.");
        return;
    }
    
    if (exitCode == 0) {
        emit progressChanged(100);
        emit finished(true, "Conversion completed successfully!");
    } else {
        emit finished(false, QString("Conversion failed with exit code %1").arg(exitCode));
    }
}

void Converter::onProcessError(QProcess::ProcessError error)
{
    isProcessing = false;
    
    QString errorString;
    switch (error) {
        case QProcess::FailedToStart:
            errorString = "Failed to start FFmpeg. Please check if FFmpeg is installed.";
            break;
        case QProcess::Crashed:
            errorString = "FFmpeg process crashed.";
            break;
        case QProcess::Timedout:
            errorString = "FFmpeg process timed out.";
            break;
        case QProcess::WriteError:
            errorString = "Write error occurred.";
            break;
        case QProcess::ReadError:
            errorString = "Read error occurred.";
            break;
        default:
            errorString = "Unknown error occurred.";
            break;
    }
    
    emit finished(false, errorString);
}

void Converter::onProcessOutput()
{
    if (!ffmpegProcess) return;
    
    QByteArray data = ffmpegProcess->readAllStandardError();
    QString output = QString::fromUtf8(data);
    
    emit logMessage(output);
    parseProgress(output);
}

void Converter::parseProgress(const QString &output)
{
    // Parse FFmpeg progress output
    static QRegularExpression frameRegex("frame=\\s*(\\d+)");
    static QRegularExpression timeRegex("time=(\\d{2}):(\\d{2}):(\\d{2}\\.\\d{2})");
    
    QRegularExpressionMatch frameMatch = frameRegex.match(output);
    if (frameMatch.hasMatch() && totalFrames > 0) {
        int currentFrame = frameMatch.captured(1).toInt();
        int percentage = qMin(100, (currentFrame * 100) / totalFrames);
        emit progressChanged(percentage);
    }
}