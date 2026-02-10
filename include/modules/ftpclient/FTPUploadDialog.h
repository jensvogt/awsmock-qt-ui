//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H
#define AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H

// Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QComboBox>

// Awsmock includes
#include <utils/Configuration.h>
#include <utils/NonEmptyValidator.h>
#include <utils/IconUtils.h>
#include <utils/DroppableTreeView.h>
#include <modules/ftpclient/FTPClientThread.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class FTPUploadDialog;
}

QT_END_NAMESPACE

class FTPUploadDialog final : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit FTPUploadDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FTPUploadDialog() override;

    void ConnectionSucceeded();

    void ConnectionFinished();

    void LogInfoMessage(const QString &message) const;

    void SetupSourceTreeView();

    void ReceiveTargetListItem(const QString &perm, const QString &size, const QString &name) const;

    void TargetTreeClear() const;

    void TargetTreeItemClicked(const QModelIndex &index) const;

    /**
     * @brief Text input verification
     *
     * @param text text to verify
     */
    void UpdateLineEditStyle(const QString &text) const;

    /**
     * @brief Sets the line edit color.
     *
     * @param lineEdit line edit widget
     * @param state validator status
     */
    static void SetLineEditColor(QLineEdit *lineEdit, QValidator::State state);

private Q_SLOTS:
    /**
     * @brief Verification of the connect input fields
     */
    void VerifyConnectInputs();

    void TargetTreeFileDropped(const QString &filePath) const;

private:
    /**
     * @brief Dialog reject callback
     */
    void HandleReject();

    /**
     * UI components
     */
    Ui::FTPUploadDialog *_ui;

    /**
     * @brief Source file infos
     */
    QFileInfo sourceFileInfo;

    /**
     * @brief FTP client thread
     */
    FTPClientThread *_ftpClientThread;

    /**
     *  @brief Log item data model
     */
    QStandardItemModel *_logDataModel{};

    /**
     * @brief File tree model
     */
    QStandardItemModel *_targetTreeModel;

    /**
     * @brief Connected flag
     */
    bool _connected = false;

    /**
     * @brief Target (FTP) tree view
     */
    DroppableTreeView *_targetTreeView;
};


#endif // AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H
