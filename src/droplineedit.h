// droplineedit.h
#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>

class DropLineEdit : public QLineEdit {
    Q_OBJECT
public:
    DropLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {
        setAcceptDrops(true);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) event->acceptProposedAction();
    }

    void dropEvent(QDropEvent *event) override {
        const QList<QUrl> urls = event->mimeData()->urls();
        if (!urls.isEmpty()) {
            setText(urls.first().toLocalFile());
        }
    }
};
