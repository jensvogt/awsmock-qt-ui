//
// Created by vogje01 on 11/9/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SQSMessageAddDialog.h" resolved

#include <modules/sqs/SQSMessageAddDialog.h>
#include "ui_SQSMessageAddDialog.h"


SQSMessageAddDialog::SQSMessageAddDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::SQSMessageAddDialog) {

    // Connect service events
    _sqsService = new SQSService();
    //connect(_sqsService, &SQSService::ListMessagesSignal, this, &SQSMessageList::HandleListMessageSignal);
    //connect(_sqsService, &SQSService::ReloadMessagesSignal, this, &SQSMessageList::HandleReloadMessageSignal);

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &SQSMessageAddDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &SQSMessageAddDialog::HandleReject);

    // Set default tab
    _ui->tabWidget->setCurrentIndex(0);
}

SQSMessageAddDialog::~SQSMessageAddDialog() {
    delete _ui;
}

void SQSMessageAddDialog::HandleAccept() {
    /*if (this->_changed) {
        _sqsService->UpdateApplication(_application);
    }*/
    accept();
}


void SQSMessageAddDialog::HandleReject() {
    accept();
}
