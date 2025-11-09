//
// Created by vogje01 on 11/9/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ApplicationDependencyDIalog.h" resolved

#include <modules/application/ApplicationDependencyDialog.h>
#include "ui_ApplicationDependencyDialog.h"


ApplicationDependencyDialog::ApplicationDependencyDialog(const QString &name, const bool add, QWidget *parent) : QDialog(parent), _ui(new Ui::ApplicationDependencyDialog) {

    _applicationService = new ApplicationService();

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ApplicationDependencyDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ApplicationDependencyDialog::HandleReject);

    // Fill in application combo box
    _applicationService->ListApplicationNames();
    connect(_applicationService, &ApplicationService::ListApplicationNamedSignal, this, [this](const QStringList &applicationNames) {
        _ui->comboBox->clear();
        _ui->comboBox->addItems(applicationNames);
    });

    // Connect application selection changed
    connect(_ui->comboBox, &QComboBox::currentTextChanged, this, [this](const QString &name) {
        _name = name;
    });

    if (!add) {
        _ui->comboBox->setCurrentText(name);
    }
}

ApplicationDependencyDialog::~ApplicationDependencyDialog() {
    delete _ui;
}

void ApplicationDependencyDialog::HandleAccept() {
    accept();
}

void ApplicationDependencyDialog::HandleReject() {
    accept();
}

QString ApplicationDependencyDialog::GetName() {
    return _name;
}
