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

// mainwindow.h
// Main application window for Image Sequence Converter. Handles all UI and user interaction.
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QProgressBar>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QCheckBox>
#include <QSlider>
#include "converter.h"
#include "presetmanager.h"
#include "editablecommanddialog.h"
#include "constants.h"
#include "conversionservice.h"

// MainWindow: top-level window and controller for the app
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI event handlers
    void selectInputPath();
    void selectOutputPath();
    void startConversion();
    void onConversionProgress(int percentage);
    void onConversionFinished(bool success, const QString &message);
    void onConversionModeChanged();
    void updateFrameRateDisplay(int value);
    void updateQualityDisplay(int value);
    void startVideoToSequenceConversion();
    void showFFmpegCommandPreview();
    void showVideoToSequenceCommandPreview();
    void showSequenceToVideoCommandPreview();
    void saveCurrentPreset();
    void loadSelectedPreset();
    void deleteSelectedPreset();
    void refreshPresetList();



private:
    void setupUI();
    void setupSequenceToVideoTab();
    void setupVideoToSequenceTab();
    void connectSignals();
    void updateUIForMode();
    
    // UI Components
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *seqToVideoTab;
    QWidget *videoToSeqTab;
    
    // Common controls
    QLineEdit *inputPathEdit;
    QLineEdit *outputPathEdit;
    QPushButton *inputBrowseBtn;
    QPushButton *outputBrowseBtn;
    QPushButton *convertBtn;
    QProgressBar *progressBar;
    QTextEdit *logOutput;
    
    // Sequence to Video controls
    QComboBox *videoFormatCombo;
    QComboBox *videoCodecCombo;
    QSpinBox *frameRateSpinBox;
    QLabel *frameRateLabel;
    QSpinBox *qualitySpinBox;
    QLabel *qualityLabel;
    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QCheckBox *maintainAspectRatio;
    QCheckBox *exportAudioCheckBox;
    
    // Video to Sequence controls
    QComboBox *imageFormatCombo;
    QSpinBox *startFrameSpinBox;
    QSpinBox *endFrameSpinBox;
    QCheckBox *extractAllFrames;
    QLineEdit *videoInputEdit;
    QLineEdit *seqOutputEdit;
    QPushButton *convertVideoBtn;
    QTextEdit *commandPreviewEdit;
    QTextEdit *commandPreviewEditVideoTab;

    // Preset controls
    QPushButton *savePresetBtn;
    QPushButton *loadPresetBtn;
    QPushButton *deletePresetBtn;
    QComboBox *presetSelector;
    
    // Backend
    //Converter *converter; // Replaced by ConversionService
    ConversionService *conversionService;
    PresetManager *presetManager;
    bool isConverting;

    // Command preview buttons
    QPushButton *previewCmdBtn;
    QPushButton *previewVideoCmdBtn;
};

#endif // MAINWINDOW_H