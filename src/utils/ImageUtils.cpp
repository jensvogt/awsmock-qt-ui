//
// Created by vogje01 on 11/6/25.
//

#include <utils/ImageUtils.h>

bool ImageUtils::IsImageContentType(const QString &contentType) {
    return contentType.startsWith("image/");
}
