//
// Created by jensv on 19/04/2026.
//

#ifndef AWSMOCK_QT_UI_CONFIG_ADD_URL_DIALOG_H
#define AWSMOCK_QT_UI_CONFIG_ADD_URL_DIALOG_H

// QT includes
#include <QDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
    class ConfigAddUrlDialog;
}

QT_END_NAMESPACE

class ConfigAddUrlDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit ConfigAddUrlDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ConfigAddUrlDialog() override;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

    /**
     * @brief Returns the host name
     *
     * @return host name
     */
    QString GetHostname() {
        return _host;
    };

    /**
     * @brief Returns the prt
     *
     * @return port
     */
    int GetPort() const {
        return _port;
    };

private:
    /**
     * @brief UI components
     */
    Ui::ConfigAddUrlDialog *_ui;

    /**
     * @brief Host name
     */
    QString _host;

    /**
     * @brief Port
     */
    int _port{};
};


#endif // AWSMOCK_QT_UI_CONFIG_ADD_URL_DIALOG_H
