// This code is provided as is, without warranty of any kind.
// presetmanager.cpp
// Implements PresetManager, which handles persistent storage and retrieval of user conversion presets.
#include "presetmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

// Constructor
PresetManager::PresetManager(QObject *parent) : QObject(parent) {}

// Returns the folder for storing presets, creating it if necessary
QString PresetManager::presetFolder() const {
    // Use QStandardPaths for cross-platform app data location
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/presets";
    if (!QDir().mkpath(dir)) {
        QMessageBox::critical(nullptr, "Preset Folder Error", "Failed to create or access the presets directory: " + dir);
    }
    return dir;
}

// Returns the full path to the presets JSON file
QString PresetManager::presetFilePath() const {
    return presetFolder() + "/presets.json";
}

// Saves a named preset to disk (overwrites if exists)
void PresetManager::savePreset(const QString &name, const ConversionSettings &settings) {
    QFile file(presetFilePath());
    QJsonObject allPresets;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) allPresets = doc.object();
        file.close();
    } else if (file.exists()) {
        QMessageBox::critical(nullptr, "Preset Load Error", "Failed to open presets file for reading: " + file.fileName());
    }

    allPresets[name] = settingsToJson(settings);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (file.write(QJsonDocument(allPresets).toJson()) == -1) {
            QMessageBox::critical(nullptr, "Preset Save Error", "Failed to write to presets file: " + file.fileName());
        }
        file.close();
    } else {
        QMessageBox::critical(nullptr, "Preset Save Error", "Failed to open presets file for writing: " + file.fileName());
    }
}

// Loads all presets from disk
QList<QPair<QString, ConversionSettings>> PresetManager::loadPresets() {
    QList<QPair<QString, ConversionSettings>> list;
    QFile file(presetFilePath());

    if (!file.open(QIODevice::ReadOnly)) {
        if (file.exists()) {
            QMessageBox::critical(nullptr, "Preset Load Error", "Failed to open presets file for reading: " + file.fileName());
        }
        return list;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return list;

    QJsonObject obj = doc.object();
    for (const QString &key : obj.keys()) {
        list.append({key, jsonToSettings(obj[key].toObject())});
    }
    return list;
}

// Removes a preset by name
bool PresetManager::removePreset(const QString &name) {
    QFile file(presetFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        if (file.exists()) {
            QMessageBox::critical(nullptr, "Preset Remove Error", "Failed to open presets file for reading: " + file.fileName());
        }
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return false;

    QJsonObject obj = doc.object();
    obj.remove(name);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(nullptr, "Preset Remove Error", "Failed to open presets file for writing: " + file.fileName());
        return false;
    }
    if (file.write(QJsonDocument(obj).toJson()) == -1) {
        QMessageBox::critical(nullptr, "Preset Remove Error", "Failed to write to presets file: " + file.fileName());
        file.close();
        return false;
    }
    file.close();
    return true;
}

// Converts settings to JSON object
QJsonObject PresetManager::settingsToJson(const ConversionSettings &s) const {
    QJsonObject o;
    o["inputPath"] = s.inputPath;
    o["outputPath"] = s.outputPath;
    o["videoFormat"] = s.videoFormat;
    o["videoCodec"] = s.videoCodec;
    o["frameRate"] = s.frameRate;
    o["quality"] = s.quality;
    o["width"] = s.width;
    o["height"] = s.height;
    o["maintainAspectRatio"] = s.maintainAspectRatio;
    o["imageFormat"] = s.imageFormat;
    o["startFrame"] = s.startFrame;
    o["endFrame"] = s.endFrame;
    o["extractAllFrames"] = s.extractAllFrames;
    o["customCommand"] = s.customCommand;
    return o;
}

// Converts JSON object to settings
ConversionSettings PresetManager::jsonToSettings(const QJsonObject &o) const {
    ConversionSettings s;
    s.inputPath = o["inputPath"].toString();
    s.outputPath = o["outputPath"].toString();
    s.videoFormat = o["videoFormat"].toString();
    s.videoCodec = o["videoCodec"].toString();
    s.frameRate = o["frameRate"].toInt();
    s.quality = o["quality"].toInt();
    s.width = o["width"].toInt();
    s.height = o["height"].toInt();
    s.maintainAspectRatio = o["maintainAspectRatio"].toBool();
    s.imageFormat = o["imageFormat"].toString();
    s.startFrame = o["startFrame"].toInt();
    s.endFrame = o["endFrame"].toInt();
    s.extractAllFrames = o["extractAllFrames"].toBool();
    s.customCommand = o["customCommand"].toString();
    return s;
}
