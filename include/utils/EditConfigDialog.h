//
// Created by vogje01 on 11/17/25.
//

#ifndef AWSMOCK_QT_UI_EDITCONFIGDIALOG_H
#define AWSMOCK_QT_UI_EDITCONFIGDIALOG_H

#include <QDialog>

#include <utils/Configuration.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class EditConfigDialog;
}

QT_END_NAMESPACE

class EditConfigDialog final : public QDialog {
    Q_OBJECT

public:
    explicit EditConfigDialog(QWidget *parent = nullptr);

    ~EditConfigDialog() override;

private slots:
    void HandleAccept();

    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::EditConfigDialog *_ui;

    /**
     * @brief changed flag
     */
    bool _changed = false;
};


#endif //AWSMOCK_QT_UI_EDITCONFIGDIALOG_H
