#include <QMenu>
#include <modules/application/ApplicationList.h>

#include "../../../include/modules/application/ApplicationAddDialog.h"

ApplicationList::ApplicationList(const QString &title, QWidget *parent) : BasePage(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    // Set region
    _region = Configuration::instance().GetRegion();

    // Connect service
    applicationService = new ApplicationService();
    connect(applicationService, &ApplicationService::LoadAllApplications, this, &ApplicationList::LoadContent);
    connect(applicationService, &ApplicationService::ReloadApplicationsSignal, this, &ApplicationList::HandleListApplicationsSignal);

    // Title label
    const auto titleLabel = new QLabel(title, this);

    // Define toolbar
    const auto toolBar = new QHBoxLayout();
    toolBar->addWidget(titleLabel);
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Toolbar add action
    const auto addButton = new QPushButton(IconUtils::GetIcon("dark", "add"), "", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setToolTip("Add a new application");
    connect(addButton, &QPushButton::clicked, []() {
        if (ApplicationAddDialog dialog; dialog.exec() == QDialog::Accepted) {
            qDebug() << "Application edit dialog exit";
        }
    });

    // Toolbar add action
    const auto purgeAllButton = new QPushButton(IconUtils::GetIcon("dark", "restart"), "", this);
    purgeAllButton->setIconSize(QSize(16, 16));
    purgeAllButton->setToolTip("Restart all applications");
    connect(purgeAllButton, &QPushButton::clicked, [this]() {
        applicationService->RestartAllApplications();
    });

    // Toolbar refresh action
    const auto refreshButton = new QPushButton(IconUtils::GetIcon("dark", "refresh"), "", this);
    refreshButton->setIconSize(QSize(16, 16));
    refreshButton->setToolTip("Refresh the application list");
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        LoadContent();
    });

    //    toolBar->addWidget(titleLabel);
    toolBar->addWidget(spacer);
    toolBar->addWidget(addButton);
    toolBar->addWidget(purgeAllButton);
    toolBar->addWidget(refreshButton);

    // Prefix editor
    auto prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("Prefix");
    connect(prefixEdit, &QLineEdit::returnPressed, this, [this, prefixEdit]() {
        prefixValue = prefixEdit->text();
        LoadContent();
    });

    // Table
    const QStringList headers = QStringList() << tr("Name")
                                << tr("Version")
                                << tr("Enabled")
                                << tr("Status")
                                << tr("Private Port")
                                << tr("Public Port")
                                << tr("Last Started")
                                << tr("Created")
                                << tr("Modified");

    tableWidget = new QTableWidget(this);

    tableWidget->setColumnCount(static_cast<int>(headers.count()));
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
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Interactive);

    // Connect double-click
    connect(tableWidget, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {
        // Get the position
        const int row = index.row();

        // Extract ARN and URL
        const QString name = tableWidget->item(row, 0)->text();

        // Send notification
        //emit ShowSnsMessages(topicArn);
    });

    // Add context menu
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &ApplicationList::ShowContextMenu);

    // Set up the layout for the individual content pages
    const auto layout = new QVBoxLayout(this);
    layout->addLayout(toolBar, 0);
    layout->addWidget(prefixEdit, 0);
    layout->addWidget(tableWidget, 2);
}

ApplicationList::~ApplicationList() {
    StopAutoUpdate();
}

void ApplicationList::LoadContent() {
    applicationService->ListApplications(prefixValue);
}

void ApplicationList::HandleListApplicationsSignal(const ApplicationListResponse &listApplicationResponse) {
    tableWidget->setRowCount(0);
    tableWidget->setSortingEnabled(false);
    tableWidget->sortItems(-1);
    for (auto r = 0; r < listApplicationResponse.applicationCounters.count(); r++) {
        tableWidget->insertRow(r);
        tableWidget->setItem(r, 0, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).name));
        tableWidget->setItem(r, 1, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).version));
        if (listApplicationResponse.applicationCounters.at(r).enabled) {
            auto *iconItem = new QTableWidgetItem();
            iconItem->setIcon(IconUtils::GetIcon("dark", "enabled"));
            iconItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            tableWidget->setItem(r, 2, iconItem);
        } else {
            auto *iconItem = new QTableWidgetItem();
            iconItem->setIcon(IconUtils::GetIcon("dark", "disabled"));
            iconItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            tableWidget->setItem(r, 2, iconItem);
        }
        tableWidget->setItem(r, 3, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).status));
        tableWidget->setItem(r, 4, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).privatePort));
        tableWidget->setItem(r, 5, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).publicPort));
        tableWidget->setItem(r, 6, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).created.toString(Qt::ISODate)));
        tableWidget->setItem(r, 7, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).created.toString(Qt::ISODate)));
        tableWidget->setItem(r, 8, new QTableWidgetItem(listApplicationResponse.applicationCounters.at(r).modified.toString(Qt::ISODate)));
    }
    tableWidget->setRowCount(static_cast<int>(listApplicationResponse.applicationCounters.count()));
    tableWidget->setSortingEnabled(true);
    NotifyStatusBar();
}

void ApplicationList::ShowContextMenu(const QPoint &pos) {

    StopAutoUpdate();

    // Cell index
    const QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid()) return;

    const int row = index.row();

    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("dark", "edit"), "Edit Application");
    editAction->setToolTip("Edit the Topic details");

    /*menu.addSeparator();

    QAction *purgeAction = menu.addAction(QIcon(":/icons/purge.png"), "Purge Topic");
    purgeAction->setToolTip("Purge the Topic");*/

    menu.addSeparator();

    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("dark", "delete"), "Delete Application");
    deleteAction->setToolTip("Delete the Topic");

    const QString name = tableWidget->item(row, 0)->text();
    if (const QAction *selectedAction = menu.exec(tableWidget->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        /*if (ApplicationAddDialog dialog(name); dialog.exec() == QDialog::Accepted) {
            qDebug() << "Application edit dialog exit";
        }*/
    } else if (selectedAction == deleteAction) {
        applicationService->DeleteApplication(name);
    } else if (selectedAction == editAction) {
        QString TopicArn = tableWidget->item(row, 7)->text();
        //SNSTopicDetailsDialog dialog(TopicArn);
        /*   if (dialog.exec() == QDialog::Accepted) {
               qDebug() << "SNS Topic edit dialog exit";
           }*/
    }
    StartAutoUpdate();
}
