//
// Created by vogje01 on 2/15/26.
//

#include <components/PageableList.h>
#include "ui_PageableList.h"

PageableList::PageableList(QWidget *parent) : QWidget(parent), _ui(new Ui::PageableList) {

    // Set default page size
    _pageSize = Configuration::instance().GetValue<int>("ui.page-size");

    // Setup UI components
    _ui->setupUi(this);

    // Prefix edit
    _ui->prefixEdit->setPlaceholderText("Prefix");
    _ui->prefixEdit->setEnabled(true);
    connect(_ui->prefixEdit, &QLineEdit::textChanged, this, [this]() {
        _ui->prefixClearButton->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(_ui->prefixEdit->text());
        _prefix = _ui->prefixEdit->text();
    });

    // Prefix clear button
    _ui->prefixClearButton->setDisabled(true);
    _ui->prefixClearButton->setText(nullptr);
    _ui->prefixClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->prefixClearButton->setToolTip("Clear the prefix field");
    connect(_ui->prefixClearButton, &QPushButton::clicked, this, [this]() {
        _proxyModel->clearFilter();
        _ui->prefixClearButton->setDisabled(true);
    });

    // Item
    _ui->listView = new QListView(this);
    _ui->listView->setWordWrap(true);
    _ui->listView->setResizeMode(QListView::Adjust);
    _ui->listView->setUniformItemSizes(false);
    _ui->listView->setLineWidth(2);
    _ui->listView->setStyleSheet(R"(QListView::item {border-bottom: 1px solid #5c5c5c;})");

    _dataModel = new QStandardItemModel(_ui->listView);

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);

    _ui->listView->setModel(_proxyModel);

    // Start button
    _ui->startButton->setText(nullptr);
    _ui->startButton->setIcon(IconUtils::GetIcon("begin"));
    connect(_ui->startButton, &QPushButton::clicked, this, [this]() {
        _pageIndex = 0;
        CalculatePageStatus();
        emit PageChanged(_pageIndex, _pageSize);
    });

    // Previous button
    _ui->previousButton->setText(nullptr);
    _ui->previousButton->setIcon(IconUtils::GetIcon("previous"));
    connect(_ui->previousButton, &QPushButton::clicked, this, [this]() {
        _pageIndex--;
        if (_pageIndex < 0) {
            _pageIndex = 0;
        }
        CalculatePageStatus();
        emit PageChanged(_pageIndex, _pageSize);
    });

    // Next button
    _ui->nextButton->setText(nullptr);
    _ui->nextButton->setIcon(IconUtils::GetIcon("next"));
    connect(_ui->nextButton, &QPushButton::clicked, this, [this]() {
        _pageIndex++;
        if (_pageIndex > _maxPage) {
            _pageIndex = _maxPage;
        }
        CalculatePageStatus();
        emit PageChanged(_pageIndex, _pageSize);
    });

    _ui->endButton->setText(nullptr);
    _ui->endButton->setIcon(IconUtils::GetIcon("end"));
    connect(_ui->endButton, &QPushButton::clicked, this, [this]() {
        _pageIndex = _maxPage;
        CalculatePageStatus();
        emit PageChanged(_pageIndex, _pageSize);
    });

    _ui->pageSizeEdit->setText(QString::number(_pageSize));
    connect(_ui->pageSizeEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _pageSize = text.toLong();
        _maxPage = (_totalSize + _pageSize - 1) / _pageSize;
        CalculatePageStatus();
        emit PageChanged(_pageIndex, _pageSize);
    });

    // Add context menu
    _ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->listView, &QListView::customContextMenuRequested, this, [this](const QPoint &pos) {
        emit ContextMenuRequested(pos);
    });

    // Defaults
    _ui->pageStatusLabel->setText(QString("%1 - %2/%3").arg(0).arg(_pageSize).arg(_totalSize));
    SetLastUpdate();
}

PageableList::~PageableList() {
    delete _ui;
}

void PageableList::CalculatePageStatus() const {
    long start = _pageIndex * _pageSize;
    if (start >= _totalSize) {
        start -= _pageSize;
    }
    long end = _pageIndex * _pageSize + _pageSize;
    if (end > _totalSize) {
        end = _totalSize;
    }
    _ui->pageStatusLabel->setText(QString("%1 - %2 / %3").arg(start).arg(end).arg(_totalSize));
    SetLastUpdate();
}

void PageableList::SetStatus(const QString &message) const {
    _ui->statusLabel->setText(message);
}

void PageableList::SetLastUpdate() const {
    const QString message = "Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime());
    _ui->statusLabel->setText(message);
}

QModelIndex PageableList::GetIndexFromPosition(const QPoint &pos) const {

    const QModelIndex proxyIndex = _ui->listView->indexAt(pos);
    if (!proxyIndex.isValid())
        return {};

    return _proxyModel->mapToSource(proxyIndex);
}

QPoint PageableList::GetGlobalPosition(const QPoint &tablePosition) const {
    return _ui->listView->viewport()->mapToGlobal(tablePosition);
}
