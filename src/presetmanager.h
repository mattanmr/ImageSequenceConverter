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
