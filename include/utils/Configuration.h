#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDir>

// Awsmock includes
#include <utils/JsonUtils.h>

#ifdef _WIN32
#define DEFAULT_CONFIGURATION_FILE_PATH QString("C:\\Program Files (x86)\\awsmock-qt-ui\\awsmock-qt-ui.json")
#else
#define DEFAULT_CONFIGURATION_FILE_PATH QString("/usr/local/awsmock-qt-ui/etc/awsmock-qt-ui.json")
#endif

class Configuration final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    Configuration() = default;

    /**
     * @brief Singleton instance
     *
     * @return
     */
    static Configuration &instance() {
        static Configuration instance;
        return instance;
    }

    template<class T>
    T GetValue(const QString &path, T defaultValue) {
        const QJsonValue v = JsonUtils::JsonValueByPath(_configurationRoot, path);
        if constexpr (std::is_same_v<T, int>) {
            return v.isDouble() ? static_cast<T>(v.toInt()) : defaultValue;
        } else if constexpr (std::is_same_v<T, long>) {
            return v.isDouble() ? static_cast<T>(v.toInteger()) : defaultValue;
        } else if constexpr (std::is_same_v<T, double>) {
            return v.isDouble() ? static_cast<T>(v.toDouble()) : defaultValue;
        } else if constexpr (std::is_same_v<T, QString>) {
            return v.isString() ? static_cast<T>(v.toString()) : defaultValue;
        } else if constexpr (std::is_same_v<T, bool>) {
            return v.isBool() ? static_cast<T>(v.toBool()) : defaultValue;
        } else if constexpr (std::is_same_v<T, QJsonObject>) {
            return v.isObject() ? static_cast<T>(v.toObject()) : defaultValue;
        } else if constexpr (std::is_same_v<T, QJsonArray>) {
            return v.isArray() ? static_cast<T>(v.toArray()) : defaultValue;
        } else {
            return defaultValue;
        }
    }

    template<class T>
    void SetValue(const QString &path, T value) {
        JsonUtils::setByPath(_configurationRoot, path, static_cast<T>(value));
        WriteConfigurationFile(filePath);
        emit ConfigurationChanged(path, value);
    }

    /**
     * @brief Write a JSON configuration file
     *
     * @param filePath absolute file path of the configuration file
     */
    void WriteConfigurationFile(const QString &filePath);

    /**
     * @brief Write a JSON configuration file
     *
     * @param filePath absolute file path of the configuration file
     */
    void ReadConfigurationFile(const QString &filePath);

    /**
     * @brief SetFilePath
     *
     * @param filePath absolute path to configuration file
     */
    void SetFilePath(const QString &filePath);

    /**
     * @brief Sets the connection state
     *
     * @param connected connection state.
     */
    void SetConnectionState(bool connected) { this->connected = connected; }

    /**
     * @brief Sets the default directory.
     *
     * @return connection state
     */
    [[nodiscard]] bool GetConnectionState() const { return this->connected; }

signals:
    /**
     * @brief Send when a preferences changed
     *
     * @param key preference key
     * @param value preference value
     */
    void ConfigurationChanged(const QString &key, const QString &value);

private:
    /**
     * @brief Configuration root
     */
    QJsonObject _configurationRoot{};

    /**
     * @brief File path
     */
    QString filePath = DEFAULT_CONFIGURATION_FILE_PATH;

    /**
     * @brief Connection flag
     */
    bool connected = true;
};

#endif // CONFIGURATION_H
