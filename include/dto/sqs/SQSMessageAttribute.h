#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_H

// C++ standard includes
#include <string>
#include <vector>

#include <dto/sqs/SQSMessageAttributeDataType.h>

struct MessageAttribute final {
    /**
     * Message attribute string value
     */
    std::string stringValue = {};

    /**
     * Message attribute number value
     */
    std::vector<std::string> stringListValues = {};

    /**
     * Message attribute binary value
     */
    unsigned char *binaryValue = nullptr;

    /**
     * Logical data type
     */
    AwsMock::Dto::SQS::MessageAttributeDataType dataType{};
};

#endif// AWSMOCK_QT_UI_SQS_MESSAGE_ATTRIBUTE_H
