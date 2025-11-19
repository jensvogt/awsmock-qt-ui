#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <QJsonDocument>
#include <QJsonObject>

class JsonUtils {
public:
    /**
     * @brief Write a JSON object to qDebug()
     *
     * @param obj JSON object
     */
    static void WriteJsonString(const QJsonObject &obj) {
        // Convert to JSON document
        const QJsonDocument doc(obj);

        // Convert to compact string
        const QString jsonString = QString::fromUtf8(doc.toJson(QJsonDocument::Indented));

        qDebug() << jsonString;
    }

    /**
     * @brief Return a json value by path
     *
     * @param root root JSON object
     * @param path path to value in dot notation, i.e.: a.b.c[2].d
     * @return JSON value
     */
    static QJsonValue JsonValueByPath(const QJsonValue &root, const QString &path) {
        QStringList tokens;
        QString token;
        bool inBracket = false;

        // --- Parse path: split into tokens like ["a","b","2","c"] ---
        for (int i = 0; i < path.length(); ++i) {
            QChar c = path[i];

            if (c == '.') {
                if (!inBracket) {
                    if (!token.isEmpty()) tokens << token;
                    token.clear();
                    continue;
                }
            }

            if (c == '[') {
                if (!token.isEmpty()) {
                    tokens << token;
                    token.clear();
                }
                inBracket = true;
                continue;
            }

            if (c == ']') {
                if (inBracket) {
                    if (!token.isEmpty()) tokens << token;
                    token.clear();
                    inBracket = false;
                }
                continue;
            }

            token.append(c);
        }

        if (!token.isEmpty())
            tokens << token;

        // --- Navigate JSON ---
        QJsonValue current = root;

        for (const QString &t: tokens) {

            if (current.isObject()) {
                QJsonObject obj = current.toObject();
                if (!obj.contains(t))
                    return {}; // key not found
                current = obj.value(t);
            } else if (current.isArray()) {
                bool ok = false;
                const int index = t.toInt(&ok);
                if (!ok)
                    return {}; // invalid array index

                QJsonArray arr = current.toArray();
                if (index < 0 || index >= arr.size())
                    return {}; // out of range

                current = arr[index];
            } else {
                return {}; // cannot navigate further
            }
        }

        return current;
    }

    struct PathElement {
        QString key;
        int index = -1; // -1 = no array index
    };

    // Split path like "a.b[2].c" into elements
    static QStringList splitPath(const QString &path) {
        QStringList parts;
        const QRegularExpression re(R"(\.?([^\.\[\]]+)(\[\d+\])?)");
        QRegularExpressionMatchIterator i = re.globalMatch(path);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString part = match.captured(1);
            if (QString index = match.captured(2); !index.isEmpty()) part += index;
            parts.append(part);
        }
        return parts;
    }

    // Recursive helper
    static void setByPath(QJsonObject &obj, const QStringList &pathParts, const QJsonValue &value) {
        if (pathParts.isEmpty()) return;

        const QString &key = pathParts.first();
        const QStringList remaining = pathParts.mid(1);

        // Match array index e.g. "b[2]"
        const QRegularExpression re(R"(^([a-zA-Z0-9_]+)\[(\d+)\]$)");

        if (const QRegularExpressionMatch match = re.match(key); match.hasMatch()) {
            const QString arrayKey = match.captured(1);
            const int idx = match.captured(2).toInt();

            QJsonArray arr = obj[arrayKey].toArray(); // copy

            // grow array if needed
            while (arr.size() <= idx)
                arr.append(QJsonValue());

            if (remaining.isEmpty()) {
                arr[idx] = value; // final assignment
            } else {
                QJsonObject nextObj = arr[idx].toObject(); // copy
                setByPath(nextObj, remaining, value);
                arr[idx] = nextObj; // assign modified object back
            }

            obj[arrayKey] = arr; // assign modified array back
        } else {
            // plain object key
            if (remaining.isEmpty()) {
                obj[key] = value; // final assignment
            } else {
                QJsonObject child = obj[key].toObject(); // copy
                setByPath(child, remaining, value);
                obj[key] = child; // assign modified object back
            }
        }
    }

    // Public interface
    static void setByPath(QJsonObject &obj, const QString &path, const QJsonValue &value) {
        setByPath(obj, splitPath(path), value);
    }
};

#endif // JSON_UTILS_H
