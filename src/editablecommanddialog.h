#ifndef EDITABLECOMMANDDIALOG_H
#define EDITABLECOMMANDDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class EditableCommandDialog : public QDialog {
    Q_OBJECT

public:
    EditableCommandDialog(const QString &command, QWidget *parent = nullptr);
    QString getCommand() const;

private:
    QPlainTextEdit *commandEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QPushButton *copyBtn;
};

#endif // EDITABLECOMMANDDIALOG_H
