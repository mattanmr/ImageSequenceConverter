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

class PresetManager : public QObject {
    Q_OBJECT

public:
    explicit PresetManager(QObject *parent = nullptr);

    void savePreset(const QString &name, const ConversionSettings &settings);
    QList<QPair<QString, ConversionSettings>> loadPresets();
    bool removePreset(const QString &name);

private:
    QString presetFilePath() const;
    QString presetFolder() const;
    QJsonObject settingsToJson(const ConversionSettings &settings) const;
    ConversionSettings jsonToSettings(const QJsonObject &obj) const;
};

#endif // PRESETMANAGER_H
