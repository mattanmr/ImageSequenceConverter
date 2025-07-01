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
// editablecommanddialog.cpp
// Implements EditableCommandDialog, a dialog for editing and copying custom FFmpeg commands.

#include "editablecommanddialog.h"
#include <QHBoxLayout>
#include <QClipboard>
#include <QApplication>

// Constructor: sets up dialog UI and connects buttons
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

// Returns the edited command string
QString EditableCommandDialog::getCommand() const {
    return commandEdit->toPlainText();
}
