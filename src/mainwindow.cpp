#include <mainwindow.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    // Connect infrastructure signals
    _moduleService = new ModuleService();
    connect(_moduleService, &ModuleService::ImportResponseSignal, this, &ImportInfrastructureResponse);
    connect(_moduleService, &ModuleService::ExportResponseSignal, this, &WriteInfrastructureExport);
    connect(_moduleService, &ModuleService::CleanResponseSignal, this, &CleanInfrastructureResponse);

    setWindowTitle("AwsMock UI v" + QString(APP_VERSION));
    resize(1600, 900);

    // Setup menu bar
    SetupMenuBar();

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
}

MainWindow::~MainWindow() = default;

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
    const auto uploadAction = new QAction(IconUtils::GetIcon("upload"), tr("&Upload file"), this);
    connect(uploadAction, &QAction::triggered, this, &MainWindow::FtpUpload);
    toolsMenu->addAction(uploadAction);

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
}

void MainWindow::ImportInfrastructure() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("_ui.default-directory.ImportInfrastructure", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON Configuration File", defaultDir, filter); !filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Error", "Could not open file:" + filePath);
            return;
        }

        const QByteArray jsonData = file.readAll();
        file.close();

        _moduleService->ImportInfrastructure(jsonData);
        Configuration::instance().SetValue<QString>("_ui.default-directory.ImportInfrastructure", QFileInfo(filePath).absolutePath());
    }
}

void MainWindow::ImportInfrastructureResponse() {
    QMessageBox::information(nullptr, "Information", "Infrastructure imported");
}

void MainWindow::ExportInfrastructure() const {
    // Create a QFileDialog set to select existing files
    const auto filter = "JSON Files (*.json);;All Files (*.*)";
    const auto defaultDir = Configuration::instance().GetValue<QString>("_ui.default-directory.ExportInfrastructure", "/usr/local/awsmock-qt-_ui");

    if (const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open JSON Configuration File", defaultDir,
                                                              filter); !filePath.isEmpty()) {
        _moduleService->ExportInfrastructure(filePath);
        Configuration::instance().SetValue<QString>("_ui.default-directory.ExportInfrastructure", QFileInfo(filePath).absolutePath());
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
    const auto dialog = new FTPUploadDialog(this);
    dialog->setModal(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
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
