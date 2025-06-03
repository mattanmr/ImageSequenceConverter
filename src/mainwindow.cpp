// mainwindow.cpp
#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include "droplineedit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , converter(new Converter(this))
    , isConverting(false)
{
    setupUI();
    connectSignals();
    
    setWindowTitle("Image Sequence Converter");
    setMinimumSize(600, 500);  // Reduced minimum width
    resize(700, 600);          // Reduced default size
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);  // Add some padding
    
    // Create tab widget
    tabWidget = new QTabWidget(this);
    setupSequenceToVideoTab();
    setupVideoToSequenceTab();
    
    mainLayout->addWidget(tabWidget);
    
    // Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);

    // // Preview FFmpeg command
    // QGroupBox *cmdGroup = new QGroupBox("FFmpeg Command Preview", this);
    // QVBoxLayout *cmdLayout = new QVBoxLayout(cmdGroup);
    // commandPreviewEdit = new QLineEdit(this);
    // commandPreviewEdit->setReadOnly(true);
    // cmdLayout->addWidget(commandPreviewEdit);
    // mainLayout->addWidget(cmdGroup);
    // previewCmdBtn = new QPushButton("Show FFmpeg Command", this);
    // previewCmdBtn->setMaximumWidth(300);
    // previewCmdBtn->setStyleSheet("QPushButton { font-size: 13px; }");

    // // Center it
    // QHBoxLayout *previewLayout = new QHBoxLayout();
    // previewLayout->addStretch();
    // previewLayout->addWidget(previewCmdBtn);
    // previewLayout->addStretch();
    // mainLayout->addLayout(previewLayout);

    
    // Log output
    QGroupBox *logGroup = new QGroupBox("Conversion Log", this);
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    logOutput = new QTextEdit(this);
    logOutput->setMaximumHeight(120);  // Slightly smaller
    logOutput->setReadOnly(true);
    logLayout->addWidget(logOutput);
    mainLayout->addWidget(logGroup);
}

