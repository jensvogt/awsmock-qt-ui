#include <mainwindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QCommandLineParser>
#include <qscreen.h>

#include <utils/Configuration.h>
#include <utils/IconUtils.h>

#define INITIAL_WIDTH 2000
#define INITIAL_HEIGHT 1200

int main(int argc, char *argv[]) {
    const QApplication app(argc, argv);

    // Set icon
    QApplication::setWindowIcon(IconUtils::GetCommonIcon("awsmock"));

    // set style
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // increase font size for better reading
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(defaultFont.pointSize());
    QApplication::setFont(defaultFont);

    // modify palette to dark
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::black);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Midlight, QColor(0, 0, 0));

    QApplication::setPalette(darkPalette);

    QCommandLineParser parser;
    parser.setApplicationDescription("AWS simulation");
    parser.addHelpOption();
    parser.addVersionOption();

    QTranslator translator;
    for (const QStringList uiLanguages = QLocale::system().uiLanguages(); const QString &locale: uiLanguages) {
        if (const QString baseName = "awsmock-qt-ui_" + QLocale(locale).name(); translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

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
