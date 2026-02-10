//
// Created by vogje01 on 2/10/26.
//

#ifndef AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H
#define AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H

// Qt includes
#include <QDragEnterEvent>
#include <QMimeData>
#include <QTreeView>

class DroppableTreeView final : public QTreeView {
    Q_OBJECT

public:
    using QTreeView::QTreeView;

signals:
    void FileDropped(const QString &filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls())
            event->acceptProposedAction();
    }

    void dragMoveEvent(QDragMoveEvent *event) override {
        if (event->mimeData()->hasUrls())
            event->acceptProposedAction();
    }

    void dropEvent(QDropEvent *event) override {
        if (!event->mimeData()->hasUrls())
            return;

        for (const QUrl &url: event->mimeData()->urls()) {
            if (QString filePath = url.toLocalFile(); !filePath.isEmpty()) {
                emit FileDropped(filePath);
            }
        }
        event->acceptProposedAction();
    }
};


#endif // AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H
