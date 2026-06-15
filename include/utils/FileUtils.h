//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_FILE_UTILS_H
#define AWSMOCK_QT_UI_FILE_UTILS_H

#include <filesystem>
#include <type_traits>

#include <QString>

#include "dto/dashboard/DashboardCounterResult.h"

class FileUtils final {

public:
    FileUtils();

    ~FileUtils();

    static QString ExtractVersionFromFileName(const QString &filename) {
        const QRegularExpression versionRegex(R"(^.*-([0-9]+\.[0-9]+\.[0-9]+)(-SNAPSHOT(-aws)?)?\.(tar\.gz|jar|zip|tar|tgz)$)");

        if (const QRegularExpressionMatch match = versionRegex.match(filename); match.hasMatch()) {
            return match.captured(1);
        }
        return "latest";
    }

    /**
     * @brief Append the different file path parts using the correct separator for Windows, Linux and macOS
     *
     * @tparam Ts type
     * @param args variable list of arguments
     * @return file path with correct separator
     */
    template<typename... Ts>
    static QString appendPath(Ts &&... args) {
        std::filesystem::path result;
        ((result /= toPathComponent(std::forward<Ts>(args))), ...);
        return QString::fromStdString(result.string());
    }

private:
    template<typename T>
    static auto toPathComponent(T &&arg) {
        if constexpr (std::is_same_v<std::decay_t<T>, QString>) {
            return arg.toStdString();
        } else {
            return std::forward<T>(arg);
        }
    }
};
#endif //AWSMOCK_QT_UI_FILE_UTILS_H
