//
// Created by vogje01 on 2/22/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ReplaceReplaceWidget.h" resolved

#include <components/ReplaceWidget.h>
#include "ui_ReplaceWidget.h"

namespace Awsmock::Components {
    ReplaceWidget::ReplaceWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::ReplaceWidget) {

        // Initialize UI components
        _ui->setupUi(this);

        _ui->forwardButton->setText(nullptr);
        _ui->forwardButton->setIcon(IconUtils::GetIcon("down"));
        connect(_ui->forwardButton, &QPushButton::clicked, this, [this]() {
            emit SigForward(_searchText);
        });
        _ui->backButton->setText(nullptr);
        _ui->backButton->setIcon(IconUtils::GetIcon("up"));
        connect(_ui->backButton, &QPushButton::clicked, this, [this]() {
            emit SigBackward(_searchText);
        });

        _ui->replaceButton->setText(tr("Replace"));
        connect(_ui->replaceButton, &QPushButton::clicked, this, [this]() {
            emit SigReplace(_searchText, _replaceText);
        });
        _ui->replaceAllButton->setText(tr("Replace All"));
        connect(_ui->replaceAllButton, &QPushButton::clicked, this, [this]() {
            emit SigReplaceAll(_searchText, _replaceText);
        });

        _ui->closeButton->setText(nullptr);
        _ui->closeButton->setIcon(IconUtils::GetIcon("close"));
        connect(_ui->closeButton, &QPushButton::clicked, this, [this]() {
            _ui->statusLabel->setText(QString::number(0) + "/" + QString::number(0));
            emit SigClose();
        });

        // Search text edit
        connect(_ui->searchEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
            _searchText = text;
            emit SigSearch(_searchText);
        });

        // Replacement text edit
        connect(_ui->replaceEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
            _replaceText = text;
        });
        _ui->searchEdit->setFocus();
        _ui->statusLabel->setText(nullptr);
    }

    ReplaceWidget::~ReplaceWidget() {
        delete _ui;
    }

    void ReplaceWidget::SetCursor(const int current, const int total) const {
        _ui->statusLabel->setText(QString::number(current) + "/" + QString::number(total));
    }
} // Awsmock::Components
