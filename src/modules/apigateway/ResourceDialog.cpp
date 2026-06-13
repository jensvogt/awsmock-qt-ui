//
// Created by vogje01 on 6/13/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ResourceDialog.h" resolved

#include <modules/apigateway/ResourceDialog.h>
#include "ui_ResourceDialog.h"

ResourceDialog::ResourceDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::ResourceDialog) {

    // Initialize
    Initialize();
}

ResourceDialog::ResourceDialog(const RestApiCounter &restApi, const QString &resourceId, QWidget *parent) : QDialog(parent), _ui(new Ui::ResourceDialog) {

    // Initialize
    Initialize();

    const RestApiResource resource = restApi.resources[resourceId];
    _ui->nameEdit->setText(resource.id);
    _ui->pathEdit->setText(resource.path);
    _ui->parentIdEdit->setText(resource.parentId);
}

ResourceDialog::~ResourceDialog() {
    delete _apiGatewayService;
    delete _ui;
}

void ResourceDialog::Initialize() {

    // Create service
    _apiGatewayService = new ApiGatewayService();

    // Set up UI components
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ResourceDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ResourceDialog::HandleReject);
}

void ResourceDialog::HandleAccept() {
    accept();
}

void ResourceDialog::HandleReject() {
    reject();
}
