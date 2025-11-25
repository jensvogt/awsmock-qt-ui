#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <utility>

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QMap>
#include <QLabel>
#include <QStatusBar>
#include <QApplication>
#include <QSplitter>
#include <QTreeView>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QStyleFactory>
#include <QImageReader>

#include <utils/About.h>
#include <utils/EditConfigDialog.h>
#include <utils/EventBus.h>
#include <utils/BasePage.h>
#include <utils/ScopedTimer.h>
#include <modules/application/ApplicationList.h>
#include <modules/dashboard/Dashboard.h>
#include <modules/ftp/FTPUploadDialog.h>
#include <modules/s3/S3BucketList.h>
#include <modules/s3/S3ObjectList.h>
#include <modules/sns/SNSTopicList.h>
#include <modules/sns/SNSMessageList.h>
#include <modules/sqs/SQSQueueList.h>
#include <modules/sqs/SQSMessageList.h>
#include <modules/infrastructure/InfrastructureService.h>

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:
    void NavigationSelectionChanged(int currentRow);

    void UpdateStatusBar(const QString &text) const;

private:
    // Setup menu bar
    void SetupMenuBar();

    void ImportInfrastructure() const;

    static void ImportInfrastructureResponse();

    void ExportInfrastructure() const;

    static void WriteInfrastructureExport(const QString &filename, const QString &exportResponse);

    void CleanInfrastructure() const;

    static void CleanInfrastructureResponse();

    void FtpUpload();

    static void EditPreferences();

    BasePage *CreatePage(int currentRow);

    static void Exit();

    /**
     * @brief Main menu bar
     */
    QMenuBar *mainMenuBar{};

    /**
     * @brief Navigation pane
     */
    QListWidget *m_navPane;

    /**
     * @brief Content pane
     */
    QStackedWidget *m_contentPane;

    /**
     * @brief List of loaded pages
     */
    QMap<int, BasePage *> loadedPages;

    /**
     * @brief Current widget shown in the content pane
     */
    int currentWidgetIndex = 0;

    /**
     * @brief Infrastructure service
     */
    InfraStructureService *_infraStructureService{};

    /**
     * @brief Status bar
     */
    QStatusBar *_statusBar{};

    /**
     * @brief Server label in status bar
     */
    QLabel *_serverName{};

    /**
     * Scoped timer
     */
    ScopeTimer *_timer{};

    /**
     * @brief Server label in status bar
     */
    QLabel *_timerLabel{};
};
#endif // MAINWINDOW_H
