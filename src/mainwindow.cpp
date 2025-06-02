// mainwindow.cpp
#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , converter(new Converter(this))
    , isConverting(false)
{
    setupUI();
    connectSignals();
    
    setWindowTitle("Image Sequence Converter");
    setMinimumSize(800, 600);
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create tab widget
    tabWidget = new QTabWidget(this);
    setupSequenceToVideoTab();
    setupVideoToSequenceTab();
    
    mainLayout->addWidget(tabWidget);
    
    // Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    
    // Log output
    QGroupBox *logGroup = new QGroupBox("Conversion Log", this);
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    logOutput = new QTextEdit(this);
    logOutput->setMaximumHeight(150);
    logOutput->setReadOnly(true);
    logLayout->addWidget(logOutput);
    mainLayout->addWidget(logGroup);
}

void MainWindow::setupSequenceToVideoTab()
{
    seqToVideoTab = new QWidget();
    tabWidget->addTab(seqToVideoTab, "Image Sequence → Video");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(seqToVideoTab);
    
    // Input/Output section
    QGroupBox *ioGroup = new QGroupBox("Input/Output", this);
    QGridLayout *ioLayout = new QGridLayout(ioGroup);
    
    ioLayout->addWidget(new QLabel("Input Directory:"), 0, 0);
    inputPathEdit = new QLineEdit(this);
    inputPathEdit->setPlaceholderText("Select folder containing image sequence...");
    ioLayout->addWidget(inputPathEdit, 0, 1);
    inputBrowseBtn = new QPushButton("Browse...", this);
    ioLayout->addWidget(inputBrowseBtn, 0, 2);
    
    ioLayout->addWidget(new QLabel("Output Video:"), 1, 0);
    outputPathEdit = new QLineEdit(this);
    outputPathEdit->setPlaceholderText("Select output video file...");
    ioLayout->addWidget(outputPathEdit, 1, 1);
    outputBrowseBtn = new QPushButton("Browse...", this);
    ioLayout->addWidget(outputBrowseBtn, 1, 2);
    
    mainLayout->addWidget(ioGroup);
    
    // Video settings
    QGroupBox *videoGroup = new QGroupBox("Video Settings", this);
    QGridLayout *videoLayout = new QGridLayout(videoGroup);
    
    videoLayout->addWidget(new QLabel("Format:"), 0, 0);
    videoFormatCombo = new QComboBox(this);
    videoFormatCombo->addItems({"MP4", "AVI", "MOV", "MKV", "WebM"});
    videoLayout->addWidget(videoFormatCombo, 0, 1);
    
    videoLayout->addWidget(new QLabel("Codec:"), 0, 2);
    videoCodecCombo = new QComboBox(this);
    videoCodecCombo->addItems({"H.264", "H.265", "VP9", "ProRes"});
    videoLayout->addWidget(videoCodecCombo, 0, 3);
    
    videoLayout->addWidget(new QLabel("Frame Rate:"), 1, 0);
    frameRateSpinBox = new QSpinBox(this);
    frameRateSpinBox->setRange(1, 60);
    frameRateSpinBox->setValue(24);
    videoLayout->addWidget(frameRateSpinBox, 1, 1);
    frameRateLabel = new QLabel("24 fps", this);
    videoLayout->addWidget(frameRateLabel, 1, 2);
    
    videoLayout->addWidget(new QLabel("Quality:"), 2, 0);
    qualitySpinBox = new QSpinBox(this);
    qualitySpinBox->setRange(1, 51);
    qualitySpinBox->setValue(23);
    videoLayout->addWidget(qualitySpinBox, 2, 1);
    qualityLabel = new QLabel("High (CRF 23)", this);
    videoLayout->addWidget(qualityLabel, 2, 2);
    
    videoLayout->addWidget(new QLabel("Width:"), 3, 0);
    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(1, 7680);
    widthSpinBox->setValue(1920);
    widthSpinBox->setSingleStep(2);
    videoLayout->addWidget(widthSpinBox, 3, 1);
    
    videoLayout->addWidget(new QLabel("Height:"), 3, 2);
    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(1, 4320);
    heightSpinBox->setValue(1080);
    heightSpinBox->setSingleStep(2);
    videoLayout->addWidget(heightSpinBox, 3, 3);
    
    maintainAspectRatio = new QCheckBox("Maintain Aspect Ratio", this);
    maintainAspectRatio->setChecked(true);
    videoLayout->addWidget(maintainAspectRatio, 4, 0, 1, 2);
    
    mainLayout->addWidget(videoGroup);
    
    // Convert button
    convertBtn = new QPushButton("Convert to Video", this);
    convertBtn->setMinimumHeight(40);
    convertBtn->setStyleSheet("QPushButton { font-weight: bold; font-size: 14px; }");
    mainLayout->addWidget(convertBtn);
    
    mainLayout->addStretch();
}

