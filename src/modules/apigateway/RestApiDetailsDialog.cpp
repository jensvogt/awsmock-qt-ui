//
// Created by vogje01 on 6/6/26.
//

#include <modules/apigateway/RestApiDetailsDialog.h>
#include "ui_RestApiDetailsDialog.h"

RestApiDetailsDialog::RestApiDetailsDialog(QWidget *parent) : ::BaseDialog(parent), _ui(new Ui::RestApiDetailsDialog) {

    // Initialize
    Initialize();
}

RestApiDetailsDialog::RestApiDetailsDialog(QString name, QWidget *parent) : BaseDialog(parent), _ui(new Ui::RestApiDetailsDialog), _name(std::move(name)) {

    // Initialize
    Initialize();

    // Name
    _ui->nameEdit->setText(_name);

    // Update
    _apiGatewayService->GetRestApi(_name);
}

void RestApiDetailsDialog::Initialize() {

    // Set up service
    _apiGatewayService = new ApiGatewayService();
    connect(_apiGatewayService, &ApiGatewayService::GetRestApiSignal, this, &RestApiDetailsDialog::HandleGet);

    // Set up UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &RestApiDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RestApiDetailsDialog::HandleReject);

    // Refresh
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));
    connect(_ui->refreshButton, &QPushButton::clicked, this, [this]() {
        _apiGatewayService->GetRestApi(_name);
    });

    // Connect the status label
    connect(&EventBus::instance(), &EventBus::TimerSignal, this, [this](const QString &timerName, const qint64 elapsed) {
        if (timerName == "GetRestApi" || timerName == "UpdateRestApi") {
            _ui->statusLabel->setText("Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" + QString::number(elapsed) + "ms]");
        }
    });

    // Updates
    connect(_ui->enabledCheck, &QCheckBox::checkStateChanged, this, [this](int) { _changed = true; });
    connect(_ui->descriptionEdit, &QTextEdit::textChanged, this, [this]() { _changed = true; });

    // Resource table
    const QStringList headers = QStringList() = {tr("ID"), tr("Name"), tr("ParentId"), tr("Created"), tr("Modified")};
    _ui->resourceTable->setColumnCount(static_cast<int>(headers.count()));
    _ui->resourceTable->setShowGrid(true);
    _ui->resourceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->resourceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->resourceTable->setHorizontalHeaderLabels(headers);
    _ui->resourceTable->setSortingEnabled(true);
    _ui->resourceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->resourceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ui->resourceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _ui->resourceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->resourceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->resourceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    // Add context menu
    _ui->resourceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->resourceTable, &QTableWidget::customContextMenuRequested, this, &RestApiDetailsDialog::ShowResourcesContextMenu);

    // Add resource button
    _ui->addResourceButton->setText(nullptr);
    _ui->addResourceButton->setIcon(IconUtils::GetIcon("add"));
    connect(_ui->addResourceButton, &QPushButton::clicked, [this]() {
        if (ResourceDialog dialog(this); dialog.exec() == Accepted) {
            _apiGatewayService->GetRestApi(_name);
        }
    });

    // API key source
    QStringList sources = {"HEADER", "AUTHORIZER", "UNKNOWN"};
    _ui->apiKeySourceCombo->addItems(sources);
    _ui->apiKeySourceCombo->setCurrentIndex(0);
}

RestApiDetailsDialog::~RestApiDetailsDialog() {
    delete _ui;
}

void RestApiDetailsDialog::HandleAccept() {
    if (_changed && !_name.isEmpty()) {
        UpdateResource();
    } else if (_changed) {
        CreateResource();
    }
    accept();
}

void RestApiDetailsDialog::HandleReject() {
    reject();
}

void RestApiDetailsDialog::HandleGet(const RestApiGetResponse &restApiGetResponse) {

    _restApi = restApiGetResponse.restApiCounter;

    // Description
    _ui->idEdit->setText(restApiGetResponse.restApiCounter.id);
    _ui->rootResourceIdEdit->setText(restApiGetResponse.restApiCounter.rootResourceId);
    _ui->descriptionEdit->setText(restApiGetResponse.restApiCounter.description);

    // API key source
    _ui->apiKeySourceCombo->setCurrentText(restApiGetResponse.restApiCounter.apiKeySource);

    // Dates
    _ui->createdEdit->setText(DateTimeUtils::GetDateTimeFormat(restApiGetResponse.restApiCounter.created));
    _ui->modifiedEdit->setText(DateTimeUtils::GetDateTimeFormat(restApiGetResponse.restApiCounter.modified));

    const int selectedRow = _ui->resourceTable->selectionModel()->currentIndex().row();
    _ui->resourceTable->setRowCount(0);
    _ui->resourceTable->setSortingEnabled(false);
    auto r = 0;
    for (const auto &[key, resource]: restApiGetResponse.restApiCounter.resources.asKeyValueRange()) {
        auto c = 0;
        _ui->resourceTable->insertRow(r);
        SetColumn(_ui->resourceTable, r, c++, resource.id);
        SetColumn(_ui->resourceTable, r, c++, resource.pathPart);
        SetColumn(_ui->resourceTable, r, c++, resource.parentId);
        SetColumn(_ui->resourceTable, r, c++, resource.created);
        SetColumn(_ui->resourceTable, r, c++, resource.modified);
        r++;
    }
    _ui->resourceTable->setRowCount(static_cast<int>(restApiGetResponse.restApiCounter.resources.count()));
    _ui->resourceTable->setSortingEnabled(true);
    //_ui->resourceTable->sortItems(_instanceSortColumn, _instanceSortOrder);
    _ui->resourceTable->selectRow(selectedRow);
}

void RestApiDetailsDialog::ShowResourcesContextMenu(const QPoint &pos) {
    const QModelIndex index = _ui->resourceTable->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    // Context menu
    QMenu menu;
    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Resource");
    editAction->setToolTip("Edit the REST API resource");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction(IconUtils::GetIcon("delete"), "Delete Resource");
    deleteAction->setToolTip("Delete the REST API resources");

    // Get the metadata attributes
    const QString key = _ui->resourceTable->item(index.row(), 0)->text();
    const QString value = _ui->resourceTable->item(index.row(), 1)->text();

    // Context menu callbacks
    if (const QAction *selectedAction = menu.exec(_ui->resourceTable->viewport()->mapToGlobal(pos)); selectedAction == editAction) {
        if (ResourceDialog dialog; dialog.exec() == Accepted) {
            _apiGatewayService->GetRestApi(_name);
        }
        _changed = true;
    } else if (selectedAction == deleteAction) {
        _apiGatewayService->DeleteResource(_restApi.id, key);
        _ui->resourceTable->removeRow(index.row());
        _changed = true;
    }
}

void RestApiDetailsDialog::UpdateResource() const {
    RestApiUpdateRequest request;
    request.restApi.name = _name;
    request.restApi.apiKeySource = _ui->apiKeySourceCombo->currentText();
    request.restApi.enabled = _ui->enabledCheck->isChecked();
    request.restApi.description = _ui->descriptionEdit->toPlainText();
    request.restApi.resources = _restApi.resources;
    request.restApi.modified = QDateTime();
    _apiGatewayService->UpdateRestApi(request);
}

void RestApiDetailsDialog::CreateResource() const {
    RestApiCreateRequest request;
    request.name = _ui->nameEdit->text();
    request.enabled = _ui->enabledCheck->isChecked();
    request.description = _ui->descriptionEdit->toPlainText();
    _apiGatewayService->CreateRestApi(request);
}
