
#include <utils/EditPreferencesDialog.h>


EditPreferencesDialog::EditPreferencesDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Preferences");
    setMinimumSize(400, 200);

    baseUrlEdit = new QLineEdit(this);
    baseUrlEdit->setText(Configuration::instance().GetBaseUrl());

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("BaseURL:", baseUrlEdit);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EditPreferencesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EditPreferencesDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}
