//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_FILE_UTILS_H
#define AWSMOCK_QT_UI_FILE_UTILS_H

#include <QString>

#include "dto/dashboard/DashboardCounterResult.h"

class FileUtils final {

public:
    FileUtils();

    ~FileUtils();

    static QString ExtractVersionFromFileName(const QString &filename) {
        const QRegularExpression versionRegex(R"(^.*-([0-9]+\.[0-9]+\.[0-9]+)(-SNAPSHOT(-aws)?)?\.(tar\.gz|jar|zip|tar|tgz)$)");

        if (const QRegularExpressionMatch match = versionRegex.match(filename); match.hasMatch()) {
            return match.captured(1);
        }
        return "latest";
    }
};
#endif //AWSMOCK_QT_UI_FILE_UTILS_H
