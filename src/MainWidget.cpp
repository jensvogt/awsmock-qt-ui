//
// Created by vogje01 on 2/6/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWidget.h" resolved

#include <MainWidget.h>
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::MainWidget) {

    // Setup module service
    _moduleService = new ModuleService();

    // Get the log limit
    _logLimit = Configuration::instance().GetValue<long>("ui.log-limit", _logLimit);

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
    if (_webSocket) {
        _webSocket->disconnected();
        _webSocket->deleteLater();
    }
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
    _navDataModel->appendRow(new QStandardItem("Cognito"));
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

    // Set server log level
    _moduleService->SetLogLevel(_currentLogLevel);

    // Get the log level from server
    connect(_moduleService, &ModuleService::GetLoglevelSignal, this, [this](const QString &logLevel) {
        _currentLogLevel = logLevel;
        _ui->logLevelCombo->setCurrentText(_currentLogLevel);
    });

    // Setup server logs
    SetupServerLogs();

    // Setup server logs
    SetupLocalLogs();

    // Default log tab
    _ui->logTabWidget->setCurrentIndex(0);
}

void MainWidget::SetupServerLogs() {

    // Loglevel combo
    const auto logLevelList = QStringList({"trace", "debug", "info", "warning", "error"});
    _ui->logLevelCombo->addItems(logLevelList);
    _ui->logLevelCombo->setCurrentText(_currentLogLevel);
    logInfo << "Server log level set to " << _currentLogLevel;

    connect(_ui->logLevelCombo, &QComboBox::currentTextChanged, this, [this](const QString &logLevel) {
        _moduleService->SetLogLevel(logLevel);
        _currentLogLevel = logLevel;
        logInfo << "Server log level changed to " << _currentLogLevel;
    });

    // Create websocket
    _webSocket = new QWebSocket();

    // Reconnect timer
    _reconnectTimer = new QTimer(this);
    _reconnectTimer->setInterval(5000);

    // Connect signals
    connect(_webSocket, &QWebSocket::connected, this, &MainWidget::OnConnected);
    connect(_webSocket, &QWebSocket::textMessageReceived, this, &MainWidget::OnMessageReceived);
    connect(_webSocket, &QWebSocket::disconnected, this, [this]() {
        const auto item = new QStandardItem("Disconnected from server websocket, URL: " + _websocketUrl);
        _serverLogDataModel->appendRow(item);
    });
    connect(_webSocket, &QWebSocket::errorOccurred, [this](const QAbstractSocket::SocketError error) {
        // Open the connection
        _webSocket->open(QUrl(_websocketUrl));
    });
    connect(_reconnectTimer, &QTimer::timeout, this, [this]() {
        _webSocket->open(QUrl(_websocketUrl));
    });

    // Open the connection
    _webSocket->open(QUrl(_websocketUrl));

    // Data model
    _serverLogDataModel = new QStandardItemModel(_ui->serverLogList);
    _ui->serverLogList->setModel(_serverLogDataModel);

    // Scroll button
    _ui->serverScrollButton->setText(nullptr);
    _ui->serverScrollButton->setIcon(IconUtils::GetIcon("purge"));
    _ui->serverScrollButton->setToolTip("Start/stop scrolling");
    _ui->serverScrollButton->setEnabled(_serverScrolling);
    connect(_ui->serverScrollButton, &QPushButton::toggled, this, [this](const bool value) {
        _serverScrolling = value;
    });

    // Clear button
    _ui->serverLogClearButton->setText(nullptr);
    _ui->serverLogClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->serverLogClearButton->setToolTip("Clear the log message list");
    connect(_ui->serverLogClearButton, &QPushButton::clicked, [this]() {
        _serverLogDataModel->removeRows(0, _serverLogDataModel->rowCount());
    });

    // Reconnect button
    _ui->serverReconnectButton->setText(nullptr);
    _ui->serverReconnectButton->setIcon(IconUtils::GetIcon("reconnect"));
    _ui->serverReconnectButton->setToolTip("Reconnect to the server websocket");
    connect(_ui->serverReconnectButton, &QPushButton::clicked, this, [this]() {
        if (_webSocket->state() == QAbstractSocket::ConnectedState) {
            _webSocket->disconnected();
            _webSocket->close();
        }
        _webSocket->open(QUrl(_websocketUrl));
    });

    // Start/Stop button
    _ui->logStopButton->setText(nullptr);
    _ui->logStopButton->setIcon(IconUtils::GetIcon("stop"));
    _ui->logStopButton->setToolTip("Stop the server websocket connection");
    connect(_ui->logStopButton, &QPushButton::clicked, this, [this]() {
        if (_webSocket->state() == QAbstractSocket::ConnectedState) {
            _webSocket->disconnected();
            _webSocket->close();
            _ui->logStopButton->setToolTip("Start the server websocket connection");
            _ui->logStopButton->setIcon(IconUtils::GetIcon("start"));
            logInfo << "Web socket connection started";
        } else if (_webSocket->state() == QAbstractSocket::UnconnectedState) {
            _webSocket->open(QUrl(_websocketUrl));
            _ui->logStopButton->setToolTip("Stop the server websocket connection");
            _ui->logStopButton->setIcon(IconUtils::GetIcon("stop"));
            logInfo << "Web socket connection stopped";
        }
    });

    // Extern window
    _ui->externWindowButton->setText(nullptr);
    _ui->externWindowButton->setIcon(IconUtils::GetIcon("extern-window"));
    _ui->externWindowButton->setToolTip("Detach the log window");
    connect(_ui->externWindowButton, &QPushButton::clicked, [this]() {
        _webSocket->disconnected();
        _webSocket->close();
        if (!_externalLogDialog) {
            _externalLogDialog = new ServerLogWidget();
            _externalLogDialog->setWindowFlags(Qt::Window);
            _externalLogDialog->setModal(false);
            _externalLogDialog->show();

            connect(_externalLogDialog, &ServerLogWidget::WebsocketClosed, this, [this]() {
                _webSocket->open(QUrl(_websocketUrl));
            });
        } else {
            // If it already exists, bring it to the front
            _externalLogDialog->show();
            _externalLogDialog->raise();
            _externalLogDialog->activateWindow();
        }
    });

    // Connect to configuration changes
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, [this](const QString &key, const QString &value) {
        if (key == "server.websocket-url") {
            _websocketUrl = value;
            if (_webSocket->state() == QAbstractSocket::ConnectedState) {
                _webSocket->disconnected();
                _webSocket->close();
            }
            _reconnectTimer->start();
        }
    });
}

