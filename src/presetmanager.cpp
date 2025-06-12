#include "presetmanager.h"
#include <QCoreApplication>
#include <QDir>

PresetManager::PresetManager(QObject *parent) : QObject(parent) {}

QString PresetManager::presetFolder() const {
    QString dir = QCoreApplication::applicationDirPath() + "/presets";
    QDir().mkpath(dir);
    return dir;
}

QString PresetManager::presetFilePath() const {
    return presetFolder() + "/presets.json";
}

void PresetManager::savePreset(const QString &name, const ConversionSettings &settings) {
    QFile file(presetFilePath());
    QJsonObject allPresets;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) allPresets = doc.object();
        file.close();
    }

    allPresets[name] = settingsToJson(settings);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(allPresets).toJson());
        file.close();
    }
}

QList<QPair<QString, ConversionSettings>> PresetManager::loadPresets() {
    QList<QPair<QString, ConversionSettings>> list;
    QFile file(presetFilePath());

    if (!file.open(QIODevice::ReadOnly)) return list;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return list;

    QJsonObject obj = doc.object();
    for (const QString &key : obj.keys()) {
        list.append({key, jsonToSettings(obj[key].toObject())});
    }
    return list;
}

bool PresetManager::removePreset(const QString &name) {
    QFile file(presetFilePath());
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) return false;

    QJsonObject obj = doc.object();
    obj.remove(name);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    file.write(QJsonDocument(obj).toJson());
    file.close();
    return true;
}

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
