//
// Created by vogje01 on 2/21/26.
//

#pragma once

// Awsmock includes
#include <dto/s3/S3LambdaConfiguration.h>
#include <dto/s3/S3QueueConfiguration.h>
#include <dto/s3/S3TopicConfiguration.h>
#include <modules/application/ApplicationAddDialog.h>

struct S3PutBucketNotificationConfigurationRequest {

    QString region;

    QString bucket;

    QVector<S3LambdaConfiguration> lambdaConfigurations;

    QVector<S3QueueConfiguration> queueConfigurations;

    QVector<S3TopicConfiguration> topicConfigurations;

    [[nodiscard]]
    QString ToXml() const {
        QString xmlString;
        QXmlStreamWriter writer(&xmlString);

        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("NotificationConfiguration");

        if (!lambdaConfigurations.isEmpty()) {
            for (const auto &[id, lambdaArn, filterRules, events]: lambdaConfigurations) {
                writer.writeStartElement("CloudFunctionConfiguration");
                writer.writeTextElement("Id", id);
                writer.writeTextElement("CloudFunction", lambdaArn);

                // Filter rules
                if (!filterRules.isEmpty()) {
                    writer.writeStartElement("Filter");
                    writer.writeStartElement("S3Key");
                    writer.writeStartElement("FilterRule");
                    for (const auto &[name, filterValue]: filterRules) {
                        writer.writeTextElement("Name", S3FilterNameToString(name));
                        writer.writeTextElement("Value", filterValue);
                    }
                    writer.writeEndElement();
                    writer.writeEndElement();
                    writer.writeEndElement();
                }

                // Events
                for (const auto &event: events) {
                    writer.writeTextElement("Event", S3NotificationEventToString(event));
                }
                writer.writeEndElement();
            }
        }

        if (!queueConfigurations.isEmpty()) {
            for (const auto &[id, queueArn, filterRules, events]: queueConfigurations) {
                writer.writeStartElement("QueueConfiguration");
                writer.writeTextElement("Id", id);
                writer.writeTextElement("Queue", queueArn);

                // Filter rules
                if (!filterRules.isEmpty()) {
                    writer.writeStartElement("Filter");
                    writer.writeStartElement("S3Key");
                    writer.writeStartElement("FilterRule");
                    for (const auto &[name, filterValue]: filterRules) {
                        writer.writeTextElement("Name", S3FilterNameToString(name));
                        writer.writeTextElement("Value", filterValue);
                    }
                    writer.writeEndElement();
                    writer.writeEndElement();
                    writer.writeEndElement();
                }

                // Events
                for (const auto &event: events) {
                    writer.writeTextElement("Event", S3NotificationEventToString(event));
                }
                writer.writeEndElement();
            }
        }

        if (!topicConfigurations.isEmpty()) {
            for (const auto &[id, topicArn, filterRules, events]: topicConfigurations) {
                writer.writeStartElement("TopicConfiguration");
                writer.writeTextElement("Id", id);
                writer.writeTextElement("Topic", topicArn);

                // Filter rules
                if (!filterRules.isEmpty()) {
                    writer.writeStartElement("Filter");
                    writer.writeStartElement("S3Key");
                    writer.writeStartElement("FilterRule");
                    for (const auto &[name, filterValue]: filterRules) {
                        writer.writeTextElement("Name", S3FilterNameToString(name));
                        writer.writeTextElement("Value", filterValue);
                    }
                    writer.writeEndElement();
                    writer.writeEndElement();
                    writer.writeEndElement();
                }

                // Events
                for (const auto &event: events) {
                    writer.writeTextElement("Event", S3NotificationEventToString(event));
                }
                writer.writeEndElement();
            }
        }
        writer.writeEndDocument();
        return xmlString;
    }
};
