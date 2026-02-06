//
// Created by vogje01 on 2/6/26.
//

#ifndef AWSMOCK_QT_UI_KMS_KEY_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_KMS_KEY_EDIT_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class KMSKeyEditDialog;
}

QT_END_NAMESPACE

class KMSKeyEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit KMSKeyEditDialog(QWidget *parent = nullptr);

    ~KMSKeyEditDialog() override;

private:
    Ui::KMSKeyEditDialog *_ui;
};


#endif // AWSMOCK_QT_UI_KMS_KEY_EDIT_DIALOG_H
