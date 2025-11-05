#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <QJsonDocument>
#include <QJsonObject>

class JsonUtils
{
public:
    static void WriteJsonString(const QJsonObject &obj)
    {
        // Convert to JSON document
        QJsonDocument doc(obj);

        // Convert to compact string
        QString jsonString = QString::fromUtf8(doc.toJson(QJsonDocument::Indented));

        qDebug() << jsonString;
    }
};

#endif // JSON_UTILS_H
