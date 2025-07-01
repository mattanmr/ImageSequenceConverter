/*
MIT License

Copyright (c) 2025 [Your Name or Organization]

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
