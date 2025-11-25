#include <mainwindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QCommandLineParser>
#include <QScreen>

#include <utils/Configuration.h>
#include <utils/IconUtils.h>

#define INITIAL_WIDTH 2000
#define INITIAL_HEIGHT 1200

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set icon
    QApplication::setWindowIcon(IconUtils::GetCommonIcon("awsmock"));

    // Translations
    QTranslator translator;
    for (const QStringList uiLanguages = QLocale::system().uiLanguages(); const QString &locale: uiLanguages) {
        if (const QString baseName = "awsmock-qt-ui_" + QLocale(locale).name(); translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    // Set separator style
    app.setStyleSheet("QMenu::separator { height: 2px; background: #7f7f7f; }");

    // Command line options
    QCommandLineParser parser;
    parser.setApplicationDescription("AWS simulation");
    parser.addHelpOption();
    parser.addVersionOption();
    const QCommandLineOption configOption(QStringList() << "c" << "config", "Path to config file.", "file");
    parser.addOption(configOption);

    // Process the actual command-line arguments
    parser.process(app);

    // Retrieve values
    if (const QString configPath = parser.value(configOption); !configPath.isEmpty()) {
        Configuration::instance().SetFilePath(configPath);
    } else {
        Configuration::instance().SetFilePath(DEFAULT_CONFIGURATION_FILE_PATH);
    }

    // set style
    app.setStyle(Configuration::instance().GetValue<QString>("ui.style", ""));
    if (Configuration::instance().GetValue<QString>("ui.style-type", "") == "Dark") {
        qApp->setStyle(QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
        if (QFile f(":/styles/styles/dark.qss"); f.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(f.readAll());
        }
    } else {
        qApp->setStyleSheet("");
        qApp->setStyle(QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
    }

    MainWindow w;
    constexpr int screenIndex = 0;
    if (const QList<QScreen *> screens = QGuiApplication::screens(); screenIndex < screens.count()) {
        const QScreen *targetScreen = screens.at(screenIndex);
        const QRect screenGeometry = targetScreen->geometry();

        // Center the window on that screen in global coordinates
        const int x = screenGeometry.x() + (screenGeometry.width() - w.width()) / 2;
        const int y = screenGeometry.y() + (screenGeometry.height() - w.height()) / 2;

        w.move(x, y);
    }
    w.show();
    return QApplication::exec();
}
