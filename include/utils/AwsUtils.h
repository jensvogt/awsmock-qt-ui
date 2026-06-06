//
// Created by vogje01 on 2/17/26.
//

#pragma once

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
