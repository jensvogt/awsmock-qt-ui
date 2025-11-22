//
// Created by vogje01 on 11/6/25.
//

#include <utils/IconUtils.h>

QIcon IconUtils::GetIcon(const QString &name) {
    const auto style = Configuration::instance().GetValue<QString>("ui.style-type", "");
    return GetIcon(style.toLower(), name);
}

QIcon IconUtils::GetIcon(const QString &type, const QString &name) {
    return QIcon(":/icons/" + type + "/" + name + ".png");
}

QIcon IconUtils::GetCommonIcon(const QString &name) {
#ifdef _WIN32
    return QIcon(":/icons/common/" + name + ".ico");
#else
    return QIcon(":/icons/common/" + name + ".png");
#endif
}
