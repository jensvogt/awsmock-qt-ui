//
// Created by vogje01 on 12/19/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SSMParameterDialog.h" resolved

#include <QInputDialog>
#include <modules/ssm/SSMParameterDialog.h>
#include "ui_SSMParameterDialog.h"

SSMParameterDialog::SSMParameterDialog(const QString &parameterName, QWidget *parent) : BaseDialog(parent), _ui(new Ui::SSMParameterDialog), _parameterName(parameterName) {

    // Connect service
    _ssmService = new SSMService();
    connect(_ssmService, &SSMService::GetParameterSignal, this, &SSMParameterDialog::HandleParameterGetSignal);

    // Initialize UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SSMParameterDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SSMParameterDialog::HandleReject);

    // Setup tabs
    SetupTagsTab();

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);

    // Load content
    SSMParameterDialog::LoadContent();
}

SSMParameterDialog::~SSMParameterDialog() {
    delete _ui;
}

void SSMParameterDialog::HandleAccept() {
    if (_changed) {
        _ssmService->UpdateParameter(_parameter);
    }
    accept();
}

void SSMParameterDialog::HandleReject() {
    accept();
}

void SSMParameterDialog::LoadContent() {
    _ssmService->GetParameter(_parameterName);
}

void SSMParameterDialog::HandleParameterGetSignal(const SSMParameterGetResponse &parameterGetResponse) {

    // Save parameter
    _parameter = parameterGetResponse.parameter;

    // Fill in UI
    _ui->regionEdit->setText(parameterGetResponse.parameter.region);
    _ui->nameEdit->setText(parameterGetResponse.parameter.name);
    _ui->arnEdit->setText(parameterGetResponse.parameter.arn);
    _ui->kmsKeyIdEdit->setText(parameterGetResponse.parameter.kmsKeyArn);
    _ui->createdEdit->setText(parameterGetResponse.parameter.created.toString("yyyy-MM-dd hh:mm:ss"));
    _ui->modifiedEdit->setText(parameterGetResponse.parameter.modified.toString("yyyy-MM-dd hh:mm:ss"));

    // Value field
    _ui->secretStringEdit->setEchoMode(QLineEdit::Password);
    _ui->secretStringEdit->setText(QString(QByteArray::fromBase64(parameterGetResponse.parameter.parameterValue.toUtf8())));
    _ui->showValueButton->setText(nullptr);
    _ui->showValueButton->setIcon(IconUtils::GetIcon("show"));
    connect(_ui->showValueButton, &QPushButton::toggled, [this](const bool checked) {
        if (checked) {
            _ui->secretStringEdit->setEchoMode(QLineEdit::Normal);
        } else {
            _ui->secretStringEdit->setEchoMode(QLineEdit::Password);
        }
    });
    _ui->editValueButton->setText(nullptr);
    _ui->editValueButton->setIcon(IconUtils::GetIcon("edit"));
    connect(_ui->editValueButton, &QPushButton::clicked, [this]() {
        _changed = true;
        bool ok;
        auto parameterValue = QString(QByteArray::fromBase64(_parameter.parameterValue.toUtf8()));
        if (parameterValue = QInputDialog::getText(nullptr, "Change secret value", "Secret String:", QLineEdit::Normal, parameterValue, &ok); ok && !parameterValue.isEmpty()) {
            _parameter.parameterValue = parameterValue.toUtf8().toBase64();
            _ui->secretStringEdit->setText(parameterValue);
        }
    });

    // Description
    _ui->descriptionEdit->setText(parameterGetResponse.parameter.description);

    const int selectedRow = _ui->tagsTableView->selectionModel()->currentIndex().row();
    _ui->tagsTableView->setSortingEnabled(false);
    int r = 0, c = 0;
    for (const auto key: parameterGetResponse.parameter.tags.keys()) {
        SetColumn(_tagsDataModel, r, c++, key);
        SetColumn(_tagsDataModel, r, c, parameterGetResponse.parameter.tags[key]);
        r++;
        c = 0;
    }

    // Reset selection
    _ui->tagsTableView->setSortingEnabled(true);
    _ui->tagsTableView->selectRow(selectedRow);
}

void SSMParameterDialog::SetupTagsTab() {

    // Tags refresh button
    _ui->tagsRefreshButton->setText(nullptr);
    _ui->tagsRefreshButton->setIcon(IconUtils::GetIcon("refresh"));

    // Table
    const QStringList headers = QStringList() = {
                                    tr("Name"), tr("Value")
                                };

    // Table
    _tagsDataModel = new QStandardItemModel();
    _tagsDataModel->setHorizontalHeaderLabels(headers);
    _tagsDataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _tagsProxyModel = new PrefixFilterProxyModel(this);
    _tagsProxyModel->setSourceModel(_tagsDataModel);
    _ui->tagsTableView->setModel(_tagsProxyModel);

    _ui->tagsTableView->setShowGrid(true);
    _ui->tagsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->tagsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->tagsTableView->setSortingEnabled(true);
    _ui->tagsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->tagsTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->tagsTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->tagsTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->tagsTableView->setColumnHidden(3, true);
}
