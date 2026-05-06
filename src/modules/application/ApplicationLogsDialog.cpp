//
// Created by vogje01 on 11/23/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationLogsDialog.h" resolved

#include <modules/application/ApplicationLogsDialog.h>
#include "ui_ApplicationLogsDialog.h"

ApplicationLogsDialog::ApplicationLogsDialog(const QString &applicationName, const QString &containerId, QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationLogsDialog), _containerId(containerId) {
    const long limit = Configuration::instance().GetValue<long>("ui.application-log-limit", 1000);
#ifdef _WIN32
    _dockerLogClient = new DockerLogClient(containerId, DockerLogClient::Mode::Tcp, "localhost:2375", limit, this);
#else
    _dockerLogClient = new DockerLogClient(containerId, DockerLogClient::Mode::UnixSocket, "/var/run/docker.sock", limit, this);
#endif
    connect(_dockerLogClient, &DockerLogClient::Connected, [this]() {
        _ui->statusLabel->setText("Status: Connected");
    });

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationLogsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationLogsDialog::HandleReject);

    // Set title4
    setWindowTitle(QString("Application Logs: ") + applicationName);

    // Status label
    _ui->statusLabel->setText("Status: Disconnected");

    // Setup log list view
    _model = new QStandardItemModel(this);
    _ui->logListView->setModel(_model);
    _ui->logListView->setUniformItemSizes(false);
    _ui->logListView->setWordWrap(false);
    _ui->logListView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _ui->logListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(_dockerLogClient, &DockerLogClient::LogReceived, [&](const QStringList &lines) {
        for (const QString &line: lines) {
            const auto item = new QStandardItem(line);

            // Color based on severity
            if (line.contains("ERROR", Qt::CaseSensitive)) {
                item->setForeground(Qt::red);
            } else if (line.contains("WARNING", Qt::CaseSensitive)) {
                item->setForeground(Qt::darkYellow);
            } else if (line.contains("INFO", Qt::CaseSensitive)) {
                item->setForeground(Qt::lightGray);
            } else if (line.contains("DEBUG", Qt::CaseSensitive) || line.contains("TRACE", Qt::CaseSensitive)) {
                item->setForeground(Qt::darkGreen);
            }
            _model->appendRow(item);

            // Remove oldest lines if exceeding limit
            while (_model->rowCount() > 1000) {
                _model->removeRow(0);
            }
        }

        // Update log level
        UpdateLogLevel();

        // Scroll to bottom
        if (_autoScroll) {
            _ui->logListView->scrollToBottom();
        }
    });

    // Level combo
    const QStringList levels = {"TRACE", "DEBUG", "INFO", "WARNING", "ERROR"};
    _ui->levelCombo->addItems(levels);
    _ui->levelCombo->setCurrentText(_currentLogLevel);
    connect(_ui->levelCombo, &QComboBox::currentTextChanged, this, [this]() {
        _currentLogLevel = _ui->levelCombo->currentText();
        UpdateLogLevel();
    });

    // Refresh button
    _ui->clearButton->setText(nullptr);
    _ui->clearButton->setIcon(IconUtils::GetIcon("clear"));
    connect(_ui->clearButton, &QPushButton::clicked, this, [this]() {
        _model->clear();
    });

    // Scroll button
    _ui->autoScrollButton->setText(nullptr);
    _ui->autoScrollButton->setIcon(IconUtils::GetIcon("purge"));
    _ui->autoScrollButton->setToolTip("Start/stop scrolling");
    _ui->autoScrollButton->setChecked(_autoScroll);
    connect(_ui->autoScrollButton, &QPushButton::toggled, this, [this](const bool value) {
        _autoScroll = value;
    });

    // Set container ID
    _ui->containerIdEdit->setText(!containerId.isEmpty() ? containerId.first(12) : "-");

    // Start log capture
    _dockerLogClient->ConnectToDocker();
}

ApplicationLogsDialog::~ApplicationLogsDialog() {
    delete _ui;
}

void ApplicationLogsDialog::HandleAccept() {
    accept();
}

void ApplicationLogsDialog::HandleReject() {
    accept();
}

bool ApplicationLogsDialog::CheckLogLevel(const QString &line) const {
    if (line.contains("ERROR", Qt::CaseSensitive)) {
        return true;
    }
    if (line.contains("WARNING", Qt::CaseSensitive) && (
            _currentLogLevel == "TRACE" || _currentLogLevel == "DEBUG" || _currentLogLevel == "INFO" ||
            _currentLogLevel == "WARNING")) {
        return true;
    }
    if (line.contains("INFO", Qt::CaseSensitive) && (
            _currentLogLevel == "TRACE" || _currentLogLevel == "DEBUG" || _currentLogLevel == "INFO")) {
        return true;
    }
    if (line.contains("DEBUG", Qt::CaseSensitive) && (
            _currentLogLevel == "TRACE" || _currentLogLevel == "DEBUG")) {
        return true;
    }
    if (line.contains("TRACE", Qt::CaseSensitive) && (
            _currentLogLevel == "TRACE")) {
        return true;
    }
    return false;
}

void ApplicationLogsDialog::UpdateLogLevel() const {
    for (int row = 0; row < _model->rowCount(); ++row) {
        const QStandardItem *item = _model->item(row);
        _ui->logListView->setRowHidden(row, !CheckLogLevel(item->text()));
    }

    // Scroll to bottom
    if (_autoScroll) {
        _ui->logListView->scrollToBottom();
    }
}
