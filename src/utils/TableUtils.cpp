//
// Created by vogje01 on 11/9/25.
//

#include <utils/TableUtils.h>


void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const QString &value) {
    tableWidget->setItem(row, col, new QTableWidgetItem(value));
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const int value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, QVariant::fromValue(value));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const long value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, QVariant::fromValue(value));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const double value, const int digits) {
    const auto item = new QTableWidgetItem();
    const QString formattedValue = QString::number(value, 'f', digits);
    item->setData(Qt::EditRole, formattedValue);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const QDateTime &value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, DateTimeUtils::GetDateTimeFormat(value));
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const bool value, const QIcon &enabledIcon, const QIcon &disabledIcon) {
    auto *iconItem = new QTableWidgetItem();
    iconItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    iconItem->setData(Qt::DisplayRole, value ? 1 : 0);
    iconItem->setText("");
    iconItem->setIcon(value ? enabledIcon : disabledIcon);
    tableWidget->setItem(row, col, iconItem);
}

void TableUtils::SetHiddenColumn(QTableWidget *tableWidget, const int row, const int col, const QString &value) {
    const auto item = new QTableWidgetItem;
    item->setData(Qt::EditRole, value);
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetHiddenColumn(QStandardItemModel *tableModel, const int row, const int col, const QString &value) {
    const auto item = new QStandardItem;
    item->setData(value, Qt::EditRole);
    tableModel->setItem(row, col, item);
}

void TableUtils::SetHiddenColumn(QTableWidget *tableWidget, const int row, const int col, const bool value) {
    const auto checkItem = new QTableWidgetItem();
    checkItem->setCheckState(value ? Qt::Checked : Qt::Unchecked);
    checkItem->setFlags(checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    tableWidget->setItem(row, col, checkItem);
}

void TableUtils::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const QString &value, const Qt::Alignment alignment) {
    const auto item = new QStandardItem(value);
    item->setTextAlignment(alignment);
    const QModelIndex index = dataModel->index(row, col);
    dataModel->setData(index, QVariant(alignment));
    dataModel->setData(index, value, Qt::EditRole);
    dataModel->setData(index, value, Qt::DisplayRole);
    dataModel->setItem(row, col, item);
}

void TableUtils::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const int value) {
    const QModelIndex index = dataModel->index(row, col);
    dataModel->setData(index, QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    dataModel->setData(index, value, Qt::UserRole);
    dataModel->setData(index, value, Qt::DisplayRole);
}

void TableUtils::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const long value) {
    const QModelIndex index = dataModel->index(row, col);
    dataModel->setData(index, QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    dataModel->setData(index, static_cast<qlonglong>(value), Qt::UserRole);
    dataModel->setData(index, static_cast<qlonglong>(value), Qt::DisplayRole);
}

void TableUtils::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const double value, const int digits) {
    const QModelIndex index = dataModel->index(row, col);
    dataModel->setData(index, QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    dataModel->setData(index, value, Qt::UserRole);
    dataModel->setData(index, QString::number(value, 'f', digits), Qt::DisplayRole);
}

void TableUtils::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const QDateTime &value) {
    dataModel->setItem(row, col, new QStandardItem(DateTimeUtils::GetDateTimeFormat(value)));
}

void TableUtils::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const bool value, const QIcon &enabledIcon, const QIcon &disabledIcon) {
    auto *iconItem = new QStandardItem();
    iconItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    iconItem->setData(Qt::DisplayRole, value ? 1 : 0);
    iconItem->setText("");
    iconItem->setIcon(value ? enabledIcon : disabledIcon);
    dataModel->setItem(row, col, iconItem);
}
