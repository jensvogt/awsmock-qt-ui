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
    _navDataModel->appendRow(new QStandardItem("KMS"));

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

    // Reconnect timer
    _reconnectTimer = new QTimer(this);
    _reconnectTimer->setInterval(5000);

    // Connect signals
    connect(&_webSocket, &QWebSocket::connected, this, &MainWidget::OnConnected);
    connect(&_webSocket, &QWebSocket::textMessageReceived, this, &MainWidget::OnMessageReceived);
    connect(&_webSocket, &QWebSocket::disconnected, this, [this]() {
        _reconnectTimer->start();
    });
    connect(&_webSocket, &QWebSocket::errorOccurred, [this](const QAbstractSocket::SocketError error) {
        // Even if it fails to connect, ensure the timer keeps running
        if (!_reconnectTimer->isActive()) {
            _reconnectTimer->start();
        }
    });
    connect(_reconnectTimer, &QTimer::timeout, this, [this]() {
        _webSocket.open(QUrl(_websocketUrl));
    });

    // Open the connection
    _webSocket.open(QUrl(_websocketUrl));

    // Data model
    _logDataModel = new QStandardItemModel(_ui->logListView);
    _ui->logListView->setModel(_logDataModel);

    // Scroll button
    _ui->scrollButton->setText(nullptr);
    _ui->scrollButton->setIcon(IconUtils::GetIcon("scroll"));
    _ui->scrollButton->setToolTip("Start/stop scrolling");
    connect(_ui->scrollButton, &QPushButton::toggled, this, [this](const bool value) {
        _scrolling = value;
    });

    // Clear button
    _ui->logClearButton->setText(nullptr);
    _ui->logClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->logClearButton->setToolTip("Clear the log message list");
    connect(_ui->logClearButton, &QPushButton::clicked, [this]() {
        _logDataModel->removeRows(0, _logDataModel->rowCount());
    });

    // Reconnect button
    _ui->reconnectButton->setText(nullptr);
    _ui->reconnectButton->setIcon(IconUtils::GetIcon("reconnect"));
    _ui->reconnectButton->setToolTip("Reconnect to the server websocket");
    connect(_ui->reconnectButton, &QPushButton::clicked, this, [this]() {
        if (_webSocket.state() == QAbstractSocket::ConnectedState) {
            _webSocket.disconnected();
            _webSocket.close();
        }
        _reconnectTimer->start();
    });
}

void MainWidget::OnConnected() const {
    _reconnectTimer->stop();

    const auto item = new QStandardItem("Connect to server websocket, URL: " + _websocketUrl);
    _logDataModel->appendRow(item);

    // Auto-scroll to bottom
    _ui->logListView->scrollToBottom();
}

void MainWidget::OnMessageReceived(const QString &message) const {

    // Add item with colors
    const auto item = new QStandardItem(message);
    if (message.contains("[error]")) {
        item->setForeground(Qt::red);
    } else if (message.contains("[warning]")) {
        item->setForeground(Qt::darkYellow);
    } else if (message.contains("[debug]")) {
        item->setForeground(Qt::green);
    }
    _logDataModel->appendRow(item);

    // Auto-scroll to bottom
    if (_scrolling) {
        _ui->logListView->scrollToBottom();
    }
}

void MainWidget::SetupStatusbar() {

    // Set base URL
    const auto baseUrl = Configuration::instance().GetValue<QString>("server.base-url");
    _ui->connectionLabel->setText(baseUrl);
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, [this](const QString &key, const QString &value) {
        if (key == "server.base-url") {
            _ui->connectionLabel->setText(value);
        }
    });

    // Set status
    connect(&EventBus::instance(), &EventBus::TimerSignal, [this](const QString &, const qint64 elapsed) {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]";
        _ui->statusLabel->setText(msg);
    });

    // Set ping status
    _ui->pingStatusLabel->setText(nullptr);
    _ui->pingStatusLabel->setPixmap(IconUtils::GetIcon("connected").pixmap(20, 20));
    connect(&EventBus::instance(), &EventBus::PingSignal, [this](const bool success) {
        if (success) {
            _ui->pingStatusLabel->setPixmap(IconUtils::GetIcon("connected").pixmap(20, 20));
        } else {
            _ui->pingStatusLabel->setPixmap(IconUtils::GetIcon("disconnected").pixmap(20, 20));
        }
    });
}
