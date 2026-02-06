//
// Created by vogje01 on 2/6/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWidget.h" resolved

#include <MainWidget.h>
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::MainWidget) {

    // Get the websocket URL for logging
    _websocketUrl = Configuration::instance().GetValue<QString>("server.websocket-url");

    // Set UI components
    _ui->setupUi(this);

    // Setup navigation pane
    SetupNavPane();

    // Setup content pane
    SetupContentPane();

    // Setup logging pane
    SetupLogPane();

    // Setup status bar
    SetupStatusbar();
}

MainWidget::~MainWidget() {
    delete _ui;
}

void MainWidget::SetupNavPane() {

    // Create the Navigation Pane (QListWidget)
    _ui->navigationListView->setGridSize(QSize(0, 20));

    // Data model
    _navDataModel = new QStandardItemModel(_ui->navigationListView);
    _ui->navigationListView->setModel(_navDataModel);
    _ui->navigationListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    _navDataModel->appendRow(new QStandardItem("Dashboard"));
    _navDataModel->appendRow(new QStandardItem("SQS"));
    _navDataModel->appendRow(new QStandardItem("SNS"));
    _navDataModel->appendRow(new QStandardItem("S3"));
    _navDataModel->appendRow(new QStandardItem("Application"));
    _navDataModel->appendRow(new QStandardItem("Lambda"));
    _navDataModel->appendRow(new QStandardItem("Secrets Manager"));
    _navDataModel->appendRow(new QStandardItem("Systems Manager"));
    _navDataModel->appendRow(new QStandardItem("DynamoDB"));

    // Set current index
    _ui->navigationListView->setCurrentIndex(_ui->navigationListView->model()->index(0, 0));
    connect(_ui->navigationListView, &QListView::clicked, [this](const QModelIndex &index) {
        const QString name = _navDataModel->item(index.row())->text();
        MainRouter::instance().SetRoute(name);
    });
}

void MainWidget::SetupContentPane() {
    _contentPane = new QStackedWidget(this);
    _contentPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _ui->horizontalSplitter->addWidget(_contentPane);

    // Set default page
    MainRouter::instance().SetContentPane(_contentPane);
    MainRouter::instance().SetRoute("Dashboard");
}

void MainWidget::SetupLogPane() {

    // Connect signals
    connect(&_webSocket, &QWebSocket::connected, this, &MainWidget::OnConnected);
    connect(&_webSocket, &QWebSocket::textMessageReceived, this, &MainWidget::OnMessageReceived);
    connect(&_webSocket, &QWebSocket::errorOccurred, [](const QAbstractSocket::SocketError error) {
        qDebug() << "Socket Error:" << error;
    });

    // Open the connection
    _webSocket.open(QUrl(_websocketUrl));

    // Data model
    _logDataModel = new QStringListModel(_ui->logListView);
    _ui->logListView->setModel(_logDataModel);

    // Clear button
    _ui->logClearButton->setText(nullptr);
    _ui->logClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->logClearButton->setToolTip("Clear the log message list");
    connect(_ui->logClearButton, &QPushButton::clicked, [this]() {
        _logDataModel->removeRows(0, _logDataModel->rowCount());
    });

    // Scroll button
    _ui->scrollButton->setText(nullptr);
    _ui->scrollButton->setIcon(IconUtils::GetIcon("scroll"));
    _ui->scrollButton->setToolTip("Start/stop scrolling");
    connect(_ui->scrollButton, &QPushButton::toggled, this, [this](const bool value) {
        _scrolling = value;
    });
}

void MainWidget::OnConnected() const {
    if (const auto model = dynamic_cast<QStringListModel *>(_ui->logListView->model())) {
        const int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row), "Connect to server websocket, URL: " + _websocketUrl);

        // Auto-scroll to bottom
        _ui->logListView->scrollToBottom();
    }
}

void MainWidget::OnMessageReceived(const QString &message) const {
    if (const auto model = dynamic_cast<QStringListModel *>(_ui->logListView->model())) {
        const int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row), message);

        // Auto-scroll to bottom
        if (_scrolling) {
            _ui->logListView->scrollToBottom();
        }
    }
}

void MainWidget::SetupStatusbar() {
    const auto baseUrl = Configuration::instance().GetValue<QString>("server.base-url");
    _ui->connectionLabel->setText(baseUrl);
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, [this](const QString &key, const QString &value) {
        if (key == "server.base-url") {
            _ui->connectionLabel->setText(value);
        }
    });

    connect(&EventBus::instance(), &EventBus::TimerSignal, [this](const QString &, const qint64 elapsed) {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]";
        _ui->statusLabel->setText(msg);
    });
}
