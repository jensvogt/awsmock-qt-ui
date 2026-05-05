#include <modules/sns/SNSTopicList.h>

SNSTopicList::SNSTopicList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Set region
    _region = Configuration::instance().GetValue<QString>("aws.region", "eu-central-1");

    // Connect service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::ListTopicSignal, this, &SNSTopicList::HandleListTopicSignal);
    connect(_snsService, &SNSService::ReloadMessagesSignal, this, &SNSTopicList::LoadContent);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new topic");
    connect(addButton, &QPushButton::clicked, [this]() {
        bool ok;
        if (const QString topicName = QInputDialog::getText(nullptr, "Topic Name", "Topic name:", QLineEdit::Normal, "", &ok); ok && !topicName.isEmpty()) {
            _snsService->AddTopic(_region, topicName);
        }
    });

    // Toolbar purge action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all topics");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _snsService->PurgeAllTopics();
        new Awsmock::Components::ToastOverlay("Purge all topics initiated.\nChanges may take some time to propagate.", this);
    });

    // Toolbar reset counter action
    auto *resetCounterButton = new QPushButton(IconUtils::GetIcon("reset-counter"), "", this);
    resetCounterButton->setToolTip("Refresh the queue message counters");
    connect(resetCounterButton, &QPushButton::clicked, [this]() {
        _snsService->ResetMessageCounters();
        LoadContent();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the topic list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(resetCounterButton);
    toolBar->addWidget(refreshButton);

    // Table
    const QStringList headers = QStringList() = {tr("Name"), tr("Available"), tr("Send"), tr("Resend"), tr("Size"), tr("Created"), tr("Modified"), tr("TopicArn")};

    _tableView = new PageableTable();
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({7});
    _tableView->SetSortColumn(1, "messages");
    _tableView->SetSortDirection(-1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN and URL
        QMap<QString, QString> arguments;
        arguments["topicArn"] = _tableView->GetValue<QString>(index, 7);

        // Send notification
        emit EventBus::instance().RouteChanged("SNS Message List", arguments);
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &SNSTopicList::ShowContextMenu);

    // Connect paging changes
    connect(_tableView, &PageableTable::ReloadTable, this, &SNSTopicList::LoadContent);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto topicArn = _tableView->GetValue<QString>(index, 7);
        SNSTopicDetailsDialog dialog(topicArn);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

SNSTopicList::~SNSTopicList() {
    StopAutoUpdate();
}

void SNSTopicList::LoadContent() {
    _snsService->ListTopics(_tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SNSTopicList::HandleListTopicSignal(const SNSListTopicResult &listTopicResult) const {
    _tableView->SaveSelection();
    _tableView->setUpdatesEnabled(false);
    _tableView->ClearContent();
    _tableView->SetTotalSize(listTopicResult.total);
    for (auto r = 0, c = 0; r < listTopicResult.topicCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listTopicResult.topicCounters.at(r).topicName);
        _tableView->SetColumn(r, c++, listTopicResult.topicCounters.at(r).messages);
        _tableView->SetColumn(r, c++, listTopicResult.topicCounters.at(r).messagesSend);
        _tableView->SetColumn(r, c++, listTopicResult.topicCounters.at(r).messagesResend);
        _tableView->SetColumn(r, c++, StringUtils::FormatSizeColumn(listTopicResult.topicCounters.at(r).size, 1), Qt::AlignRight | Qt::AlignVCenter);
        _tableView->SetColumn(r, c++, listTopicResult.topicCounters.at(r).created);
        _tableView->SetColumn(r, c++, listTopicResult.topicCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listTopicResult.topicCounters.at(r).topicArn);
    }
    _tableView->setUpdatesEnabled(true);
    _tableView->UpdateSorting();
    _tableView->RestoreSelection();
}

void SNSTopicList::ShowContextMenu(const QPoint &pos) {

    StopAutoUpdate();

    const QModelIndex index = _tableView->GetIndexFromPosition(pos);
    const long total = _tableView->GetValue<long>(index, 1) + _tableView->GetValue<long>(index, 2) + _tableView->GetValue<long>(index, 3);

    QMenu *menu = new ContextMenu(this);

    QAction *sendAction = menu->addAction(IconUtils::GetIcon("send"), "Send a Message");
    sendAction->setToolTip("Send a message to the topic.");

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Topic");
    editAction->setToolTip("Edit the topic details.");

    QAction *resendAction = menu->addAction(IconUtils::GetIcon("resend"), "Resend All Messages");
    resendAction->setToolTip("Resend all messages in the topic.");

    menu->addSeparator();

    QAction *purgeAction = menu->addAction(IconUtils::GetIcon("purge"), "Purge Topic");
    purgeAction->setToolTip("Purge the topic.");
    purgeAction->setEnabled(total > 0);

    menu->addSeparator();

    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Topic");
    deleteAction->setToolTip("Delete the topic.");

    const auto topicArn = _tableView->GetValue<QString>(index, 7);
    if (const QAction *selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == purgeAction) {
        _snsService->PurgeTopic(topicArn);
        new Awsmock::Components::ToastOverlay("Purge topic initiated.\nChanges may take some time to propagate.", this);
    } else if (selectedAction == sendAction) {
        if (SNSMessageAddDialog dialog(topicArn); dialog.exec() == QDialog::Accepted) {
            new Awsmock::Components::ToastOverlay("Message sent!\nMessageId: " + dialog.GetMessageId(), _tableView);
        }
    } else if (selectedAction == resendAction) {
        _snsService->ResendTopic(topicArn);
        new Awsmock::Components::ToastOverlay("Messages resend initiated.\nChanges may take some time to propagate.", this);
    } else if (selectedAction == deleteAction) {
        _snsService->DeleteTopic(topicArn);
        new Awsmock::Components::ToastOverlay("Topic deletion initiated.\nChanges may take some time to propagate.", this);
    } else if (selectedAction == editAction) {
        SNSTopicDetailsDialog dialog(topicArn);
        dialog.exec();
    }
    StartAutoUpdate();
}
