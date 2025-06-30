// This code is provided as is, without warranty of any kind.
// droplineedit.cpp
// Implements DropLineEdit, a QLineEdit subclass that accepts file/folder drag-and-drop.

#include "droplineedit.h"
#include <QMimeData>
#include <QUrl>

// Constructor: enables drag-and-drop
DropLineEdit::DropLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setAcceptDrops(true);
}

// Accepts drag if it contains URLs (files/folders)
void DropLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

// Sets the line edit text to the dropped file/folder path
void DropLineEdit::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        setText(urls.first().toLocalFile());
    }
}
