// mainwindow.h
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
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
    
    // Video to Sequence controls
    QComboBox *imageFormatCombo;
    QSpinBox *startFrameSpinBox;
    QSpinBox *endFrameSpinBox;
    QCheckBox *extractAllFrames;
    QLineEdit *videoInputEdit;
    QLineEdit *seqOutputEdit;
    QPushButton *convertVideoBtn;
    QLineEdit *commandPreviewEdit;
    QPushButton *previewCmdBtn;

    
    // Backend
    Converter *converter;
    bool isConverting;
};

#endif // MAINWINDOW_H