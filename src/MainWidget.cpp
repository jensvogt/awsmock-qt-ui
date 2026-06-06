//
// Created by vogje01 on 2/6/26.
//

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

    // Set splitter position
    _ui->horizontalSplitter->setStretchFactor(0, 0); // left — don't stretch
    _ui->horizontalSplitter->setStretchFactor(1, 4); // right — take all space

    // Status label
    _ui->statusLabel->setText("Initialized");
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
    _navList = new QListView(this);
    _navList->setGridSize(QSize(0, 20));

    // Data model
    _navDataModel = new QStandardItemModel(_navList);
    _navList->setModel(_navDataModel);
    _navList->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    _navDataModel->appendRow(new QStandardItem("API Gateway"));

    // Set current index
    _navList->setCurrentIndex(_navList->model()->index(0, 0));
    connect(_navList, &QListView::clicked, [this](const QModelIndex &index) {
        const QString name = _navDataModel->item(index.row())->text();
        MainRouter::instance().SetRoute(name);
    });
    _ui->horizontalSplitter->addWidget(_navList);
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
    _moduleService->SetLogLevel(_currentLogLevel, "All");

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

    // Set up logging timer
    _logTimer = new QTimer(this);
    _logTimer->setInterval(LOG_BATCH_MS);
    connect(_logTimer, &QTimer::timeout, this, &MainWidget::FlushLogQueue);
    _logTimer->start();

    // Logging channel combo
    const auto logChannelList = QStringList({
        "All", "ApiGateway", "Application", "Cognito", "Core", "DynamoDB", "KMS", "Lambda", "Manager", "Module", "Monitoring", "S3", "SNS", "SQS", "SSM",
        "SecretsManager", "Transfer"
    });
    _ui->logChannelCombo->addItems(logChannelList);
    _ui->logChannelCombo->setCurrentText(_currentLogChannel);
    logInfo << "Server log channel set to " << _currentLogChannel;

    connect(_ui->logChannelCombo, &QComboBox::currentTextChanged, this, [this](const QString &logChannel) {
        _moduleService->SetLogLevel(_currentLogLevel, logChannel);
        _currentLogChannel = logChannel;
        logInfo << "Server log level changed to " << _currentLogLevel;
    });

    // Logging level combo
    const auto logLevelList = QStringList({"trace", "debug", "info", "warning", "error", "fatal"});
    _ui->logLevelCombo->addItems(logLevelList);
    _ui->logLevelCombo->setCurrentText(_currentLogLevel);
    logInfo << "Server log level set to " << _currentLogLevel;

    connect(_ui->logLevelCombo, &QComboBox::currentTextChanged, this, [this](const QString &logLevel) {
        _moduleService->SetLogLevel(logLevel, _currentLogChannel);
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
    _serverLogDataModel = new QStandardItemModel();
    _serverProxyModel = new QSortFilterProxyModel(this);
    _serverProxyModel->setSourceModel(_serverLogDataModel);
    _serverProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _ui->serverLogList->setModel(_serverProxyModel);
    _ui->serverListFilterWidget->SetFilterProxyModel(_serverProxyModel);

    // Scroll button
    _ui->serverScrollButton->setText(nullptr);
    _ui->serverScrollButton->setIcon(IconUtils::GetIcon("purge"));
    _ui->serverScrollButton->setToolTip("Start/stop scrolling");
    _ui->serverScrollButton->setChecked(_serverScrolling);
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

void MainWidget::FlushLogQueue() {
    if (_logQueue.isEmpty()) return;

    const auto batch = _logQueue.mid(0, LOG_BATCH_MAX);
    _logQueue.erase(_logQueue.begin(), _logQueue.begin() + batch.size());

    // Only block view repaints, NOT model signals
    _ui->serverLogList->setUpdatesEnabled(false);

    for (const auto &message: batch) {
        const auto item = new QStandardItem(message);
        if (message.contains("[error]")) {
            item->setForeground(Qt::red);
        } else if (message.contains("[warning]")) {
            item->setForeground(Qt::darkYellow);
        } else if (message.contains("[debug]")) {
            item->setForeground(Qt::green);
        }
        _serverLogDataModel->appendRow(item);
    }

    if (const int excess = _serverLogDataModel->rowCount() - _logLimit; excess > 0)
        _serverLogDataModel->removeRows(0, excess);

    _ui->serverLogList->setUpdatesEnabled(true);

    if (_serverScrolling)
        _ui->serverLogList->scrollToBottom();
}

void MainWidget::SetupLocalLogs() {

    // Data model
    _localLogDataModel = new QStandardItemModel(_ui->serverLogsTab);
    _localProxyModel = new QSortFilterProxyModel(this);
    _localProxyModel->setSourceModel(_localLogDataModel);
    _localProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _ui->localLogList->setModel(_localProxyModel);
    _ui->localListFilterWidget->SetFilterProxyModel(_localProxyModel);

    // Scroll button
    _ui->localScrollButton->setText(nullptr);
    _ui->localScrollButton->setIcon(IconUtils::GetIcon("scroll"));
    _ui->localScrollButton->setToolTip("Start/stop local scrolling");
    _ui->localScrollButton->setChecked(_localScrolling);
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

void MainWidget::OnMessageReceived(const QString &message) {
    _logQueue.append(message);
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
