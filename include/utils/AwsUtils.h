//
// Created by vogje01 on 2/17/26.
//

#ifndef AWSMOCK_QT_UI_AWSUTILS_H
#define AWSMOCK_QT_UI_AWSUTILS_H

// Qt includes
#include <QList>
#include <QString>

class AwsUtils {
public:
    static QString ArnToName(const QString &arn) {
        if (arn.isEmpty()) {
            return {};
        }
        return arn.split(":")[5];
    }
};
#endif //AWSMOCK_QT_UI_AWSUTILS_H
