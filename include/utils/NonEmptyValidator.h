//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_NON_EMPTY_VALIDATOR_H
#define AWSMOCK_QT_UI_NON_EMPTY_VALIDATOR_H

#include <qvalidator.h>

class NotEmptyValidator final : public QValidator {
    Q_OBJECT

public:
    explicit NotEmptyValidator(QObject *parent = nullptr) : QValidator(parent) {
    }

    State validate(QString &input, int &pos) const override {
        Q_UNUSED(pos);

        // If the string is empty after trimming whitespace, it is invalid.
        if (input.trimmed().isEmpty()) {
            return Invalid;
        }
        // Otherwise, it is acceptable.
        return Acceptable;
    }
};

#endif //AWSMOCK_QT_UI_NON_EMPTY_VALIDATOR_H
