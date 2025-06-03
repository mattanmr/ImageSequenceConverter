#ifndef DROPLINEEDIT_H
#define DROPLINEEDIT_H

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>

class DropLineEdit : public QLineEdit {
    Q_OBJECT  // ← This is essential for any QObject-based class using signals/slots or polymorphism
public:
    DropLineEdit(QWidget *parent = nullptr);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};


#endif // DROPLINEEDIT_H
