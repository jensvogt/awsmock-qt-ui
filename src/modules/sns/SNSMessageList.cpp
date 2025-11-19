
#include <modules/sns/SNSMessageList.h>

#include "utils/IconUtils.h"

SNSMessageList::SNSMessageList(const QString &title, const QString &topicArn, QWidget *parent) : BasePage(parent), topicArn(topicArn) {

    // Connect service
    _snsService = new SNSService();
    connect(_snsService, &SNSService::ListMessagesSignal, this, &SNSMessageList::HandleListMessageSignal);
    connect(_snsService, &SNSService::ReloadMessagesSignal, this, &SNSMessageList::HandleReloadMessageSignal);

    // Toolbar
    const auto toolBar = new QHBoxLayout();
    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar back action
    const auto backButton = new QPushButton(IconUtils::GetIcon("dark", "back"), "");
    backButton->setIconSize(QSize(16, 16));
    backButton->setToolTip("Go back to the topic list");
    connect(backButton, &QPushButton::clicked, [this]() {
        OnBackClicked();
    });

    // Toolbar label
    const auto titleLabel = new QLabel(title);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "");
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new topic");
    connect(addButton, &QPushButton::clicked, []() {
        bool ok;
        if (const QString text = QInputDialog::getText(nullptr, "Topic Name", "Topic name:", QLineEdit::Normal, "", &ok); ok && !text.isEmpty()) {
            // AddQueue(text);
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "purge"), "");
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Purge all messages");
    connect(purgeAllButton, &QPushButton::clicked, [this,topicArn]() {
        _snsService->PurgeMessages(topicArn);
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

    tableWidget->setColumnCount(static_cast<int>(headers.count()));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSortingEnabled(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableWidget->setColumnHidden(7, true);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Get the position
        const int row = index.row();

        const QString messageId = tableWidget->item(row, 0)->text();
        if (SNSMessageDetailsDialog dialog(messageId); dialog.exec() == QDialog::Accepted) {
            qDebug() << "SQS Queue edit dialog exit";
        }
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &SNSMessageList::ShowContextMenu);

    // Save sort column
    const QHeaderView *header = tableWidget->horizontalHeader();
    connect(header, &QHeaderView::sortIndicatorChanged, this, [this](const int column, const Qt::SortOrder order) {
        _sortColumn = column;
        _sortOrder = order;
    });

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 1);
    layout->addWidget(tableWidget, 2);
}

SNSMessageList::~SNSMessageList() {
    StopAutoUpdate();
}

void SNSMessageList::LoadContent() {
    if (Configuration::instance().GetConnectionState()) {
        _snsService->ListMessages(topicArn, prefixValue);
    } else {
        QMessageBox::critical(nullptr, "Error", "Backend is not reachable");
    }
}

void SNSMessageList::HandleListMessageSignal(const SNSListMessagesResult &listMessageResult) {

    const int selectedRow = tableWidget->selectionModel()->currentIndex().row();
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    tableWidget->sortItems(-1);
    for (auto r = 0, c = 0; r < listMessageResult.messageCounters.count(); r++, c = 0) {

        tableWidget->insertRow(r);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).messageId);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).contentType);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).size);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).messageStatus);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).lastSend);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).created);
        SetColumn(tableWidget, r, c++, listMessageResult.messageCounters.at(r).modified);
        SetHiddenColumn(tableWidget, r, c, listMessageResult.messageCounters.at(r).topicArn);
    }
    tableWidget->setRowCount(static_cast<int>(listMessageResult.messageCounters.count()));
    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(_sortColumn, _sortOrder);
    tableWidget->selectRow(selectedRow);
    NotifyStatusBar();
}

void SNSMessageList::HandleReloadMessageSignal() {
    _snsService->ListMessages(topicArn, prefixValue);
    NotifyStatusBar();
}

void SNSMessageList::ShowContextMenu(const QPoint &pos) const {

    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    //    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Message");
    deleteAction->setToolTip("Delete the message");
    if (const auto selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == deleteAction) {
        const QString messageId = tableWidget->item(row, 0)->text();
        _snsService->DeleteMessage(topicArn, messageId);
    }
}
