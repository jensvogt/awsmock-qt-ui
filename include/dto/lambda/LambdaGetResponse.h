#pragma once

#include <QJsonObject>
#include <QJsonDocument>

#include <dto/lambda/LambdaCounter.h>

struct LambdaGetResponse {
    QString region;

    QString id;

    QString lambdaName;

    QString lambdaArn;

    QString role;

    QString user;

    QString handler;

    QString runtime;

    QString version;

    int size{};

    int concurrency{};

    int instances{};

    int invocations{};

    int avgDuration{};

    bool enabled{};

    QString state;

    QString zipFile;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        id = jsonObject["id"].toString();
        lambdaName = jsonObject["functionName"].toString();
        lambdaArn = jsonObject["lambdaArn"].toString();
        role = jsonObject["role"].toString();
        user = jsonObject["user"].toString();
        handler = jsonObject["handler"].toString();
        runtime = jsonObject["runtime"].toString();
        version = jsonObject["version"].toString();
        size = jsonObject["size"].toInt();
        concurrency = jsonObject["concurrency"].toInt();
        instances = jsonObject["instances"].toInt();
        invocations = jsonObject["invocations"].toInt();
        avgDuration = jsonObject["avgDuration"].toInt();
        state = jsonObject["state"].toString();
        zipFile = jsonObject["zipFile"].toString();
        enabled = jsonObject["enabled"].toBool();
    }
};
