#include <mainwindow.h>

#include <modules/application/ApplicationList.h>
#include <modules/dashboard/Dashboard.h>

#include <utility>

#include "modules/ftp/FTPUploadDialog.h"
#include "modules/s3/S3BucketList.h"

/**
 * @brief Helper widget for the content area.
 * Displays a simple message based on the section selected.
 */
class ContentPage final : public BasePage {
public:
    explicit ContentPage(QString title, QWidget *parent = nullptr) : BasePage(parent), title(std::move(title)) {
    }

    void LoadContent() override {
        // Set up the layout for the individual content pages
        const auto layout = new QVBoxLayout(this);

        const auto titleLabel = new QLabel(QString("<h1>%1</h1>").arg(title), this);
        titleLabel->setAlignment(Qt::AlignCenter);

        const auto detailLabel = new QLabel(
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

    // Connect infrastructure signals
    _infraStructureService = new InfraStructureService();
    connect(_infraStructureService, &InfraStructureService::ImportResponseSignal, this, &ImportInfrastructureResponse);
    connect(_infraStructureService, &InfraStructureService::ExportResponseSignal, this, &WriteInfrastructureExport);
    connect(_infraStructureService, &InfraStructureService::CleanResponseSignal, this, &CleanInfrastructureResponse);

    setWindowTitle("AwsMock UI");
    resize(1200, 800);

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

MainWindow::~MainWindow() = default;

void MainWindow::SetupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *ftpMenu = menuBar()->addMenu(tr("&FTP"));
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    // File menu
    const auto importAction = new QAction(IconUtils::GetIcon("dark", "import"), tr("&Import infrastructure"), this);
    connect(importAction, &QAction::triggered, this, &MainWindow::ImportInfrastructure);
    fileMenu->addAction(importAction);

    const auto exportAction = new QAction(IconUtils::GetIcon("dark", "export"), tr("&Export infrastructure"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::ExportInfrastructure);
    fileMenu->addAction(exportAction);

    const auto cleanAction = new QAction(IconUtils::GetIcon("dark", "clean"), tr("&Clean infrastructure"), this);
    connect(cleanAction, &QAction::triggered, this, &MainWindow::CleanInfrastructure);
    fileMenu->addAction(cleanAction);

    fileMenu->addSeparator();

    const auto exitAction = new QAction(IconUtils::GetIcon("dark", "exit"), tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);
    fileMenu->addAction(exitAction);

    // Edit Menu
    const auto prefAction = new QAction(IconUtils::GetIcon("dark", "preferences"), tr("&Preferences"), this);
    connect(prefAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    editMenu->addAction(prefAction);

    // FTP menu
    const auto uploadAction = new QAction(IconUtils::GetIcon("dark", "upload"), tr("&Upload file"), this);
    connect(uploadAction, &QAction::triggered, this, &MainWindow::FtpUpload);
    ftpMenu->addAction(uploadAction);

    // Help Menu
    const auto helpAction = new QAction(IconUtils::GetIcon("dark", "help"), tr("&Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    editMenu->addAction(helpAction);
}

void MainWindow::ImportInfrastructure() const {

    // Create a QFileDialog set to select existing files
    const QString filter = "JSON Files (*.json);;All Files (*.*)";
    const QString defaultDir = Configuration::instance().GetDefaultDirectory();

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }

        const QByteArray jsonData = file.readAll();
        file.close();

        _infraStructureService->ImportInfrastructure(jsonData);
        Configuration::instance().SetDefaultDirectory(QFileInfo(filePath).absolutePath());
    }
}

void MainWindow::ImportInfrastructureResponse() {
    QMessageBox::information(nullptr, "Information", "Infrastructure imported");
}

void MainWindow::ExportInfrastructure() const {

    // Create a QFileDialog set to select existing files
    const QString filter = "JSON Files (*.json);;All Files (*.*)";
    const QString defaultDir = Configuration::instance().GetDefaultDirectory();

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        _infraStructureService->ExportInfrastructure(filePath);
        Configuration::instance().SetDefaultDirectory(QFileInfo(filePath).absolutePath());
    }
}

void MainWindow::WriteInfrastructureExport(const QString &filename, const QString &exportResponse) {

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Warning", "Couldn't open file for writing: " + file.fileName());
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(exportResponse.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(nullptr, "Warning", "Could not convert to pretty print, file: " + file.fileName());
        return;
    }

    // Write formatted (pretty-printed) JSON
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    QMessageBox::information(nullptr, "Information", "Infrastructure saved to file: " + file.fileName());
}

void MainWindow::CleanInfrastructure() const {
    _infraStructureService->CleanInfrastructure();
}

void MainWindow::CleanInfrastructureResponse() {
    QMessageBox::information(nullptr, "Information", "Infrastructure cleaned");
}

void MainWindow::FtpUpload() {
    if (FTPUploadDialog dialog; dialog.exec() == QDialog::Accepted) {
        //QMessageBox::information(nullptr, "User Info", "info");
    }
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
                const auto messageListPage = new SNSMessageList("SNS Message List: " + topicName, topicArn, nullptr);

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

        case 3: {
            const auto bucketListPage = new S3BucketList("S3 Bucket List");

            // Connect child's signal to update status bar
            connect(bucketListPage, &S3BucketList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            // Route to the message list
            // connect(bucketListPage, &S3BucketList::ShowS3Messages, this, [=](const QString &bucketArn) {
            //     // Stop the auto updater
            //     bucketListPage->StopAutoUpdate();
            //
            //     // Get the Queue name
            //     const QString bucketName = bucketArn.mid(bucketArn.lastIndexOf(":") + 1);
            //
            //     // Create the message list page
            //     const auto messageListPage = new S3MessageList("S3 Message List: " + bucketName, bucketArn, nullptr);
            //
            //     // Add it to the loaded pages list
            //     m_contentPane->addWidget(messageListPage);
            //     m_contentPane->setCurrentWidget(messageListPage);
            //
            //     connect(messageListPage, &S3MessageList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);
            //
            //     // Connect the back button
            //     connect(messageListPage, &S3MessageList::BackToBucketList, this, [&]() {
            //         NavigationSelectionChanged(2);
            //     });
            //
            //     // Start auto updater
            //     messageListPage->StartAutoUpdate();
            // });

            return bucketListPage;
        }

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
