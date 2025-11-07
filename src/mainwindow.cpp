#include <mainwindow.h>

#include <modules/application/ApplicationList.h>
#include <modules/dashboard/Dashboard.h>

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class ContentPage : public BasePage {
public:
    ContentPage(const QString &title, QWidget *parent = nullptr) : BasePage(parent), title(title) {
    }

    void LoadContent() {
        // Set up the layout for the individual content pages
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel(QString("<h1>%1</h1>").arg(title), this);
        titleLabel->setAlignment(Qt::AlignCenter);

        QLabel *detailLabel = new QLabel(
            QString("<p>This is the detailed content for the <b>%1</b> section.</p>").arg(title), this);
        detailLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(titleLabel);
        layout->addWidget(detailLabel);
        layout->addStretch(); // Push content to the top
    }

private:
    /**
     * Page title
     */
    QString title;
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("AwsMock UI");
    setMinimumSize(1200, 800);

    // Setup menu bar
    SetupMenuBar();

    // 1. Create the main layout container (QSplitter)
    const auto mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);

    // 2. Create the Navigation Pane (QListWidget)
    m_navPane = new QListWidget(mainSplitter);
    m_navPane->setMaximumWidth(200); // Set a maximum width for the navigation bar
    m_navPane->setMinimumWidth(150);

    m_navPane->addItem("Dashboard");
    m_navPane->addItem("SQS");
    m_navPane->addItem("SNS");
    m_navPane->addItem("S3");
    m_navPane->addItem("Application");

    // Select the first item by default
    m_navPane->setCurrentRow(0);

    // 3. Create the Content Pane (QStackedWidget)
    // QStackedWidget allows us to stack multiple widgets and show only one at a time.
    m_contentPane = new QStackedWidget(this);
    m_contentPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 4. Add the panes to the splitter
    mainSplitter->addWidget(m_navPane);
    mainSplitter->addWidget(m_contentPane);

    // Set initial proportional sizes for the panes (e.g., 20% nav, 80% content)
    QList<int> sizes;
    sizes << 200 << 600; // Initial widths in pixels (QSplitter prefers list of sizes)
    mainSplitter->setSizes(sizes);

    // 5. Set the splitter as the central widget of the QMainWindow
    setCentralWidget(mainSplitter);

    // 6. Connect navigation signal to content slot
    // When the selected row in the list changes, update the content pane index.
    connect(m_navPane, &QListWidget::currentRowChanged, this, &MainWindow::NavigationSelectionChanged);

    // Create dashboard
    NavigationSelectionChanged(0);

    // "Status bar" at the bottom
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() {
}

void MainWindow::SetupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    // File menu
    const auto importAction = new QAction(IconUtils::GetIcon("dark", "import"), tr("&Import infrastructure"), this);
    connect(importAction, &QAction::triggered, this, &MainWindow::ImportInfrastructure);
    fileMenu->addAction(importAction);

    const auto exportAction = new QAction(IconUtils::GetIcon("dark", "export"), tr("&Export infrastructure"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::ExportInfrastructure);
    fileMenu->addAction(exportAction);

    fileMenu->addSeparator();

    const auto exitAction = new QAction(IconUtils::GetIcon("dark", "exit"), tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);
    fileMenu->addAction(exitAction);

    // Edit Menu
    const auto prefAction = new QAction(IconUtils::GetIcon("dark", "preferences"), tr("&Preferences"), this);
    connect(prefAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    editMenu->addAction(prefAction);

    // Edit Menu
    const auto helpAction = new QAction(IconUtils::GetIcon("dark", "help"), tr("&Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    editMenu->addAction(helpAction);
}

void MainWindow::ImportInfrastructure() {
    qDebug() << "Import Infrastructure";
}

void MainWindow::ExportInfrastructure() {
    qDebug() << "Export Infrastructure";
}

void MainWindow::EditPreferences() {
    if (EditPreferencesDialog dialog; dialog.exec() == QDialog::Accepted) {
        const QString info = QString("BaseUrl: %1").arg(dialog.GetBaseUrl());

        QMessageBox::information(nullptr, "User Info", info);
    }
}

void MainWindow::NavigationSelectionChanged(const int currentRow) {
    currentWidgetIndex = currentRow;
    if (!loadedPages.contains(currentRow)) {
        // Lazy load page
        BasePage *page = CreatePage(currentRow);
        loadedPages[currentRow] = page;
        m_contentPane->addWidget(page);
    }
    m_contentPane->setCurrentWidget(loadedPages[currentRow]);
    loadedPages[currentRow]->StartAutoUpdate();
}

void MainWindow::UpdateStatusBar(const QString &text) const {
    statusBar()->showMessage(text);
}

BasePage *MainWindow::CreatePage(const int currentRow) {
    switch (currentRow) {
        case 0: {
            const auto dashboardPage = new Dashboard("Dashboard", m_contentPane);

            // Connect child's signal to update status bar
            connect(dashboardPage, &SQSQueueList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            return dashboardPage;
        }
        case 1: {
            const auto queueListPage = new SQSQueueList("SQS Queue List");

            // Connect child's signal to update status bar
            connect(queueListPage, &SQSQueueList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            // Route to the message list
            connect(queueListPage, &SQSQueueList::ShowMessages, this,
                    [=](const QString &queueArn, const QString &queueUrl) {
                        // Stop the auto updater
                        queueListPage->StopAutoUpdate();

                        // Get the Queue name
                        const QString queueName = queueArn.mid(queueArn.lastIndexOf(":") + 1);

                        // Create the message list page
                        const auto messageListPage = new SQSMessageList("SQS Message List: " + queueName, queueArn,
                                                                        queueUrl, nullptr);

                        // Add it to the loaded pages list
                        m_contentPane->addWidget(messageListPage);
                        m_contentPane->setCurrentWidget(messageListPage);

                        connect(messageListPage, &SQSMessageList::StatusUpdateRequested, this,
                                &MainWindow::UpdateStatusBar);

                        // Connect the back button
                        connect(messageListPage, &SQSMessageList::BackToQueueList, this, [&]() {
                            NavigationSelectionChanged(1);
                        });

                        // Start auto updater
                        messageListPage->StartAutoUpdate();
                    });
            return queueListPage;
        }
        case 2: {
            const auto topicListPage = new SNSTopicList("SNS Topic List");

            // Connect child's signal to update status bar
            connect(topicListPage, &SNSTopicList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            // Route to the message list
            connect(topicListPage, &SNSTopicList::ShowSnsMessages, this, [=](const QString &topicArn) {
                // Stop the auto updater
                topicListPage->StopAutoUpdate();

                // Get the Queue name
                const QString topicName = topicArn.mid(topicArn.lastIndexOf(":") + 1);

                // Create the message list page
                const auto messageListPage = new SNSMessageList("SNS Message List: " + topicName, topicArn,nullptr);

                // Add it to the loaded pages list
                m_contentPane->addWidget(messageListPage);
                m_contentPane->setCurrentWidget(messageListPage);

                connect(messageListPage, &SNSMessageList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

                // Connect the back button
                connect(messageListPage, &SNSMessageList::BackToTopicList, this, [&]() {
                    NavigationSelectionChanged(2);
                });

                // Start auto updater
                messageListPage->StartAutoUpdate();
            });

            return topicListPage;
        }
        case 3:
            return new ContentPage("S3");

        case 4: {
            const auto applicationPage = new ApplicationList("Applications", m_contentPane);

            // Connect child's signal to update status bar
            connect(applicationPage, &ApplicationList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            return applicationPage;
        }
        default:
            return nullptr;
    }
}

void MainWindow::Exit() {
    QApplication::quit();
}
