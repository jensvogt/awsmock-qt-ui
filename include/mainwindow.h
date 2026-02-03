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

#include <Version.h>
#include <utils/About.h>
#include <utils/EditConfigDialog.h>
#include <utils/EventBus.h>
#include <utils/BasePage.h>
#include <utils/ScopedTimer.h>
#include <modules/module/ShowInfrastructure.h>
#include <modules/application/ApplicationList.h>
#include <modules/dashboard/Dashboard.h>
#include <modules/ftp/FTPUploadDialog.h>
#include <modules/s3/S3BucketList.h>
#include <modules/s3/S3ObjectList.h>
#include <modules/sns/SNSTopicList.h>
#include <modules/sns/SNSMessageList.h>
#include <modules/sqs/SQSQueueList.h>
#include <modules/sqs/SQSMessageList.h>
#include <modules/lambda/LambdaList.h>
#include <modules/module/ModuleService.h>
#include <modules/docker/DockerStatsDialog.h>
#include <modules/ssm/SSMParameterList.h>
#include <modules/secretsmanager/SecretList.h>
#include <modules/dynamodb/DynamoDbTableList.h>
#include <modules/dynamodb/DynamoDbItemList.h>

#define PAGE_DASHBOARD 0
#define PAGE_SQS 1
#define PAGE_SNS 2
#define PAGE_S3 3
#define PAGE_APPLICATION 4
#define PAGE_LAMBDA 5
#define PAGE_SECRETS_MANAGER 6
#define PAGE_SSM 7
#define PAGE_DYNAMODB 8

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

    static void ShowInfrastructureDialog();

    void FtpUpload();

    void DockerStats();

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
    ModuleService *_moduleService{};

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