void MainWindow::setupVideoToSequenceTab()
{
    videoToSeqTab = new QWidget();
    tabWidget->addTab(videoToSeqTab, "Video → Image Sequence");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(videoToSeqTab);
    
    // Input/Output section for video tab
    QGroupBox *ioGroup = new QGroupBox("Input/Output", this);
    QGridLayout *ioLayout = new QGridLayout(ioGroup);
    
    ioLayout->addWidget(new QLabel("Input Video:"), 0, 0);
    QLineEdit *videoInputEdit = new QLineEdit(this);
    videoInputEdit->setPlaceholderText("Select video file...");
    ioLayout->addWidget(videoInputEdit, 0, 1);
    QPushButton *videoBrowseBtn = new QPushButton("Browse...", this);
    ioLayout->addWidget(videoBrowseBtn, 0, 2);
    
    ioLayout->addWidget(new QLabel("Output Directory:"), 1, 0);
    QLineEdit *seqOutputEdit = new QLineEdit(this);
    seqOutputEdit->setPlaceholderText("Select output directory...");
    ioLayout->addWidget(seqOutputEdit, 1, 1);
    QPushButton *seqBrowseBtn = new QPushButton("Browse...", this);
    ioLayout->addWidget(seqBrowseBtn, 1, 2);
    
    mainLayout->addWidget(ioGroup);
    
    // Image settings
    QGroupBox *imageGroup = new QGroupBox("Image Settings", this);
    QGridLayout *imageLayout = new QGridLayout(imageGroup);
    
    imageLayout->addWidget(new QLabel("Format:"), 0, 0);
    imageFormatCombo = new QComboBox(this);
    imageFormatCombo->addItems({"PNG", "JPEG", "TIFF", "BMP", "EXR"});
    imageLayout->addWidget(imageFormatCombo, 0, 1);
    
    extractAllFrames = new QCheckBox("Extract All Frames", this);
    extractAllFrames->setChecked(true);
    imageLayout->addWidget(extractAllFrames, 1, 0, 1, 2);
    
    imageLayout->addWidget(new QLabel("Start Frame:"), 2, 0);
    startFrameSpinBox = new QSpinBox(this);
    startFrameSpinBox->setRange(0, 999999);
    startFrameSpinBox->setEnabled(false);
    imageLayout->addWidget(startFrameSpinBox, 2, 1);
    
    imageLayout->addWidget(new QLabel("End Frame:"), 2, 2);
    endFrameSpinBox = new QSpinBox(this);
    endFrameSpinBox->setRange(0, 999999);
    endFrameSpinBox->setValue(100);
    endFrameSpinBox->setEnabled(false);
    imageLayout->addWidget(endFrameSpinBox, 2, 3);
    
    mainLayout->addWidget(imageGroup);
    
    // Convert button
    QPushButton *convertVideoBtn = new QPushButton("Convert to Image Sequence", this);
    convertVideoBtn->setMinimumHeight(40);
    convertVideoBtn->setStyleSheet("QPushButton { font-weight: bold; font-size: 14px; }");
    mainLayout->addWidget(convertVideoBtn);
    
    mainLayout->addStretch();
    
    // Connect video tab signals
    connect(videoBrowseBtn, &QPushButton::clicked, [this, videoInputEdit]() {
        QString fileName = QFileDialog::getOpenFileName(this, 
            "Select Video File", 
            QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
            "Video Files (*.mp4 *.avi *.mov *.mkv *.webm *.flv *.wmv)");
        if (!fileName.isEmpty()) {
            videoInputEdit->setText(fileName);
        }
    });
    
    connect(seqBrowseBtn, &QPushButton::clicked, [this, seqOutputEdit]() {
        QString dirName = QFileDialog::getExistingDirectory(this, 
            "Select Output Directory",
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        if (!dirName.isEmpty()) {
            seqOutputEdit->setText(dirName);
        }
    });
    
    connect(extractAllFrames, &QCheckBox::toggled, [this](bool checked) {
        startFrameSpinBox->setEnabled(!checked);
        endFrameSpinBox->setEnabled(!checked);
    });
}

void MainWindow::connectSignals()
{
    connect(inputBrowseBtn, &QPushButton::clicked, this, &MainWindow::selectInputPath);
    connect(outputBrowseBtn, &QPushButton::clicked, this, &MainWindow::selectOutputPath);
    connect(convertBtn, &QPushButton::clicked, this, &MainWindow::startConversion);
    
    //connect(frameRateSpinBox, &QSlider::valueChanged, this, &MainWindow::updateFrameRateDisplay);
    //connect(qualitySpinBox, &QSlider::valueChanged, this, &MainWindow::updateQualityDisplay);
    
    connect(converter, &Converter::progressChanged, this, &MainWindow::onConversionProgress);
    connect(converter, &Converter::finished, this, &MainWindow::onConversionFinished);
}

void MainWindow::selectInputPath()
{
    QString dirName = QFileDialog::getExistingDirectory(this, 
        "Select Image Sequence Directory",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    if (!dirName.isEmpty()) {
        inputPathEdit->setText(dirName);
    }
}

void MainWindow::selectOutputPath()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        "Save Video As", 
        QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/output.mp4",
        "Video Files (*.mp4 *.avi *.mov *.mkv *.webm)");
    if (!fileName.isEmpty()) {
        outputPathEdit->setText(fileName);
    }
}

