#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <modules/sns/SNSTopicList.h>
#include <modules/sns/SNSMessageList.h>
#include <modules/sqs/SQSQueueList.h>
#include <modules/sqs/SQSMessageList.h>
#include <utils/BasePage.h>
#include <utils/EditPreferencesDialog.h>

#include "modules/infrastructure/InfrastructureService.h"

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

    static void FtpUpload();

    static void EditPreferences();

    BasePage *CreatePage(int currentRow);

    void Exit();

    QMenuBar *mainMenuBar{};
    QListWidget *m_navPane;
    QStackedWidget *m_contentPane;
    QMap<int, BasePage *> loadedPages;
    QLabel *myStatusBar{};
    QLabel *timeLabel{};

    int currentWidgetIndex = 0;

    /**
     * @brief Infrastructure service
     */
    InfraStructureService *_infraStructureService{};
};
#endif // MAINWINDOW_H
