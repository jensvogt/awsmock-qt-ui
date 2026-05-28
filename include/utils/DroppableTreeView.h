//
// Created by vogje01 on 2/10/26.
//

#ifndef AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H
#define AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H

// Qt includes
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QSortFilterProxyModel>
#include <QTreeView>

#include "Logging.h"

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
            if (QString filePath = url.toLocalFile(); !filePath.isEmpty()) {
                emit FileDropped(filePath);
            }
        }
        event->acceptProposedAction();
    }

    void startDrag(Qt::DropActions supportedActions) override {
        QModelIndex proxyIndex = currentIndex();
        if (!proxyIndex.isValid())
            return;

        proxyIndex = proxyIndex.siblingAtColumn(0);
        const QAbstractItemModel *model = this->model();

        QModelIndex sourceIndex = proxyIndex;

        // If model is a proxy, map it
        if (const auto proxy = qobject_cast<const QSortFilterProxyModel *>(model)) {
            sourceIndex = proxy->mapToSource(proxyIndex);
            model = proxy->sourceModel();
        }

        // Qt::UserRole holds the path for QStandardItemModel items;
        // QFileSystemModel stores it at FilePathRole (Qt::UserRole + 1).
        QString filePath = model->data(sourceIndex, Qt::UserRole).toString();
        if (filePath.isEmpty())
            filePath = model->data(sourceIndex, Qt::UserRole + 1).toString();
        if (!filePath.isEmpty()) {
            auto *mimeData = new QMimeData;
            mimeData->setUrls({QUrl::fromLocalFile(filePath)});

            auto *drag = new QDrag(this);
            drag->setMimeData(mimeData);

            drag->exec(Qt::CopyAction);
        } else {
            logWarning << "Empty file path: row: " + QString::number(sourceIndex.row());
        }
    }
};


#endif // AWSMOCK_QT_UI_DROPPABLE_TREE_VIEW_H
