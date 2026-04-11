
#include <modules/sns/SNSMessageList.h>

SNSMessageList::SNSMessageList(const QString &title, QWidget *parent) : BasePage(parent) {

    // Connect service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::ListMessagesSignal, this, &SNSMessageList::HandleListMessageSignal);
    connect(_snsService, &SNSService::ReloadMessagesSignal, this, &SNSMessageList::HandleReloadMessageSignal);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("back"), "");
    backButton->setToolTip("Go back to the topic list");
    connect(backButton, &QPushButton::clicked, []() {
        emit EventBus::instance().RouteChanged("SNS");
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("add"), "");
    addButton->setToolTip("Add a new topic");
    connect(addButton, &QPushButton::clicked, [this]() {
        SNSMessageAddDialog dialog(_topicArn);
        dialog.exec();
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("purge"), "");
    purgeAllButton->setToolTip("Purge all messages");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        _snsService->PurgeMessages(_topicArn);
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("refresh"), "");
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

    // Table
    const QStringList headers = {tr("ID"), tr("ContentType"), tr("Size"), tr("Status"), tr("LastSend"), tr("Created"), tr("Modified"), tr("TopicArn")};

    _tableView = new PageableTable();
    _tableView->SetHeaderNames(headers);
    _tableView->SetResizeModes({QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents});
    _tableView->SetHiddenColumns({7});
    _tableView->SetSortColumn(1, "created");
    _tableView->SetSortDirection(-1);

    // Connect double-click
    connect(_tableView, &PageableTable::DoubleClicked, this, [this](const QModelIndex &index) {
        const auto messageId = _tableView->GetValue<QString>(index, 0);
        SNSMessageDetailsDialog dialog(messageId, this);
        dialog.exec();
    });

    // Add context menu
    connect(_tableView, &PageableTable::ContextMenuRequested, this, &SNSMessageList::ShowContextMenu);

    // Connect paging changes
    connect(_tableView, &PageableTable::ReloadTable, this, &SNSMessageList::LoadContent);

    // Add details shortcut
    connect(_tableView, &PageableTable::ShowDetailsSignal, this, [this](const QModelIndex &index) {
        const auto messageId = _tableView->GetValue<QString>(index, 0);
        SNSMessageDetailsDialog dialog(messageId, this);
        dialog.exec();
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(_tableView, 2);
}

SNSMessageList::~SNSMessageList() {
    StopAutoUpdate();
}

void SNSMessageList::LoadContent() {
    _topicArn = GetArgument<QString>("topicArn");
    _snsService->ListMessages(_topicArn, _tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SNSMessageList::HandleListMessageSignal(const SNSListMessagesResult &listMessageResult) const {
    _tableView->Clear();
    _tableView->SetTotalSize(listMessageResult.total);
    for (auto r = 0, c = 0; r < listMessageResult.messageCounters.count(); r++, c = 0) {
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).messageId);
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).contentType);
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).size);
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).messageStatus);
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).lastSend);
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).created);
        _tableView->SetColumn(r, c++, listMessageResult.messageCounters.at(r).modified);
        _tableView->SetHiddenColumn(r, c++, listMessageResult.messageCounters.at(r).topicArn);
    }
    _tableView->UpdateSorting();
}

void SNSMessageList::HandleReloadMessageSignal() const {
    _snsService->ListMessages(_topicArn, _tableView->GetPrefix(), _tableView->GetPageSize(), _tableView->GetPageIndex(), _tableView->GetSortAttribute(), _tableView->GetSortDirection());
}

void SNSMessageList::ShowContextMenu(const QPoint &pos) {
    const QModelIndex index = _tableView->GetIndexFromPosition(pos);

    QMenu *menu = new ContextMenu(this);

    QAction *editAction = menu->addAction(IconUtils::GetIcon("edit"), "Edit Message");
    editAction->setToolTip("Edit the message");

    menu->addSeparator();
    QAction *deleteAction = menu->addAction(IconUtils::GetIcon("delete"), "Delete Message");
    deleteAction->setToolTip("Delete the message");

    if (const auto selectedAction = menu->exec(_tableView->GetGlobalPosition(pos)); selectedAction == editAction) {
        const auto messageId = _tableView->GetValue<QString>(index, 0);
        SNSMessageDetailsDialog dialog(messageId, this);
        dialog.exec();
    } else if (selectedAction == deleteAction) {
        const auto messageId = _tableView->GetValue<QString>(index, 0);
        _snsService->DeleteMessage(_topicArn, messageId);
    }
}
