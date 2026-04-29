//
// Created by jensv on 26/04/2026.
//

#ifndef AWSMOCK_QT_UI_HELP_DIALOG_H
#define AWSMOCK_QT_UI_HELP_DIALOG_H

// Qt includes
#include <QDialog>
#include <qtextbrowser.h>
#include <QVBoxLayout>

#include "FTPFileTree.h"

namespace Awsmock::Components {

    class HelpDialog : public QDialog {
    public:
        explicit HelpDialog(const QString &filePath, QWidget *parent = nullptr) : QDialog(parent) {
            setWindowTitle("Documentation");
            resize(600, 400);

            auto *layout = new QVBoxLayout(this);
            auto *browser = new QTextBrowser(this);

            const QString resourcePath = ":/help/docs/" + filePath;
            QFile file(resourcePath);

            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                const QString content = in.readAll();

                // If using the HelpDialog logic we built:
                browser->setMarkdown(content);
            } else {
                qDebug() << "Failed to load resource:" << resourcePath;
            }

            auto *closeButton = new QPushButton("Close", this);
            connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

            layout->addWidget(browser);
            layout->addWidget(closeButton);
        }
    };
}

#endif // AWSMOCK_QT_UI_HELP_DIALOG_H
