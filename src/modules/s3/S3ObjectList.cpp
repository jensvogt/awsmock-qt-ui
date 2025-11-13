
#include <QHeaderView>
#include <modules/s3/S3ObjectList.h>

S3ObjectList::S3ObjectList(const QString &title, const QString &bucketName, QWidget *parent) : BasePage(parent), bucketName(bucketName) {
    // Connect service
    _s3Service = new S3Service();
    connect(_s3Service, &S3Service::ListObjectsSignal, this, &S3ObjectList::HandleListObjectSignal);
    connect(_s3Service, &S3Service::ReloadObjectsSignal, this, &S3ObjectList::HandleReloadObjectSignal);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("dark", "back"), "");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Add a new Queue");
    connect(backButton, &QPushButton::clicked, [this]() {
        StopAutoUpdate();
        emit BackToBucketList();
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new Queue");
    connect(addButton, &QPushButton::clicked, []() {
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Queue Name", "Queue name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
            // AddQueue(text);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all Queues");
    connect(purgeAllButton, &QPushButton::clicked, [&]() {
        qDebug() << "Purge bucket: " << bucketName;
        // _s3Service->PurgeTopic(topicArn);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the queue list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    const auto prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this,prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    const QStringList headers = QStringList() << tr("ID")
                                << tr("ContentType")
                                << tr("Size")
                                << tr("Status")
                                << tr("LastSend")
                                << tr("Created")
                                << tr("Modified")
                                << tr("TopicArn");

    tableWidget = new QTableWidget();

    tableWidget->setColumnCount(headers.count());
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(7, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {

        // Get the position
        const int row = index.row();

        const QString objectId = tableWidget->item(row, 0)->text();
        /* if (S3ObjectDetailsDialog dialog(objectId); dialog.exec() == QDialog::Accepted) {
             qDebug() << "SQS Queue edit dialog exit";
         }*/
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &S3ObjectList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [=](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
}

S3ObjectList::~S3ObjectList() {
    StopAutoUpdate();
}

void S3ObjectList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        //_s3Service->ListObjects(topicArn, prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void S3ObjectList::HandleListObjectSignal(const S3ListObjectsResult &listObjectResult) {

    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    for (auto r = 0; r < listObjectResult.objectCounters.count(); r++) {

        tableWidget->insertRow(r);
        /*SetColumn(tableWidget, r, 0, listObjectResult.objectCounters.at(r).objectId);
        SetColumn(tableWidget, r, 1, listObjectResult.objectCounters.at(r).contentType);
        SetColumn(tableWidget, r, 2, listObjectResult.objectCounters.at(r).size);
        SetColumn(tableWidget, r, 3, listObjectResult.objectCounters.at(r).objectStatus);
        SetColumn(tableWidget, r, 4, listObjectResult.objectCounters.at(r).lastSend);
        SetColumn(tableWidget, r, 5, listObjectResult.objectCounters.at(r).created);
        SetColumn(tableWidget, r, 6, listObjectResult.objectCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, 7, listObjectResult.objectCounters.at(r).topicArn);*/
    }
    tableWidget->setRowCount(static_cast<int>(listObjectResult.objectCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    NotifyStatusBar();
}

void S3ObjectList::HandleReloadObjectSignal() {
    //_s3Service->ListObjects(topicArn, prefixValue);
    NotifyStatusBar();
}

void S3ObjectList::ShowContextMenu(const QPoint &pos) const {

    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    /*    QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Queue");
    purgeAction->setToolTip("Purge the Queue");
    QAction *redriveAction = menu.addAction(QIcon(":/icons/redrive.png"), "Redrive Queue");
    redriveAction->setToolTip("Redrive all objects");*/
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Object");
    deleteAction->setToolTip("Delete the object");

    /*if (selectedAction == purgeAction) {
        QString QueueUrl = tableWidget->item(row, 7)->text();
//        PurgeQueue(QueueUrl);
    } else if (selectedAction == redriveAction) {
        QString QueueUrl = tableWidget->item(row, 7)->text();
//        DeleteQueue(QueueUrl);
    } else*/
    if (const auto selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        const QString objectId = tableWidget->item(row, 0)->text();
        //_s3Service->DeleteObject(topicArn, objectId);
    }
}
