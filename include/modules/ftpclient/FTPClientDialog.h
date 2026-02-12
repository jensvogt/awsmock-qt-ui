//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_FTP_CLIENT_DIALOG_H
#define AWSMOCK_QT_UI_FTP_CLIENT_DIALOG_H

// Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QComboBox>
#include <QMenu>
#include <QTableView>
#include <QInputDialog>

// Awsmock includes
#include <utils/Configuration.h>
#include <utils/NonEmptyValidator.h>
#include <utils/IconUtils.h>
#include <utils/DroppableTreeView.h>
#include <modules/ftpclient/FTPClientThread.h>
#include <components/FTPFileTree.h>
#include <components/LocalFileTree.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class FTPClientDialog;
}

QT_END_NAMESPACE

class FTPClientDialog final : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit FTPClientDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FTPClientDialog() override;

    void SetupLogPanel();

    void ConnectionSucceeded();

    void LogInfoMessage(const QString &message) const;

    void ReceiveTargetListItem(const FileInfo &fileInfo, QStandardItem *parent) const;

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

private slots:
    void TargetFolderSelectionChanged(const QString &absPath, QStandardItem *parent) const;

    /**
     * @brief Verification of the connect input fields
     */
    void HandleConnectButton();

    /**
     * @brief A target file has been dropped into the target tree view widget
     *
     * @param filePath absolute file path
     */
    void TargetTreeFileDropped(const QString &filePath) const;

    /**
     * @brief A target file has been deleted
     *
     * @param filePath absolute file path
     */
    void TargetTreeFileDelete(const QString &filePath) const;

    void TargetTreeDirectoryDelete(const QString &filePath) const;

    void TargetTreeFileRename(const QString &filePath);

    void TargetTreeAddDirectory();

    void TargetTreeDirectoryRename(const QString &filePath);

private:
    /**
     * @brief Dialog reject callback
     */
    void HandleReject();

    /**
     * UI components
     */
    Ui::FTPClientDialog *_ui;

    /**
     * @brief FTP client thread
     */
    FTPClientThread *_ftpClientThread;

    /**
     * @brief Local folder tree
     */
    LocalFileTree *_localFolderTree;

    /**
     * @brief FTP folder tree
     */
    FTPFileTree *_ftpFolderTree;

    /**
     *  @brief Log item data model
     */
    QStandardItemModel *_logDataModel{};

    /**
     * @brief Log scrolling flag
     */
    bool _logScrolling = true;

    /**
     * @brief Connected flag
     */
    bool _connected = false;
};


#endif // AWSMOCK_QT_UI_FTP_CLIENT_DIALOG_H
