#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>

#ifdef _WIN32
#define DEFAULT_CONFIGURAITON_FILE_PATH QString("C:\\Program Files (x86)\\awsmock-qt-ui\\etc\\awsmock-qt-ui.json")
#else
#define DEFAULT_CONFIGURAITON_FILE_PATH QString("/usr/local/awsmock-qt-ui/etc/awsmock-qt-ui.json")
#endif

class Configuration : public QObject {
    Q_OBJECT

public:

    /**
     * @brief Constructor
     */
    Configuration()=default;

    static Configuration& instance() {
        static Configuration instance;
        return instance;
    }

    /**
     * @brief Returns the base URL
     *
     * @return base URL
     */
    QString GetBaseUrl() {return baseUrl;}

    /**
     * @brief Returns the region
     *
     * @return region
     */
    QString GetRegion() {return region;}

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
     * @param filePath absulote path to configuration file
     */
    void SetFilePath(const QString &filePath);

signals:

    /**
     * @brief Send when a preferences changed
     *
     * @param key preference key
     * @param value preferance value
     */
    void ConfigurationChanged(const QString &key, const QString &value);

private:

    /**
     * @brief File path
     */
    QString filePath = DEFAULT_CONFIGURAITON_FILE_PATH;

    /**
     * @brief Base URL
     */
    QString region = "eu-central-1";

    /**
     * @brief Base URL
     */
    QString baseUrl = "http://localhost:4566";
};

#endif // CONFIGURATION_H
