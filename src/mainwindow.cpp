#include <mainwindow.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Connect infrastructure signals
    _moduleService = new ModuleService();
    connect(_moduleService, &ModuleService::ImportResponseSignal, this, &ImportInfrastructureResponse);
    connect(_moduleService, &ModuleService::ExportResponseSignal, this, &WriteInfrastructureExport);
    connect(_moduleService, &ModuleService::CleanResponseSignal, this, &CleanInfrastructureResponse);

    // Start pinging server
    StartServerPing();

    // Setup menu bar
    SetupMenuBar();

    // Main widget
    QWidget *mainWidget = new MainWidget(this);

    // Set the splitter as the central widget of the QMainWindow
    setCentralWidget(mainWidget);

    connect(&Configuration::instance(), &Configuration::ConfigurationChanged, this, [this](const QString &key, const QString &value) {
        if (key == "ui.style") {
            qApp->setStyle(QStyleFactory::create(value));
        }
        if (key == "ui.style-type") {
            if (value == "Dark") {
                qApp->setStyle(QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
                if (QFile f(":/styles/styles/dark.qss"); f.open(QFile::ReadOnly)) {
                    qApp->setStyleSheet(f.readAll());
                }
            } else {
                qApp->setStyleSheet("");
                qApp->setStyle(QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
            }
        }
    });

    // Updater
    _updateChecker = new UpdateChecker(this);
    connect(_updateChecker, &UpdateChecker::UpdateAvailable, this, [](const QString &version) {
        if (version.isEmpty()) {
            QMessageBox::information(nullptr, "Info", "You have already the latest version.");
        } else {
            QMessageBox::information(nullptr, "Info", "A new version is available, version: " + version);
        }
    });
}

MainWindow::~MainWindow() {
    _pingThread->quit();
    _pingThread->wait(3000);
    delete _pingThread;
};

void MainWindow::StartServerPing() {
    const int interval = Configuration::instance().GetValue<int>("ui.auto-update-period", 10);

    _pingThread = new QThread(this);
    _pingTimer = new QTimer();
    _pingTimer->setInterval(interval * 1000);

    // Move timer to thread
    _pingTimer->moveToThread(_pingThread);

    // Start the timer when the thread starts. By connecting directly to the timer, Qt uses a 'QueuedConnection'
    // to cross the thread boundary safely.
    connect(_pingThread, &QThread::started, _pingTimer, QOverload<>::of(&QTimer::start));

    // Perform the ping
    connect(_pingTimer, &QTimer::timeout, this, [this]() {
        _moduleService->PingServer();
    });

    // Cleanup
    connect(_pingThread, &QThread::finished, _pingTimer, &QObject::deleteLater);

    _pingThread->start();
}

void MainWindow::SetupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
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

    const auto showAction = new QAction(IconUtils::GetIcon("show"), tr("&Show infrastructure file"), this);
    connect(showAction, &QAction::triggered, this, &MainWindow::ShowInfrastructureDialog);
    fileMenu->addAction(showAction);

    fileMenu->addSeparator();

    const auto exitAction = new QAction(IconUtils::GetIcon("exit"), tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);
    fileMenu->addAction(exitAction);

    // Edit Menu
    const auto prefAction = new QAction(IconUtils::GetIcon("preferences"), tr("&Preferences"), this);
    connect(prefAction, &QAction::triggered, this, &MainWindow::EditPreferences);
    editMenu->addAction(prefAction);

    // Tools menu
    const auto ftpClientAction = new QAction(IconUtils::GetIcon("upload"), tr("&FTP client"), this);
    connect(ftpClientAction, &QAction::triggered, this, &MainWindow::FtpUpload);
    toolsMenu->addAction(ftpClientAction);

    const auto dockerStatsAction = new QAction(IconUtils::GetIcon("docker-stats"), tr("&Docker Statistics"), this);
    connect(dockerStatsAction, &QAction::triggered, this, &MainWindow::DockerStats);
    toolsMenu->addAction(dockerStatsAction);

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

    // Check for updates
    const auto updateAction = new QAction(IconUtils::GetIcon("update"), tr("Check for &Update"), this);
    connect(updateAction, &QAction::triggered, this, [this]() {
        _updateChecker->checkForUpdates();
    });
    helpMenu->addAction(updateAction);
}

void MainWindow::ImportInfrastructure() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ImportInfrastructure", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }

        const QByteArray jsonData = file.readAll();
        file.close();

        _moduleService->ImportInfrastructure(jsonData);
        Configuration::instance().SetValue<QString>("ui.default-directory.ImportInfrastructure", QFileInfo(filePath).absolutePath());
    }
}

void MainWindow::ImportInfrastructureResponse() {
    QMessageBox::information(nullptr, "Information", "Infrastructure imported");
}

void MainWindow::ExportInfrastructure() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("ui.default-directory.ExportInfrastructure", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir,
                                                              filter); !filePath.isEmpty()) {
        _moduleService->ExportInfrastructure(filePath);
        Configuration::instance().SetValue<QString>("ui.default-directory.ExportInfrastructure", QFileInfo(filePath).absolutePath());
    }
}

void MainWindow::WriteInfrastructureExport(const QString &filename, const QString &exportResponse) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Warning", "Couldn't open file for writing: " + file.fileName());
        return;
    }

    // Write formatted (pretty-printed) JSON
    file.write(StringUtils::ConvertToIndentedJson(exportResponse).toUtf8());
    file.close();
    QMessageBox::information(nullptr, "Information", "Infrastructure saved to file: " + file.fileName());
}

void MainWindow::CleanInfrastructure() const {
    _moduleService->CleanInfrastructure();
}

void MainWindow::CleanInfrastructureResponse() {
    QMessageBox::information(nullptr, "Information", "Infrastructure cleaned");
}

void MainWindow::ShowInfrastructureDialog() {
    ShowInfrastructure dialog;
    dialog.exec();
}

void MainWindow::FtpUpload() {

    // If the dialog doesn't exist, create it
    if (!_ftpClientDialog) {
        _ftpClientDialog = new FTPClientDialog(nullptr);
        _ftpClientDialog->setWindowFlags(Qt::Window);
        _ftpClientDialog->show();

        // Reset the pointer to nullptr when the user clicks 'X'
        connect(_ftpClientDialog, &QObject::destroyed, this, [this]() {
            _ftpClientDialog = nullptr;
        });
    } else {
        // If it already exists, bring it to the front
        _ftpClientDialog->show();
        _ftpClientDialog->raise();
        _ftpClientDialog->activateWindow();
    }
}

void MainWindow::DockerStats() {
    const auto dialog = new DockerStatsDialog(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::EditPreferences() {
    EditConfigDialog dialog;
    dialog.exec();
}

void MainWindow::UpdateStatusBar(const QString &text) const {
    if (_statusBar)
        _statusBar->showMessage(text);
}

void MainWindow::Exit() {
    QApplication::quit();
}
