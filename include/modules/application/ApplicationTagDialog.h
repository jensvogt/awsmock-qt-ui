//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_TAG_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_TAG_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationTagDialog;
}

QT_END_NAMESPACE

class ApplicationTagDialog : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationTagDialog(const QString &key, const QString &value, const bool add, QWidget *parent = nullptr);

    ~ApplicationTagDialog() override;

    void HandleAccept();

    void HandleReject();

    QString GetValue();

    QString GetKey();

private:
    Ui::ApplicationTagDialog *_ui;
    QString _key;
    QString _value;
};


#endif //AWSMOCK_QT_UI_APPLICATION_TAG_DIALOG_H
