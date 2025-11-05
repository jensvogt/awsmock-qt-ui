#ifndef EDITPREFERENCESDIALOG_H
#define EDITPREFERENCESDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIntValidator>

#include <utils/Configuration.h>

class EditPreferencesDialog : public QDialog {

    Q_OBJECT

public:
    explicit EditPreferencesDialog(QWidget *parent = nullptr);
    QString GetBaseUrl() const {return baseUrlEdit->text();};

signals:
    /**
     * @brief Send when a preferences changed
     *
     * @param key preference key
     * @param value preferance value
     */
    void PreferencesChanged(const QString &key, const QString &value);

private:
    QLineEdit *baseUrlEdit;
    QDialogButtonBox *buttonBox;
};

#endif // EDITPREFERENCESDIALOG_H
