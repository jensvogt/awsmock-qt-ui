//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_DROP_FIELD_H
#define AWSMOCK_QT_UI_DROP_FIELD_H

#include <QLabel>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

class DropField final : public QLabel {
    Q_OBJECT

public:
    explicit DropField(QWidget *parent = nullptr) : QLabel(parent) {
        setLayout(new QHBoxLayout());
        setText("Drop files here");
        setAlignment(Qt::AlignCenter);
        setAcceptDrops(true);
        resize(300,300);
        setStyleSheet("QLabel { border: 2px dashed gray; }");
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override {
        QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty())
            return;

        QStringList filePaths;
        for (const QUrl &url : urls)
            filePaths << url.toLocalFile();

        setText(filePaths.join("\n")); // Just show dropped files for now
    }
};

#endif //AWSMOCK_QT_UI_DROP_FIELD_H