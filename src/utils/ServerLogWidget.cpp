//
// Created by vogje01 on 2/13/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ServerLogWidget.h" resolved

#include <utils/ServerLogWidget.h>
#include "ui_ServerLogWidget.h"

ServerLogWidget::ServerLogWidget(QWidget *parent) : BaseDialog(parent), _ui(new Ui::ServerLogWidget) {

    // UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ServerLogWidget::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ServerLogWidget::HandleReject);

    // Create websocket
    _websocketUrl = Configuration::instance().GetValue<QString>("server.websocket-url");
    _webSocket = new QWebSocket();
    _ui->statusLabel->setText("Server: " + _websocketUrl);

    // Reconnect timer
    _reconnectTimer = new QTimer(this);
    _reconnectTimer->setInterval(5000);

    // Connect signals
    connect(_webSocket, &QWebSocket::connected, this, &ServerLogWidget::OnConnected);
    connect(_webSocket, &QWebSocket::textMessageReceived, this, &ServerLogWidget::OnMessageReceived);
    connect(_webSocket, &QWebSocket::disconnected, this, [this]() {
        _reconnectTimer->start();
    });
    connect(_webSocket, &QWebSocket::errorOccurred, [this](const QAbstractSocket::SocketError error) {
        // Even if it fails to connect, ensure the timer keeps running
        if (!_reconnectTimer->isActive()) {
            _reconnectTimer->start();
        }
    });
    connect(_reconnectTimer, &QTimer::timeout, this, [this]() {
        _webSocket->open(QUrl(_websocketUrl));
    });

    // Open the connection
    _webSocket->open(QUrl(_websocketUrl));

    // Data model
    _serverLogDataModel = new QStandardItemModel(_ui->logList);
    _ui->logList->setModel(_serverLogDataModel);

    // Scroll button
    _ui->scrollButton->setText(nullptr);
    _ui->scrollButton->setIcon(IconUtils::GetIcon("purge"));
    _ui->scrollButton->setToolTip("Start/stop scrolling");
    _ui->scrollButton->setChecked(_autoScroll);
    connect(_ui->scrollButton, &QPushButton::toggled, this, [this](const bool value) {
        _autoScroll = value;
    });

    // Clear button
    _ui->clearButton->setText(nullptr);
    _ui->clearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->clearButton->setToolTip("Clear the log message list");
    connect(_ui->clearButton, &QPushButton::clicked, [this]() {
        _serverLogDataModel->removeRows(0, _serverLogDataModel->rowCount());
    });

    // Reconnect button
    _ui->reconnectButton->setText(nullptr);
    _ui->reconnectButton->setIcon(IconUtils::GetIcon("reconnect"));
    _ui->reconnectButton->setToolTip("Reconnect to the server websocket");
    connect(_ui->reconnectButton, &QPushButton::clicked, this, [this]() {
        if (_webSocket->state() == QAbstractSocket::ConnectedState) {
            _webSocket->disconnected();
            _webSocket->close();
        }
        _reconnectTimer->start();
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
            _ui->statusLabel->setText("Server: " + _websocketUrl);
        }
    });
}

ServerLogWidget::~ServerLogWidget() {
    delete _ui;
}

void ServerLogWidget::HandleAccept() {
    accept();
}

void ServerLogWidget::HandleReject() {
    if (_webSocket) {
        _webSocket->close();
        emit WebsocketClosed();
    }

    accept();
}

void ServerLogWidget::OnConnected() const {
    _reconnectTimer->stop();

    const auto item = new QStandardItem("Connect to server websocket, URL: " + _websocketUrl);
    _serverLogDataModel->appendRow(item);

    // Auto-scroll to bottom
    _ui->logList->scrollToBottom();
}

void ServerLogWidget::OnMessageReceived(const QString &message) const {
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

    // Auto-scroll to bottom
    if (_autoScroll) {
        _ui->logList->scrollToBottom();
    }
}