void MainWindow::startConversion()
{
    if (isConverting) {
        converter->cancel();
        return;
    }
    
    QString inputPath = inputPathEdit->text();
    QString outputPath = outputPathEdit->text();
    
    if (inputPath.isEmpty() || outputPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select both input and output paths.");
        return;
    }
    
    ConversionSettings settings;
    settings.inputPath = inputPath;
    settings.outputPath = outputPath;
    settings.videoFormat = videoFormatCombo->currentText().toLower();
    settings.videoCodec = videoCodecCombo->currentText();
    settings.frameRate = frameRateSpinBox->value();
    settings.quality = qualitySpinBox->value();
    settings.width = widthSpinBox->value();
    settings.height = heightSpinBox->value();
    settings.maintainAspectRatio = maintainAspectRatio->isChecked();
    
    logOutput->clear();
    progressBar->setVisible(true);
    progressBar->setValue(0);
    convertBtn->setText("Cancel");
    isConverting = true;
    
    converter->convertSequenceToVideo(settings);
}

void MainWindow::onConversionProgress(int percentage)
{
    progressBar->setValue(percentage);
}

void MainWindow::onConversionFinished(bool success, const QString &message)
{
    progressBar->setVisible(false);
    convertBtn->setText("Convert to Video");
    isConverting = false;
    
    logOutput->append(message);
    
    if (success) {
        QMessageBox::information(this, "Success", "Conversion completed successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Conversion failed. Check the log for details.");
    }
}

void MainWindow::onConversionModeChanged()
{
    updateUIForMode();
}

void MainWindow::updateFrameRateDisplay(int value)
{
    frameRateLabel->setText(QString("%1 fps").arg(value));
}

void MainWindow::updateQualityDisplay(int value)
{
    QString quality;
    if (value <= 18) quality = "Very High";
    else if (value <= 23) quality = "High";
    else if (value <= 28) quality = "Medium";
    else if (value <= 33) quality = "Low";
    else quality = "Very Low";
    
    qualityLabel->setText(QString("%1 (CRF %2)").arg(quality).arg(value));
}

void MainWindow::updateUIForMode()
{
    // Future implementation for mode-specific UI updates
}