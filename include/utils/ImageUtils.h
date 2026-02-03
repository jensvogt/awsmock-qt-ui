//
// Created by vogje01 on 11/6/25.
//

#ifndef AWSMOCK_QT_UI_IMAGE_UTILS_H
#define AWSMOCK_QT_UI_IMAGE_UTILS_H

// Qt includes
#include <QString>

class ImageUtils {
public:
    ImageUtils() = delete;

    static bool IsImageContentType(const QString &contentType);

};

#endif //AWSMOCK_QT_UI_IMAGE_UTILS_H