void MainWindow::setupSequenceToVideoTab()
{
    seqToVideoTab = new QWidget();
    tabWidget->addTab(seqToVideoTab, "Image Sequence → Video");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(seqToVideoTab);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Input/Output section - more compact
    QGroupBox *ioGroup = new QGroupBox("Input/Output", this);
    QVBoxLayout *ioLayout = new QVBoxLayout(ioGroup);
    ioLayout->setSpacing(8);
    
    // Input row
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Input Directory:"));
    inputPathEdit = new DropLineEdit(this);
    inputPathEdit->setPlaceholderText("Select folder containing image sequence...");
    inputLayout->addWidget(inputPathEdit, 1);  // Stretch factor
    inputBrowseBtn = new QPushButton("Browse...", this);
    inputBrowseBtn->setMaximumWidth(80);  // Limit button width
    inputLayout->addWidget(inputBrowseBtn);
    ioLayout->addLayout(inputLayout);
    
    // Output row
    QHBoxLayout *outputLayout = new QHBoxLayout();
    outputLayout->addWidget(new QLabel("Output Video:"));
    outputPathEdit = new DropLineEdit(this);
    outputPathEdit->setPlaceholderText("Select output video file...");
    outputLayout->addWidget(outputPathEdit, 1);  // Stretch factor
    outputBrowseBtn = new QPushButton("Browse...", this);
    outputBrowseBtn->setMaximumWidth(80);  // Limit button width
    outputLayout->addWidget(outputBrowseBtn);
    ioLayout->addLayout(outputLayout);
    
    mainLayout->addWidget(ioGroup);
    
    // Video settings - more compact arrangement
    QGroupBox *videoGroup = new QGroupBox("Video Settings", this);
    QVBoxLayout *videoLayout = new QVBoxLayout(videoGroup);
    videoLayout->setSpacing(8);
    
    // Format and Codec row
    QHBoxLayout *formatRow = new QHBoxLayout();
    formatRow->addWidget(new QLabel("Format:"));
    videoFormatCombo = new QComboBox(this);
    videoFormatCombo->addItems({"MP4", "AVI", "MOV", "MKV", "WebM"});
    videoFormatCombo->setMaximumWidth(100);
    formatRow->addWidget(videoFormatCombo);
    
    formatRow->addSpacing(20);  // Add some space
    formatRow->addWidget(new QLabel("Codec:"));
    videoCodecCombo = new QComboBox(this);
    videoCodecCombo->addItems({"H.264", "H.265", "VP9", "ProRes"});
    videoCodecCombo->setMaximumWidth(100);
    formatRow->addWidget(videoCodecCombo);
    formatRow->addStretch();  // Push everything to the left
    videoLayout->addLayout(formatRow);
    
    // Frame Rate and Quality row
    QHBoxLayout *rateQualityRow = new QHBoxLayout();
    rateQualityRow->addWidget(new QLabel("Frame Rate:"));
    frameRateSpinBox = new QSpinBox(this);
    frameRateSpinBox->setRange(1, 60);
    frameRateSpinBox->setValue(24);
    frameRateSpinBox->setMaximumWidth(60);
    rateQualityRow->addWidget(frameRateSpinBox);
    frameRateLabel = new QLabel("24 fps", this);
    frameRateLabel->setMinimumWidth(50);
    rateQualityRow->addWidget(frameRateLabel);
    
    rateQualityRow->addSpacing(20);
    rateQualityRow->addWidget(new QLabel("Quality:"));
    qualitySpinBox = new QSpinBox(this);
    qualitySpinBox->setRange(1, 51);
    qualitySpinBox->setValue(23);
    qualitySpinBox->setMaximumWidth(60);
    rateQualityRow->addWidget(qualitySpinBox);
    qualityLabel = new QLabel("High (CRF 23)", this);
    qualityLabel->setMinimumWidth(100);
    rateQualityRow->addWidget(qualityLabel);
    rateQualityRow->addStretch();
    videoLayout->addLayout(rateQualityRow);
    
    // Resolution row
    QHBoxLayout *resolutionRow = new QHBoxLayout();
    resolutionRow->addWidget(new QLabel("Width:"));
    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(1, 7680);
    widthSpinBox->setValue(1920);
    widthSpinBox->setSingleStep(2);
    widthSpinBox->setMaximumWidth(80);
    resolutionRow->addWidget(widthSpinBox);
    
    resolutionRow->addSpacing(10);
    resolutionRow->addWidget(new QLabel("Height:"));
    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(1, 4320);
    heightSpinBox->setValue(1080);
    heightSpinBox->setSingleStep(2);
    heightSpinBox->setMaximumWidth(80);
    resolutionRow->addWidget(heightSpinBox);
    
    resolutionRow->addSpacing(20);
    maintainAspectRatio = new QCheckBox("Maintain Aspect Ratio", this);
    maintainAspectRatio->setChecked(true);
    resolutionRow->addWidget(maintainAspectRatio);
    resolutionRow->addStretch();
    videoLayout->addLayout(resolutionRow);
    
    mainLayout->addWidget(videoGroup);
    
    // Convert button
    convertBtn = new QPushButton("Convert to Video", this);
    convertBtn->setMinimumHeight(40);
    convertBtn->setMaximumWidth(300);  // Limit button width
    convertBtn->setStyleSheet("QPushButton { font-weight: bold; font-size: 14px; }");
    
    // Center the button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(convertBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    previewCmdBtn = new QPushButton("Show FFmpeg Command", this);
    previewCmdBtn->setMaximumWidth(300);
    previewCmdBtn->setStyleSheet("QPushButton { font-size: 13px; }");

    // Center it
    QHBoxLayout *previewLayout = new QHBoxLayout();
    previewLayout->addStretch();
    previewLayout->addWidget(previewCmdBtn);
    previewLayout->addStretch();
    mainLayout->addLayout(previewLayout);
    
    mainLayout->addStretch();
}

