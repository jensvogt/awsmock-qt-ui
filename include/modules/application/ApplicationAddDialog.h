//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_ADD_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_ADD_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QComboBox>
#include <QMimeData>
#include <QFileInfo>
#include <QDialogButtonBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>

#include <modules/application/ApplicationService.h>

class ApplicationAddDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationAddDialog(QWidget *parent = nullptr);

    ~ApplicationAddDialog() override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

private Q_SLOTS:
    /**
     * @brief Slot executed when the "Open" button is clicked.
     * Launches a QFileDialog to select multiple files.
     */
    void OpenFilesDialog();
    void VerifyAllInputs();
    void UpdateLineEditStyle(const QString &text) const;

private:

    /**
     * @brief Line edit editor
     */
    static void SetLineEditColor(QLineEdit *lineEdit, QValidator::State state);

    /**
     * @brief Upload file to server
     */
    void UploadFile(const QFileInfo &fileInfo);

    QLabel *statusLabel, *nameLabel, *versionLabel, *runtimeLabel, *privatePortLabel, *publicPortLabel, *fileNameLabel;
    QLineEdit *nameEdit, *fileNameEdit, *versionEdit, *privatePortEdit, *publicPortEdit;
    QComboBox *runtimeComboBox;
    QDialogButtonBox *buttonBox;
    QFileInfo filePath;

    /**
     * @brief Service
     */
    ApplicationService *applicationService;
};


#endif //AWSMOCK_QT_UI_APPLICATION_ADD_DIALOG_H
