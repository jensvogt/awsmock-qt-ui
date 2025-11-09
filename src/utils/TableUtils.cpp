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
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const long value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, QVariant::fromValue(value));
    tableWidget->setItem(row, col, item);
}

void TableUtils::SetColumn(QTableWidget *tableWidget, const int row, const int col, const QDateTime &value) {
    const auto item = new QTableWidgetItem();
    item->setData(Qt::EditRole, value.toString("yyyy-MM-dd hh:mm:ss"));
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

void TableUtils::SetHiddenColumn(QTableWidget *tableWidget, const int row, const int col, const bool value) {
    const auto checkItem = new QTableWidgetItem();
    checkItem->setCheckState(value ? Qt::Checked : Qt::Unchecked);
    checkItem->setFlags(checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    tableWidget->setItem(row, col, checkItem);
}
