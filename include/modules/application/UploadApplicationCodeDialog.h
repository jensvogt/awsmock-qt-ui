//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_UPLOADAPPLICATIONCODEDIALOG_H
#define AWSMOCK_QT_UI_UPLOADAPPLICATIONCODEDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class UploadApplicationCodeDialog;
}

QT_END_NAMESPACE

class UploadApplicationCodeDialog final : public QDialog {
    Q_OBJECT

public:
    explicit UploadApplicationCodeDialog(const QString name, QWidget *parent = nullptr);

    ~UploadApplicationCodeDialog() override;

private:
    Ui::UploadApplicationCodeDialog *ui;
};


#endif //AWSMOCK_QT_UI_UPLOADAPPLICATIONCODEDIALOG_H
