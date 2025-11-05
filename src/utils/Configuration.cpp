
#include <utils/Configuration.h>

#include "utils/JsonUtils.h"

void Configuration::SetFilePath(const QString &filePath) {
    this->filePath = filePath;
    ReadConfigurationFile(filePath);
}

void Configuration::ReadConfigurationFile(const QString &filePath) {

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open config file:" << file.errorString();
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format: root is not an object";
        return;
    }

    region = doc.object()["Region"].toString();
    baseUrl = doc.object()["BaseUrl"].toString();
}

void Configuration::WriteConfigurationFile(const QString &filePath){

    if(!filePath.isEmpty()) {
        this->filePath = filePath;
    }

    // Create your JSON object
    QJsonObject configurationObject;
    configurationObject["BaseUrl"] = baseUrl;

    // Wrap it in a QJsonDocument
    QJsonDocument doc(configurationObject);

    // Open the file for writing
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open file for writing:" << file.errorString();
        return;
    }

    // Write formatted (pretty-printed) JSON
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "JSON written to" << filePath;
}
