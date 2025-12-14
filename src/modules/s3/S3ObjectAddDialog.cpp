//
// Created by vogje01 on 12/13/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_S3ObjectAddDIalog.h" resolved

#include <QFileDialog>
#include <QMessageBox>
#include <modules/s3/S3ObjectAddDialog.h>
#include "ui_S3ObjectAddDialog.h"
#include "utils/Configuration.h"
#include "utils/IconUtils.h"


S3ObjectAddDialog::S3ObjectAddDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::S3ObjectAddDialog) {

    // Setup UI
    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &S3ObjectAddDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &S3ObjectAddDialog::HandleReject);

    // Connect browse button
    _ui->fileBrowseButton->setToolTip("Search for the file");
    _ui->fileBrowseButton->setIcon(IconUtils::GetIcon("search"));
    connect(_ui->fileBrowseButton, &QPushButton::clicked, this, &S3ObjectAddDialog::BrowseSourceFile);

    // Metatdata
    _ui->tabWidget->setCurrentIndex(0);
    _ui->tabWidget->removeTab(1);
}

S3ObjectAddDialog::~S3ObjectAddDialog() {
    delete _ui;
}


void S3ObjectAddDialog::BrowseSourceFile() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>(
        "ui.default-directory", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open source file", defaultDir, filter); !
        filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }
        const auto sourceFileInfo = QFileInfo(file.fileName());
        _ui->fileEdit->setText(file.fileName());
        _ui->s3KeyEdit->setText(sourceFileInfo.baseName() + "." + sourceFileInfo.suffix());
        Configuration::instance().SetValue<QString>("ui.default-directory", sourceFileInfo.absolutePath());
    }
}

void S3ObjectAddDialog::HandleAccept() {
    _s3Key = _ui->s3KeyEdit->text();
    _fileName = _ui->fileEdit->text();
    accept();
}

void S3ObjectAddDialog::HandleReject() {
    accept();
}
