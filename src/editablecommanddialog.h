// This code is provided as is, without warranty of any kind.
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
