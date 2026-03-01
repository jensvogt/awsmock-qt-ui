//
// Created by vogje01 on 2/20/26.
//

#ifndef AWSMOCK_QT_UI_REPLACE_WORD_DIALOG_H
#define AWSMOCK_QT_UI_REPLACE_WORD_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class ReplaceWordDialog;
}

QT_END_NAMESPACE

class ReplaceWordDialog : public QDialog {
    Q_OBJECT

public:
    explicit ReplaceWordDialog(QWidget *parent = nullptr);

    ~ReplaceWordDialog() override;

    void HandleAccept();

    void HandleReject();

    QString GetSearchText() {
        return _searchText;
    }

    QString GetReplacementText() {
        return _replacementText;
    }

private:
    /**
     * @brief UI components
     */
    Ui::ReplaceWordDialog *_ui;

    /**
     * @brief Search for field text
     */
    QString _searchText;

    /**
     * @brief Search for replacement text
     */
    QString _replacementText;
};


#endif // AWSMOCK_QT_UI_REPLACE_WORD_DIALOG_H