void MainWidget::SetupLocalLogs() {
    // Data model
    _localLogDataModel = new QStandardItemModel(_ui->serverLogsTab);
    _ui->localLogList->setModel(_localLogDataModel);

    // Scroll button
    _ui->localScrollButton->setText(nullptr);
    _ui->localScrollButton->setIcon(IconUtils::GetIcon("scroll"));
    _ui->localScrollButton->setToolTip("Start/stop local scrolling");
    connect(_ui->localScrollButton, &QPushButton::toggled, this, [this](const bool value) {
        _localScrolling = value;
    });

    // Clear button
    _ui->localClearButton->setText(nullptr);
    _ui->localClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->localClearButton->setToolTip("Clear the local log message list");
    connect(_ui->localClearButton, &QPushButton::clicked, [this]() {
        _localLogDataModel->removeRows(0, _localLogDataModel->rowCount());
    });

    // Log level combo
    _ui->localLogLevelCombo->addItems(GetLogLevelNames());
    _ui->localLogLevelCombo->setCurrentText(LogLevelToString(_localLogLevel));
    connect(_ui->localLogLevelCombo, &QComboBox::currentTextChanged, this, [](const QString &text) {
        LogSignaler::instance().SetLevel(LogLevelToValue(LogLevelFromString(text)));
    });

    // Connect log events
    connect(&LogSignaler::instance(), &LogSignaler::newLog, this, [this](const QString &message) {
        _localLogDataModel->appendRow(new QStandardItem(message));
        // Auto-scroll to bottom
        if (_localScrolling) {
            _ui->localLogList->scrollToBottom();
        }
    });
}

void MainWidget::OnConnected() const {
    _reconnectTimer->stop();

    const auto item = new QStandardItem("Connect to server websocket, URL: " + _websocketUrl);
    _serverLogDataModel->appendRow(item);

    // Auto-scroll to bottom
    _ui->serverLogList->scrollToBottom();
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

    _serverLogDataModel->appendRow(item);

    if (_serverLogDataModel->rowCount() > _logLimit) {
        _serverLogDataModel->removeRows(0, 1);
    }

    // Auto-scroll to bottom
    if (_serverScrolling) {
        _ui->serverLogList->scrollToBottom();
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