void MainWindow::setupVideoToSequenceTab()
{
    videoToSeqTab = new QWidget();
    tabWidget->addTab(videoToSeqTab, "Video → Image Sequence");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(videoToSeqTab);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Input/Output section for video tab - more compact
    QGroupBox *ioGroup = new QGroupBox("Input/Output", this);
    QVBoxLayout *ioLayout = new QVBoxLayout(ioGroup);
    ioLayout->setSpacing(8);
    
    // Video input row
    QHBoxLayout *videoInputLayout = new QHBoxLayout();
    videoInputLayout->addWidget(new QLabel("Input Video:"));
    videoInputEdit = new DropLineEdit(this);
    videoInputEdit->setPlaceholderText("Select video file...");
    videoInputLayout->addWidget(videoInputEdit, 1);
    QPushButton *videoBrowseBtn = new QPushButton("Browse...", this);
    videoBrowseBtn->setMaximumWidth(80);
    videoInputLayout->addWidget(videoBrowseBtn);
    ioLayout->addLayout(videoInputLayout);
    
    // Sequence output row
    QHBoxLayout *seqOutputLayout = new QHBoxLayout();
    seqOutputLayout->addWidget(new QLabel("Output Directory:"));
    seqOutputEdit = new DropLineEdit(this);
    seqOutputEdit->setPlaceholderText("Select output directory...");
    seqOutputLayout->addWidget(seqOutputEdit, 1);
    QPushButton *seqBrowseBtn = new QPushButton("Browse...", this);
    seqBrowseBtn->setMaximumWidth(80);
    seqOutputLayout->addWidget(seqBrowseBtn);
    ioLayout->addLayout(seqOutputLayout);
    
    mainLayout->addWidget(ioGroup);
    
    // Image settings - more compact
    QGroupBox *imageGroup = new QGroupBox("Image Settings", this);
    QVBoxLayout *imageLayout = new QVBoxLayout(imageGroup);
    imageLayout->setSpacing(8);
    
    // Format row
    QHBoxLayout *formatRow = new QHBoxLayout();
    formatRow->addWidget(new QLabel("Format:"));
    imageFormatCombo = new QComboBox(this);
    imageFormatCombo->addItems({"PNG", "JPEG", "TIFF", "BMP", "EXR"});
    imageFormatCombo->setMaximumWidth(100);
    formatRow->addWidget(imageFormatCombo);
    formatRow->addStretch();
    imageLayout->addLayout(formatRow);
    
    // Extract all frames checkbox
    extractAllFrames = new QCheckBox("Extract All Frames", this);
    extractAllFrames->setChecked(true);
    imageLayout->addWidget(extractAllFrames);
    
    // Frame range row
    QHBoxLayout *frameRangeRow = new QHBoxLayout();
    frameRangeRow->addWidget(new QLabel("Start Frame:"));
    startFrameSpinBox = new QSpinBox(this);
    startFrameSpinBox->setRange(0, 999999);
    startFrameSpinBox->setEnabled(false);
    startFrameSpinBox->setMaximumWidth(80);
    frameRangeRow->addWidget(startFrameSpinBox);
    
    frameRangeRow->addSpacing(20);
    frameRangeRow->addWidget(new QLabel("End Frame:"));
    endFrameSpinBox = new QSpinBox(this);
    endFrameSpinBox->setRange(0, 999999);
    endFrameSpinBox->setValue(100);
    endFrameSpinBox->setEnabled(false);
    endFrameSpinBox->setMaximumWidth(80);
    frameRangeRow->addWidget(endFrameSpinBox);
    frameRangeRow->addStretch();
    imageLayout->addLayout(frameRangeRow);
    
    mainLayout->addWidget(imageGroup);
    
    // Convert button
    convertVideoBtn = new QPushButton("Convert to Image Sequence", this);
    convertVideoBtn->setMinimumHeight(40);
    convertVideoBtn->setMaximumWidth(300);
    convertVideoBtn->setStyleSheet("QPushButton { font-weight: bold; font-size: 14px; }");
    
    // Center the button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(convertVideoBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    QPushButton *previewVideoCmdBtn = new QPushButton("Show FFmpeg Command", this);
    previewVideoCmdBtn->setMaximumWidth(300);
    previewVideoCmdBtn->setStyleSheet("QPushButton { font-size: 13px; }");

    // Center the button
    QHBoxLayout *previewLayout = new QHBoxLayout();
    previewLayout->addStretch();
    previewLayout->addWidget(previewVideoCmdBtn);
    previewLayout->addStretch();
    mainLayout->addLayout(previewLayout);

    connect(previewVideoCmdBtn, &QPushButton::clicked, this, &MainWindow::showVideoToSequenceCommandPreview);

    videoInputEdit->setAcceptDrops(true);
    inputPathEdit->setAcceptDrops(true);
    seqOutputEdit->setAcceptDrops(true);
    outputPathEdit->setAcceptDrops(true);
    
    mainLayout->addStretch();
    
    // Connect video tab signals
    connect(videoBrowseBtn, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, 
            "Select Video File", 
            QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
            "Video Files (*.mp4 *.avi *.mov *.mkv *.webm *.flv *.wmv)");
        if (!fileName.isEmpty()) {
            videoInputEdit->setText(fileName);
        }
    });

    connect(seqBrowseBtn, &QPushButton::clicked, [this]() {
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
    connect(convertVideoBtn, &QPushButton::clicked, this, &MainWindow::startVideoToSequenceConversion);
    connect(previewCmdBtn, &QPushButton::clicked, this, &MainWindow::showFFmpegCommandPreview);

    
    connect(frameRateSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::updateFrameRateDisplay);
    connect(qualitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::updateQualityDisplay);
    
    connect(converter, &Converter::progressChanged, this, &MainWindow::onConversionProgress);
    connect(converter, &Converter::finished, this, &MainWindow::onConversionFinished);
    connect(converter, &Converter::logMessage, [this](const QString &message) {
        logOutput->append(message);
    });
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
    QString extension = "." + videoFormatCombo->currentText().toLower();
    if (!outputPath.endsWith(extension, Qt::CaseInsensitive)) {
        outputPath += extension;
        outputPathEdit->setText(outputPath); // Optionally update field
    }

    
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

void MainWindow::startVideoToSequenceConversion()
{
    if (isConverting) {
        converter->cancel();
        return;
    }

    QString inputVideo = videoInputEdit->text();
    QString outputDir = seqOutputEdit->text();

    if (inputVideo.isEmpty() || outputDir.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select both input video and output directory.");
        return;
    }

    ConversionSettings settings;
    settings.inputPath = inputVideo;
    settings.outputPath = outputDir;
    settings.imageFormat = imageFormatCombo->currentText();
    settings.extractAllFrames = extractAllFrames->isChecked();
    settings.startFrame = startFrameSpinBox->value();
    settings.endFrame = endFrameSpinBox->value();

    logOutput->clear();
    progressBar->setVisible(true);
    progressBar->setValue(0);
    convertVideoBtn->setText("Cancel");
    isConverting = true;

    converter->convertVideoToSequence(settings);
}

void MainWindow::onConversionProgress(int percentage)
{
    progressBar->setValue(percentage);
}

void MainWindow::onConversionFinished(bool success, const QString &message)
{
    progressBar->setVisible(false);
    convertBtn->setText("Convert to Video");
    if (convertVideoBtn) {
        convertVideoBtn->setText("Convert to Image Sequence");
    }
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

void MainWindow::showFFmpegCommandPreview()
{
    QString inputPath = inputPathEdit->text();
    QString outputPath = outputPathEdit->text();

    if (inputPath.isEmpty() || outputPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please set both input and output paths.");
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

    QStringList args = converter->buildFFmpegArguments(settings, true);
    QString command = converter->isFFmpegAvailable() ? converter->findFFmpegPath() + " " + args.join(" ") : "ffmpeg not found";

    QMessageBox::information(this, "FFmpeg Command Preview", command);
}

void MainWindow::showVideoToSequenceCommandPreview()
{
    QString inputVideo = videoInputEdit->text();
    QString outputDir = seqOutputEdit->text();

    if (inputVideo.isEmpty() || outputDir.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select both input video and output directory.");
        return;
    }

    ConversionSettings settings;
    settings.inputPath = inputVideo;
    settings.outputPath = outputDir;
    settings.imageFormat = imageFormatCombo->currentText();
    settings.extractAllFrames = extractAllFrames->isChecked();
    settings.startFrame = startFrameSpinBox->value();
    settings.endFrame = endFrameSpinBox->value();

    QStringList args = converter->buildFFmpegArguments(settings, false);
    QString command = converter->isFFmpegAvailable() ? converter->findFFmpegPath() + " " + args.join(" ") : "ffmpeg not found";

    QMessageBox::information(this, "FFmpeg Command Preview", command);
}

void MainWindow::updateUIForMode()
{
    // Future implementation for mode-specific UI updates
}