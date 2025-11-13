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
#define DEFAULT_CONFIGURATION_FILE_PATH QString("/usr/local/awsmock/etc/awsmock-qt-ui.json")
#endif

class Configuration final {
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

    /**
     * @brief Returns the base URL
     *
     * @return base URL
     */
    QString GetBaseUrl() { return baseUrl; }

    /**
     * @brief Returns the region
     *
     * @return region
     */
    QString GetRegion() { return region; }

    /**
     * @brief Returns the auto update period
     *
     * @return auto update period
     */
    [[nodiscard]] int GetAUtoUpdatePeriod() const { return autoUpdatePeriod; }

    /**
     * @brief Write a JSON configuration file
     *
     * @param filePath absolute file path of the configuration file
     */
    void WriteConfigurationFile(const QString &filePath);

    /**
     * @brief Returns the default directory.
     *
     * @return default directory.
     */
    QString GetDefaultDirectory() {
        return defaultDirectory;
    }

    /**
     * @brief Sets the default directory.
     *
     * @param defaultDir
     */
    void SetDefaultDirectory(const QString &defaultDir) {
        this->defaultDirectory = defaultDir;
    }

    /**
     * @brief Sets the connection state
     *
     * @param connected connection state.
     */
    void SetConnectionState(bool connected) {
        this->connected = connected;
    }

    /**
     * @brief Sets the default directory.
     *
     * @return connection state
     */
    bool GetConnectionState() const {
        return this->connected;
    }

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
     * @brief File path
     */
    QString filePath = DEFAULT_CONFIGURATION_FILE_PATH;

    /**
     * @brief Base URL
     */
    QString region = "eu-central-1";

    /**
     * @brief Base URL
     */
    QString baseUrl = "http://localhost:4566";

    /**
     * @brief Current directory
     */
    QString defaultDirectory = QDir::homePath();

    /**
     * @brief Default auto update period in seconds.
     */
    int autoUpdatePeriod = 10;

    /**
     * @brief Connection flag
     */
    bool connected = true;
};

#endif // CONFIGURATION_H
