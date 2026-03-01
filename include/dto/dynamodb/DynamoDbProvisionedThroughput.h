#ifndef AWSMOCK_QT_UI_DYNAMODB_PROVISIONED_THROUGHPUT_H
#define AWSMOCK_QT_UI_DYNAMODB_PROVISIONED_THROUGHPUT_H

// Qt includes
#include <QJsonObject>

struct DynamoDbProvisionedThroughput {

    int readCapacity{};
    int writeCapacity{};
    long lastDecreaseDateTime;
    long lastIncreaseDateTime;
    long numberOfDecreasesToday;

    void FromJsonObject(const QJsonObject &jsonObject) {
        readCapacity = jsonObject["ReadCapacityUnits"].toInt();
        writeCapacity = jsonObject["WriteCapacityUnits"].toInt();
        lastDecreaseDateTime = jsonObject["LastDecreaseDateTime"].toInteger();
        lastIncreaseDateTime = jsonObject["LastIncreaseDateTime"].toInteger();
        numberOfDecreasesToday = jsonObject["NumberOfDecreasesToday"].toInteger();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject result;
        result["ReadCapacityUnits"] = readCapacity;
        result["WriteCapacityUnits"] = writeCapacity;
        result["LastDecreaseDateTime"] = static_cast<qint64>(lastDecreaseDateTime);
        result["LastIncreaseDateTime"] = static_cast<qint64>(lastIncreaseDateTime);
        result["NumberOfDecreasesToday"] = static_cast<qint64>(numberOfDecreasesToday);
        return result;
    }
};

#endif // AWSMOCK_QT_UI_DYNAMODB_PROVISIONED_THROUGHPUT_H
