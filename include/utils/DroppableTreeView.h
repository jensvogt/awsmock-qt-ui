//
// Created by vogje01 on 2/10/26.
//

#ifndef AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H
#define AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H

// Qt includes
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QTreeView>

class DroppableTreeView final : public QTreeView {
    Q_OBJECT

public:
    using QTreeView::QTreeView;

signals:
    void FileDropped(const QString &filePath);

    void FileDeleted(const QString &filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls())
            event->acceptProposedAction();
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Delete) {
            if (const QModelIndex index = currentIndex(); index.isValid()) {
                const QString fileName = model()->itemData(index)[0].toString();
                model()->removeRow(index.row(), index.parent());
                emit FileDeleted(fileName);
            }
            return;
        }
        QTreeView::keyPressEvent(event);
    }

    void dragMoveEvent(QDragMoveEvent *event) override {
        if (event->mimeData()->hasUrls())
            event->acceptProposedAction();
    }

    void dropEvent(QDropEvent *event) override {
        if (!event->mimeData()->hasUrls())
            return;

        for (const QUrl &url: event->mimeData()->urls()) {
            QString tmp = url.toString();
            if (QString filePath = url.toLocalFile(); !filePath.isEmpty()) {
                emit FileDropped(filePath);
            }
        }
        event->acceptProposedAction();
    }

    void startDrag(Qt::DropActions supportedActions) override {
        const QModelIndex index = currentIndex();
        if (!index.isValid())
            return;

        const QAbstractItemModel *model = this->model();
        const QString filePath = model->data(index, Qt::UserRole).toString(); // however you store it

        auto *mimeData = new QMimeData;
        mimeData->setUrls({QUrl::fromLocalFile(filePath)});

        auto *drag = new QDrag(this);
        drag->setMimeData(mimeData);

        drag->exec(Qt::CopyAction);
    }
};


#endif // AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H
