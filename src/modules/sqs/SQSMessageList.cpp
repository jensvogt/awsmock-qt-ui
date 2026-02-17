
#include <modules/sqs/SQSMessageList.h>

SQSMessageList::SQSMessageList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service events
    _sqsService = new SQSService();
    connect(_sqsService, &SQSService::ListMessagesSignal, this, &SQSMessageList::HandleListMessageSignal);
    connect(_sqsService, &SQSService::ReloadMessagesSignal, this, &SQSMessageList::LoadContent);

    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("dark", "back"), "");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Go back to the queue list");
    connect(backButton, &QPushButton::clicked, []() {
        emit EventBus::instance().RouteChanged("SQS");
    });

    // Toolbar label
    _titleLabel = new QLabel(title);

    // Toolbar add message action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new message");
    connect(addButton, &QPushButton::clicked, [this]() {
        SQSMessageAddDialog dialog(_queueUrl);
        dialog.exec();
    });

    // Toolbar purge action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all messages");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _sqsService->PurgeAllMessages(_queueUrl);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "");
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the message list");
    connect(refreshButton, &QPushButton::clicked, [this]() {
        LoadContent();
    });

    toolBar->addWidget(backButton);
    toolBar->addWidget(_titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {
                                    tr("ID"), tr("ContentType"), tr("Size"), tr("Retries"), tr("Created"),
                                    tr("Modified"), tr("QueueUrl"), tr("QueueArn"), tr("ReceiptHandle")
                                };

    _tableView = new PageableTable();
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Interactive, QHeaderView::Interactive, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({6, 7, 8});
    _tableView->SetSorting(4, "created", -1);
    _tableView->SetMultiRowSelection(true);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {
        const auto messageId = _tableView->GetValue<QString>(index, 0);
        SQSMessageDetailsDialog dialog(messageId, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &SQSMessageList::ShowContextMenu);

    // Connect paging changes
    connect(_tableView, &PageableTable::ReloadTable, this, &SQSMessageList::LoadContent);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 6);
}

SQSMessageList::~SQSMessageList() {
    StopAutoUpdate();
}

void SQSMessageList::LoadContent() {
    _queueArn = GetArgument<QString>("queueArn");
    _queueUrl = GetArgument<QString>("queueUrl");
    _isDlq = GetArgument<bool>("isDlq");
    _titleLabel->setText(QString("SQS Message List: %1").arg(AwsUtils::ArnToName(_queueArn)));
    _sqsService->ListMessages(_queueArn, _tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SQSMessageList::HandleListMessageSignal(const SQSListMessagesResponse &listMessageResponse) const {
    _tableView->Clear();
    _tableView->SetTotalSize(listMessageResponse.total);
    for (auto r = 0, c = 0; r < listMessageResponse.messageCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).messageId);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).contentType);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).size);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).retries);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).created);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).modified);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).queueUrl);
        _tableView->SetColumn(r, c++, listMessageResponse.messageCounters.at(r).queueArn);
        _tableView->SetColumn(r, c, listMessageResponse.messageCounters.at(r).receiptHandle);
    }
    _tableView->UpdateSorting();
}

void SQSMessageList::HandleBulkDelete(const QModelIndexList &proxyIndices) const {

    // Convert to Persistent Source Indexes
    QList<QPersistentModelIndex> persistentRows;
    for (const QModelIndex &proxyIdx: proxyIndices) {
        persistentRows.append(_tableView->GetSourceIndex(proxyIdx));
    }

    // Now it is safe to delete in a loop
    for (const QPersistentModelIndex &srcIdx: persistentRows) {
        if (srcIdx.isValid()) {
            const auto queueUrl = _tableView->GetValue<QString>(srcIdx, 6);
            const auto receiptHandle = _tableView->GetValue<QString>(srcIdx, 8);
            _sqsService->DeleteMessage(queueUrl, receiptHandle);
            _tableView->RemoveRow(srcIdx);
        }
    }
    logInfo << "Deleted messages, count: " << proxyIndices.count();
}

void SQSMessageList::HandleBulkResend(const QModelIndexList &proxyIndices) const {

    // Convert to Persistent Source Indexes
    QList<QPersistentModelIndex> persistentRows;
    for (const QModelIndex &proxyIdx: proxyIndices) {
        persistentRows.append(_tableView->GetSourceIndex(proxyIdx));
    }

    // Now it is safe to delete in a loop
    for (const QPersistentModelIndex &srcIdx: persistentRows) {
        if (srcIdx.isValid()) {
            const auto queueArn = _tableView->GetValue<QString>(srcIdx, 7);
            const auto messageId = _tableView->GetValue<QString>(srcIdx, 0);
            _sqsService->ResendMessage(queueArn, messageId);
        }
    }
    logInfo << "Resend messages, count: " << proxyIndices.count();
}

void SQSMessageList::HandleBulkRedrive(const QModelIndexList &proxyIndices) const {

    // Convert to Persistent Source Indexes
    QList<QPersistentModelIndex> persistentRows;
    for (const QModelIndex &proxyIdx: proxyIndices) {
        persistentRows.append(_tableView->GetSourceIndex(proxyIdx));
    }

    // Now it is safe to delete in a loop
    for (const QPersistentModelIndex &srcIdx: persistentRows) {
        if (srcIdx.isValid()) {
            const auto queueArn = _tableView->GetValue<QString>(srcIdx, 7);
            const auto messageId = _tableView->GetValue<QString>(srcIdx, 0);
            _sqsService->RedriveMessage(queueArn, messageId);
        }
    }
    logInfo << "Resend messages, count: " << proxyIndices.count();
}

void SQSMessageList::ShowContextMenu(const QPoint &pos) {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    QMenu menu;
    menu.setToolTipsVisible(true);
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Message");
    editAction->setToolTip("Edit message");

    QAction *resendAction = menu.addAction(IconUtils::GetIcon("resend"), "Resend Messages");
    resendAction->setToolTip("Resend message");
    if (_isDlq) {
        resendAction->setEnabled(false);
    } else {
        resendAction->setEnabled(true);
    }
    QAction *redriveAction = menu.addAction(IconUtils::GetIcon("redrive"), "Redrive Messages");
    resendAction->setToolTip("Redrive messages");
    if (_isDlq) {
        redriveAction->setEnabled(true);
    } else {
        redriveAction->setEnabled(false);
    }
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Message");
    deleteAction->setToolTip("Delete the message");

    if (const QAction *selectedAction = menu.exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        const auto messageId = _tableView->GetValue<QString>(index, 0);
        SQSMessageDetailsDialog dialog(messageId, this);
        dialog.exec();
    } else if (selectedAction == resendAction) {
        const QModelIndexList selectedProxyIndices = _tableView->GetSelectedRows();
        HandleBulkResend(selectedProxyIndices);
    } else if (selectedAction == redriveAction) {
        const QModelIndexList selectedProxyIndices = _tableView->GetSelectedRows();
        HandleBulkRedrive(selectedProxyIndices);
    } else if (selectedAction == deleteAction) {
        const QModelIndexList selectedProxyIndices = _tableView->GetSelectedRows();
        HandleBulkDelete(selectedProxyIndices);
    }
}
