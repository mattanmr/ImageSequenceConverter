// test_presetmanager.cpp
// Minimal test for PresetManager: save, load, and remove a preset.
#include <QtTest/QtTest>
#include "../src/presetmanager.h"

class TestPresetManager : public QObject {
    Q_OBJECT
private slots:
    void testSaveLoadRemove();
};

void TestPresetManager::testSaveLoadRemove() {
    PresetManager manager;
    QString presetName = "test_preset";
    ConversionSettings settings;
    settings.inputPath = "/tmp/input";
    settings.outputPath = "/tmp/output";
    settings.videoFormat = "mp4";
    settings.videoCodec = "H.264";
    settings.frameRate = 24;
    settings.quality = 23;
    settings.width = 1920;
    settings.height = 1080;
    settings.maintainAspectRatio = true;
    settings.imageFormat = "PNG";
    settings.startFrame = 0;
    settings.endFrame = 10;
    settings.extractAllFrames = true;
    settings.customCommand = "";
    // Save preset
    manager.savePreset(presetName, settings);
    // Load presets and check
    auto list = manager.loadPresets();
    bool found = false;
    for (const auto &pair : list) {
        if (pair.first == presetName) {
            found = true;
            QCOMPARE(pair.second.inputPath, settings.inputPath);
            QCOMPARE(pair.second.outputPath, settings.outputPath);
        }
    }
    QVERIFY(found);
    // Remove preset
    QVERIFY(manager.removePreset(presetName));
}

QTEST_MAIN(TestPresetManager)
#include "test_presetmanager.moc"
