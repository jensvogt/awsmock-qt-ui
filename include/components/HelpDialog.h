//
// Created by jensv on 26/04/2026.
//

#pragma once

// Qt includes
#include <QDialog>
#include <qtextbrowser.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>

// Md4c includes
#include <utils/md4c/md4c-html.h>

namespace Awsmock::Components {

    class HelpDialog : public QDialog {
    public:
        explicit HelpDialog(const QString &filePath, QWidget *parent = nullptr) : QDialog(parent) {
            setWindowTitle("Documentation");
            resize(800, 600); // Bumped size slightly for tables

            // Keep your existing helpStyle CSS here...
            const QString helpStyle =
                    "body { font-family: 'Segoe UI', sans-serif; line-height: 1.5; color: white; padding: 15px; }"

                    // 1. Style the block container
                    "pre { "
                    "  background-color: #3c3f41; "
                    "  color: white; "
                    //                    "  padding: 10px; "
                    "  border: 1px solid #5c5c5c; "
                    "  border-radius: 4px; "
                    "}"

                    // 2. THE FIX: Stop the inner <code> from drawing its own background
                    "pre code { background-color: transparent; border: none; padding: 0; }"

                    // 3. Style inline code (code not inside a <pre>) so it still looks good
                    "code { background-color: #3c3f41; color: #cc7832; font-family: 'Consolas'; }"
                    "pre { margin: 0px; }" // Keep it 0 here; let the table handle the spacing
                    "p { margin-bottom: 8px; }"; // Control standard paragraph spacing;

            auto *layout = new QVBoxLayout(this);
            auto *browser = new QTextBrowser(this);
            browser->setOpenExternalLinks(true); // Useful for help docs
            browser->document()->setDefaultStyleSheet(helpStyle);
            browser->setStyleSheet("background-color: #2b2b2b; border: none;");

            const QString resourcePath = ":/help/docs/" + filePath;
            QFile file(resourcePath);

            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                const QString markdown = QTextStream(&file).readAll();

                // --- MD4C CONVERSION LOGIC ---
                std::string htmlBuffer;

                // Callback to append MD4C output to our string buffer
                auto processOutput = [](const MD_CHAR *str, MD_SIZE size, void *userData) {
                    static_cast<std::string *>(userData)->append(str, size);
                };

                // Parse with Github Dialect to support Tables and Task Lists
                if (const int result = md_html(markdown.toStdString().c_str(), static_cast<MD_SIZE>(markdown.size()), processOutput, &htmlBuffer,MD_DIALECT_GITHUB, 0); result == 0) {
                    // Wrap in basic HTML tags to ensure CSS is applied correctly
                    QString convertedHtml = QString::fromStdString(htmlBuffer);

                    // 2. Wrap <pre> in a table, but FORCE margins to zero
                    // We use style="margin: 0;" to kill the extra vertical space
                    convertedHtml.replace("<pre>",
                                          "<table width=\"100%\" bgcolor=\"#3c3f41\" cellpadding=\"10\" style=\"margin-top: 10px; margin-bottom: 10px;\">"
                                          "<tr><td>"
                                          "<pre style=\"margin: 0; white-space: pre-wrap;\">");

                    convertedHtml.replace("</pre>",
                                          "</pre></td></tr></table>");
                    browser->setHtml(QString("<html><body>%1</body></html>").arg(convertedHtml));

                } else {
                    browser->setPlainText("Error parsing markdown.");
                }
                // -----------------------------

            } else {
                browser->setPlainText("Documentation file not found.");
            }

            auto *closeButton = new QPushButton("Close", this);
            connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

            layout->addWidget(browser);
            layout->addWidget(closeButton);
        }
    };
}
