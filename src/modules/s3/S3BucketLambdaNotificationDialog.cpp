//
// Created by vogje01 on 2/21/26.
//

#include <modules/s3/S3BucketLambdaNotificationDialog.h>
#include "ui_S3BucketLambdaNotificationDialog.h"

S3BucketLambdaNotificationDialog::S3BucketLambdaNotificationDialog(QString bucket, QWidget *parent) : QDialog(parent), _ui(new Ui::S3BucketLambdaNotificationDialog), _bucket(std::move(bucket)) {

    // Initialize UI component
    Initialize();
}

S3BucketLambdaNotificationDialog::S3BucketLambdaNotificationDialog(QString bucket, S3LambdaConfiguration lambdaConfiguration, QWidget *parent) : QDialog(parent), _ui(new Ui::S3BucketLambdaNotificationDialog),
                                                                                                                                                 _bucket(std::move(bucket)), _lambdaConfiguration(std::move((lambdaConfiguration))) {
    // Initialize UI component
    Initialize();

    if (!_lambdaConfiguration.filterRules.isEmpty()) {
        _ui->filterRulesEdit->setText(_lambdaConfiguration.filterRules.first().filterValue);
        _ui->filterRuleTypeCombo->setCurrentText(S3FilterNameToString(_lambdaConfiguration.filterRules.first().name));
    }

    if (!_lambdaConfiguration.events.isEmpty()) {
        for (auto r = 0; r < _lambdaConfiguration.events.count(); r++) {

            // Append to right list
            const auto item = new QStandardItem(S3NotificationEventToString(_lambdaConfiguration.events.at(r)));
            _rightDataModel->appendRow(item);

            // Remove from left list
            if (QModelIndexList results = _leftDataModel->match(_leftDataModel->index(0, 0), Qt::DisplayRole, S3NotificationEventToString(_lambdaConfiguration.events.at(r)), 1, Qt::MatchExactly); !results.isEmpty()) {
                _leftDataModel->removeRow(results.at(0).row());
            }
        }
    }
}

S3BucketLambdaNotificationDialog::~S3BucketLambdaNotificationDialog() {
    delete _s3Service;
    delete _lambdaService;
    delete _ui;
}

void S3BucketLambdaNotificationDialog::Initialize() {

    // REST API services
    _s3Service = new S3Service();
    _lambdaService = new LambdaService();

    // Setup UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3BucketLambdaNotificationDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3BucketLambdaNotificationDialog::HandleReject);

    // Bucket field
    _ui->bucketEdit->setText(_bucket);

    // Connect combo box
    _lambdaService->ListLambdaArns();
    connect(_lambdaService, &LambdaService::ListLambdaArnsSignal, this, [this](const LambdaListArnsResponse &response) {
        _ui->lambdaCombo->addItems(response.lambdaArns);
    });

    // Filter rule
    _ui->filterRuleTypeCombo->addItems(S3FilterRuleNameList());
    _ui->filterRuleTypeCombo->setCurrentIndex(0);

    // Event types
    _ui->leftToRightButton->setText(nullptr);
    _ui->leftToRightButton->setIcon(IconUtils::GetIcon("left-2-right"));
    _ui->rightToLeftButton->setText(nullptr);
    _ui->rightToLeftButton->setIcon(IconUtils::GetIcon("right-2-left"));

    // Available list
    _leftDataModel = new QStandardItemModel(_ui->availableEventTypeList);
    _ui->availableEventTypeList->setModel(_leftDataModel);
    _ui->availableEventTypeList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Fill in left list
    _leftDataModel->removeRows(0, _leftDataModel->rowCount());
    for (auto r = 0; r < S3EventTypeNameList().count(); r++) {
        _leftDataModel->appendRow(new QStandardItem(S3EventTypeNameList().at(r)));
    }
    _leftDataModel->sort(0, Qt::AscendingOrder);

    // Selected list
    _rightDataModel = new QStandardItemModel(_ui->selectedEventTypeList);
    _rightDataModel->sort(0, Qt::AscendingOrder);
    _ui->selectedEventTypeList->setModel(_rightDataModel);
    _ui->selectedEventTypeList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    SetupListButtons();
}

void S3BucketLambdaNotificationDialog::SetupListButtons() {
    connect(_ui->leftToRightButton, &QPushButton::clicked, [this] {

        // Get the selection model
        const QItemSelectionModel *select = _ui->availableEventTypeList->selectionModel();

        // Sort indices in descending order by row
        QModelIndexList selectedItems = select->selectedIndexes();
        std::ranges::sort(selectedItems, [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        // Iterate and access data
        foreach(const QModelIndex &index, selectedItems) {
            if (QString text = index.data(Qt::DisplayRole).toString(); _rightDataModel->findItems(text).isEmpty()) {
                if (!text.isEmpty()) {
                    _rightDataModel->appendRow(new QStandardItem(text));
                    _leftDataModel->removeRow(index.row());
                }
            }
        }
        _ui->availableEventTypeList->clearSelection();
        _leftDataModel->sort(0, Qt::AscendingOrder);
        _rightDataModel->sort(0, Qt::AscendingOrder);
    });
    connect(_ui->rightToLeftButton, &QPushButton::clicked, [this] {

        // Get the selection model
        const QItemSelectionModel *select = _ui->selectedEventTypeList->selectionModel();

        // Sort indices in descending order by row
        QModelIndexList selectedItems = select->selectedIndexes();
        std::ranges::sort(selectedItems, [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        // Iterate and access data
        foreach(const QModelIndex &index, selectedItems) {
            if (QString text = index.data(Qt::DisplayRole).toString(); _leftDataModel->findItems(text).isEmpty()) {
                if (!text.isEmpty()) {
                    _leftDataModel->appendRow(new QStandardItem(text));
                    _rightDataModel->removeRow(index.row());
                }
            }
        }
        _ui->availableEventTypeList->clearSelection();
        _leftDataModel->sort(0, Qt::AscendingOrder);
        _rightDataModel->sort(0, Qt::AscendingOrder);
    });
}

void S3BucketLambdaNotificationDialog::HandleAccept() {
    _lambdaConfiguration.id = StringUtils::GenerateRandomNumericString(13);
    _lambdaConfiguration.lambdaArn = _ui->lambdaCombo->currentText();

    // Get all events types
    for (int i = 0; i < _rightDataModel->rowCount(); ++i) {

        // Get the index for the current row
        QModelIndex index = _rightDataModel->index(i, 0);

        // Pull the data (text)
        if (QString text = _rightDataModel->data(index, Qt::DisplayRole).toString(); !_lambdaConfiguration.events.contains(S3NotificationEventFromString(text))) {
            _lambdaConfiguration.events.append(S3NotificationEventFromString(text));
        }
    }

    // Get filter rule
    S3FilterRule filterRule;
    filterRule.name = S3FilterNameFromString(_ui->filterRuleTypeCombo->currentText());
    filterRule.filterValue = _ui->filterRulesEdit->text();
    _lambdaConfiguration.filterRules.append(filterRule);

    accept();
}


void S3BucketLambdaNotificationDialog::HandleReject() {
    reject();
}
