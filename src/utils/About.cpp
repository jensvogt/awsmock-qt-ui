//
// Created by vogje01 on 11/16/25.
//

#include <utils/About.h>
#include "ui_About.h"

About::About(QWidget *parent) : QDialog(parent), _ui(new Ui::About) {
    _infraStructureService = new ModuleService();
    connect(_infraStructureService, &ModuleService::GetServerConfigSignal, this, &About::UpdateServerConfig);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        this->accept();
    });

    _ui->versionText->setText(APP_VERSION);
    _ui->qtVersionText->setText(QT_VERSION_STR);

    _ui->tabWidget->setCurrentIndex(0);

    _infraStructureService->GetServerConfig();
}

About::~About() {
    delete _ui;
}

void About::UpdateServerConfig(const GatewayConfig &gatewayConfig) const {
    _ui->serverVersionText->setText(gatewayConfig.version);
    _ui->serverRegionText->setText(gatewayConfig.region);
    _ui->serverPortText->setText(QString::number(gatewayConfig.port));
    _ui->serverPidText->setText(QString::number(gatewayConfig.pid));
    _ui->serverEndpointText->setText(gatewayConfig.endpoint);
    _ui->serverUserText->setText(gatewayConfig.user);
    _ui->serverAccountIdText->setText(gatewayConfig.accessId);
}
