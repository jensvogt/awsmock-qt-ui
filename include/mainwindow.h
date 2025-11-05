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

#include <modules/sns/SNSTopicList.h>
#include <modules/sns/SNSMessageList.h>
#include <modules/sqs/SQSQueueList.h>
#include <modules/sqs/SQSMessageList.h>
#include <utils/BasePage.h>
#include <utils/EditPreferencesDialog.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void NavigationSelectionChanged(int currentRow);
    void UpdateStatusBar(const QString &text);

private:
    // Setup menu bar
    void SetupMenuBar();
    void ImportInfrastructure();
    void ExportInfrastructure();
    void EditPreferences();
    BasePage* CreatePage(int currentRow);
    void Exit();

    QMenuBar* mainMenuBar;
    QListWidget *m_navPane;
    QStackedWidget *m_contentPane;
    QMap<int, BasePage*> loadedPages;
    QLabel* myStatusBar;
    QLabel* timeLabel;

    int currentWidgetIndex = 0;
};
#endif // MAINWINDOW_H
