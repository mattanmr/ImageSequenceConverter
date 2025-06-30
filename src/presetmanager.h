// This code is provided as is, without warranty of any kind.
// presetmanager.h
// Handles saving, loading, and removing user presets for conversion settings.
#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include "converter.h"

// PresetManager: manages persistent storage of user presets
class PresetManager : public QObject {
    Q_OBJECT

public:
    explicit PresetManager(QObject *parent = nullptr);

    // Save a named preset
    void savePreset(const QString &name, const ConversionSettings &settings);
    // Load all saved presets
    QList<QPair<QString, ConversionSettings>> loadPresets();
    // Remove a preset by name
    bool removePreset(const QString &name);

private:
    // Returns the full path to the presets file
    QString presetFilePath() const;
    // Returns the folder where presets are stored
    QString presetFolder() const;
    // Serialize settings to JSON
    QJsonObject settingsToJson(const ConversionSettings &settings) const;
    // Deserialize settings from JSON
    ConversionSettings jsonToSettings(const QJsonObject &obj) const;
};

#endif // PRESETMANAGER_H
