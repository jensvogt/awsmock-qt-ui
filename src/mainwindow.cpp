#include <mainwindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Connect infrastructure signals
    _infraStructureService = new InfraStructureService();
    connect(_infraStructureService, &InfraStructureService::ImportResponseSignal, this, &ImportInfrastructureResponse);
    connect(_infraStructureService, &InfraStructureService::ExportResponseSignal, this, &WriteInfrastructureExport);
    connect(_infraStructureService, &InfraStructureService::CleanResponseSignal, this, &CleanInfrastructureResponse);

    setWindowTitle("AwsMock UI v" + QString(APP_VERSION));
    resize(1600, 900);

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
    m_navPane->addItem("Lambda");

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
    _serverName = new QLabel(QString("Server: ") + Configuration::instance().GetValue<QString>("server.base-url", ""),
                             this);
    _statusBar = new QStatusBar(this);
    _statusBar->showMessage("Ready");
    _statusBar->addPermanentWidget(_serverName);
    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, this,
            [this](const QString &key, const QString &value) {
                if (key == "server.base-url") {
                    _serverName->setText(value);
                }
            });

    _timerLabel = new QLabel("", this);
    _statusBar->addWidget(_timerLabel);
    connect(&EventBus::instance(), &EventBus::TimerSignal, [this](const QString &name, qint64 elapsed) {
        const QString msg = "Last update: " + QDateTime::currentDateTime().toString("hh:mm:ss") + " [" +
                            QString::number(elapsed) + "ms]";
        _statusBar->showMessage(msg);
    });
    setStatusBar(_statusBar);

    connect(&Configuration::instance(), &Configuration::ConfigurationChanged,
            [&](const QString &key, const QString &value) {
                if (key == "ui.style") {
                    qApp->setStyle(QStyleFactory::create(value));
                }
                if (key == "ui.style-type") {
                    if (value == "Dark") {
                        qApp->setStyle(
                            QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
                        if (QFile f(":/styles/styles/dark.qss"); f.open(QFile::ReadOnly)) {
                            qApp->setStyleSheet(f.readAll());
                        }
                    } else {
                        qApp->setStyleSheet("");
                        qApp->setStyle(
                            QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
                    }
                }
            });
}

MainWindow::~MainWindow() = default;

void MainWindow::SetupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *ftpMenu = menuBar()->addMenu(tr("&FTP"));
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    // File menu
    const auto importAction = new QAction(IconUtils::GetIcon("import"), tr("&Import infrastructure"), this);
    connect(importAction, &QAction::triggered, this, &MainWindow::ImportInfrastructure);
    fileMenu->addAction(importAction);

    const auto exportAction = new QAction(IconUtils::GetIcon("export"), tr("&Export infrastructure"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::ExportInfrastructure);
    fileMenu->addAction(exportAction);

    const auto cleanAction = new QAction(IconUtils::GetIcon("clean"), tr("&Clean infrastructure"), this);
    connect(cleanAction, &QAction::triggered, this, &MainWindow::CleanInfrastructure);
    fileMenu->addAction(cleanAction);

    fileMenu->addSeparator();

    const auto exitAction = new QAction(IconUtils::GetIcon("exit"), tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);
    fileMenu->addAction(exitAction);

    // Edit Menu
    const auto prefAction = new QAction(IconUtils::GetIcon("preferences"), tr("&Preferences"), this);
    connect(prefAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    editMenu->addAction(prefAction);

    // FTP menu
    const auto uploadAction = new QAction(IconUtils::GetIcon("upload"), tr("&Upload file"), this);
    connect(uploadAction, &QAction::triggered, this, &MainWindow::FtpUpload);
    ftpMenu->addAction(uploadAction);

    // Help Menu
    const auto helpAction = new QAction(IconUtils::GetIcon("help"), tr("&Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    helpMenu->addAction(helpAction);

    // About
    const auto aboutAction = new QAction(IconUtils::GetIcon("about"), tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, []() {
        About aboutDialog;
        aboutDialog.exec();
    });
    helpMenu->addAction(aboutAction);
}

void MainWindow::ImportInfrastructure() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>(
        "ui.default-directory", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Configuration File", defaultDir,
                                                              filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }

        const QByteArray jsonData = file.readAll();
        file.close();

        _infraStructureService->ImportInfrastructure(jsonData);
        Configuration::instance().SetValue<QString>("ui.default-directory", QFileInfo(filePath).absolutePath());
    }
}

void MainWindow::ImportInfrastructureResponse() {
    QMessageBox::information(nullptr, "Information", "Infrastructure imported");
}

void MainWindow::ExportInfrastructure() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>(
        "ui.default-directory", "/usr/local/awsmock-qt-ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir,
                                                              filter); !filePath.isEmpty()) {
        _infraStructureService->ExportInfrastructure(filePath);
        Configuration::instance().SetValue<QString>("ui.default-directory", QFileInfo(filePath).absolutePath());
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
    const auto dialog = new FTPUploadDialog(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::EditPreferences() {
    EditConfigDialog dialog;
    dialog.exec();
}

void MainWindow::NavigationSelectionChanged(const int currentRow) {
    currentWidgetIndex = currentRow;
    if (!loadedPages.contains(currentRow)) {
        // Lazy load page
        BasePage *page = CreatePage(currentRow);
        loadedPages[currentRow] = page;
        m_contentPane->addWidget(page);
    }
    for (const auto &loadedPage: loadedPages) {
        if (loadedPage) {
            loadedPage->StopAutoUpdate();
        }
    }
    loadedPages[currentRow]->StartAutoUpdate();
    m_contentPane->setCurrentWidget(loadedPages[currentRow]);
}

void MainWindow::UpdateStatusBar(const QString &text) const {
    if (_statusBar)
        _statusBar->showMessage(text);
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
                    [this, queueListPage](const QString &queueArn, const QString &queueUrl) {
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
            connect(topicListPage, &SNSTopicList::ShowSnsMessages, this,
                    [this, topicListPage](const QString &topicArn) {
                        // Stop the auto updater
                        topicListPage->StopAutoUpdate();

                        // Get the Queue name
                        const QString topicName = topicArn.mid(topicArn.lastIndexOf(":") + 1);

                        // Create the message list page
                        const auto messageListPage = new SNSMessageList("SNS Message List: " + topicName, topicArn,
                                                                        nullptr);

                        // Add it to the loaded pages list
                        m_contentPane->addWidget(messageListPage);
                        m_contentPane->setCurrentWidget(messageListPage);

                        connect(messageListPage, &SNSMessageList::StatusUpdateRequested, this,
                                &MainWindow::UpdateStatusBar);

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

            // Route to the S3 object list
            connect(bucketListPage, &S3BucketList::ShowS3Objects, this,
                    [this,bucketListPage](const QString &bucketName) {
                        // Stop the auto updater
                        bucketListPage->StopAutoUpdate();

                        // Create the message list page
                        const auto objectListPage = new S3ObjectList("S3 Object List: " + bucketName, bucketName,
                                                                     nullptr);

                        // Add it to the loaded pages list
                        m_contentPane->addWidget(objectListPage);
                        m_contentPane->setCurrentWidget(objectListPage);
                        connect(objectListPage, &S3ObjectList::StatusUpdateRequested, this,
                                &MainWindow::UpdateStatusBar);

                        // Connect the back button
                        connect(objectListPage, &S3ObjectList::BackToBucketList, this, [&]() {
                            NavigationSelectionChanged(3);
                        });

                        // Start auto updater
                        objectListPage->StartAutoUpdate();
                    });

            return bucketListPage;
        }

        case 4: {
            const auto applicationPage = new ApplicationList("Applications", m_contentPane);

            // Connect child's signal to update status bar
            connect(applicationPage, &ApplicationList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            return applicationPage;
        }

        case 5: {
            const auto lambdaPage = new LambdaList("Lambdas", m_contentPane);

            // Connect child's signal to update status bar
            connect(lambdaPage, &LambdaList::StatusUpdateRequested, this, &MainWindow::UpdateStatusBar);

            return lambdaPage;
        }

        default:
            return nullptr;
    }
}

void MainWindow::Exit() {
    QApplication::quit();
}
