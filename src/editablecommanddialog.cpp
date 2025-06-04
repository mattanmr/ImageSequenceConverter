#include "editablecommanddialog.h"
#include <QHBoxLayout>
#include <QClipboard>
#include <QApplication>

EditableCommandDialog::EditableCommandDialog(const QString &command, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Edit FFmpeg Command");
    resize(600, 300);

    commandEdit = new QPlainTextEdit(this);
    commandEdit->setPlainText(command);

    okBtn = new QPushButton("Use This Command", this);
    cancelBtn = new QPushButton("Cancel", this);
    copyBtn = new QPushButton("Copy to Clipboard", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(copyBtn);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(commandEdit);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(copyBtn, &QPushButton::clicked, [this]() {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(commandEdit->toPlainText());
    });
}

QString EditableCommandDialog::getCommand() const {
    return commandEdit->toPlainText();
}
