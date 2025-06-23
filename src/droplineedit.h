// droplineedit.h
// Custom QLineEdit subclass that accepts file/folder drag-and-drop for user convenience.

#ifndef DROPLINEEDIT_H
#define DROPLINEEDIT_H

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>

class DropLineEdit : public QLineEdit {
    Q_OBJECT  // Required for QObject-based classes using signals/slots
public:
    // Constructor: enables drag-and-drop
    DropLineEdit(QWidget *parent = nullptr);
protected:
    // Accepts drag if it contains URLs (files/folders)
    void dragEnterEvent(QDragEnterEvent *event) override;
    // Sets the line edit text to the dropped file/folder path
    void dropEvent(QDropEvent *event) override;
};

#endif // DROPLINEEDIT_H
