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
// editablecommanddialog.h
// Dialog for editing and copying custom FFmpeg commands.
#ifndef EDITABLECOMMANDDIALOG_H
#define EDITABLECOMMANDDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

// EditableCommandDialog: allows user to edit/copy a generated FFmpeg command
class EditableCommandDialog : public QDialog {
    Q_OBJECT

public:
    EditableCommandDialog(const QString &command, QWidget *parent = nullptr);
    // Returns the edited command
    QString getCommand() const;

private:
    QPlainTextEdit *commandEdit; // Text area for command
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QPushButton *copyBtn;
};

#endif // EDITABLECOMMANDDIALOG_H
