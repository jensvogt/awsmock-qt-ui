/**
 * AwsMock Qt UI. A rich client interface to the AwsMock AWS somilation.
 * Copyright (C) 2015-2016 Dr. Jens Vogt (jensvogt@opitz-consulting.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <mainwindow.h>

// Qt includes
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QCommandLineParser>
#include <QScreen>

// AwsMock includes
#include <utils/Configuration.h>
#include <utils/IconUtils.h>
#include <utils/Logging.h>

#define INITIAL_WIDTH 2000
#define INITIAL_HEIGHT 1200

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    // Install the redirection
    qInstallMessageHandler(myCustomMessageHandler);

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

    // Set style
    app.setStyle(Configuration::instance().GetValue<QString>("ui.style", ""));
    if (Configuration::instance().GetValue<QString>("ui.style-type", "") == "Dark") {
        qApp->setStyle(QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
        if (QFile f(":/styles/styles/dark.qss"); f.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(f.readAll());
        }
    } else {
        qApp->setStyle(QStyleFactory::create(Configuration::instance().GetValue<QString>("ui.style", "")));
        if (QFile f(":/styles/styles/light.qss"); f.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(f.readAll());
        }
    }

    MainWindow w;
    w.resize(INITIAL_WIDTH, INITIAL_HEIGHT);
    w.setWindowTitle("AwsMock UI v" + QString(APP_VERSION));
    constexpr int screenIndex = 1;

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
