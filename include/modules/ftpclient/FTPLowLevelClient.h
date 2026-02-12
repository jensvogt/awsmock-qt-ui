#ifndef AWSMOCK_QT_UI_LOW_LEVEL_CLIENT_H
#define AWSMOCK_QT_UI_LOW_LEVEL_CLIENT_H

// C++ common
#include <iostream>
#include <fstream>
#include <algorithm>

// Qt
#include <QString>

// Ftp client
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef int SOCKET;
#define SOCKADDR_IN struct sockaddr_in
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR   (-1)
#define Sleep(s) usleep(s * 1000)
#define closesocket(s) close(s)
#endif

// Awsmock in
#include <modules/ftpclient/FTPInfoThread.h>

constexpr int BUFLEN = 1000;
constexpr int DATABUFLEN = 1000;
const auto DELIMITER = "\r\n";

char *qstr2pch(const QString &src);

inline char *qstr2pch(const QString &src) {
    const std::string basic_string = src.toStdString();
    const auto pstr = new char[basic_string.length() + 1];
    strcpy(pstr, basic_string.c_str());
    return pstr;
}

inline QString pch2qstr(char *src) {
    return {QLatin1String(src)};
}

struct FileInfo {
    QString path{};
    QString name{};
    QString groupname{};
    QString username{};
    QString permissions{};
    QString timestamp{};
    QString contentType{};
    long size{};
};

/**
 * @brief Simple FTP client
 *
 * @see https://github.com/zycliao
 */
class Client {
    //int executeFTPCmd(int stateCode, char* cmd, char* arg=nullptr);
    int getStateCode() const;

    int getPortNum() const;

    int getFileSize(const std::string &fname);

    int listPwd();

    int intoPasv();

    int recvControl(int stateCode, std::string errorInfo = {});

    void executeCmd(std::string cmd) const;

    static void removeSpace(std::string &);

    void changeCurrentDir(const std::string &tardir);

    SOCKADDR_IN serverAddr{};
    std::string _ip_addr, _username, _password, INFO;
    char *buf = new char[BUFLEN];
    char *databuf = new char[DATABUFLEN];
    SOCKET controlSocket{};
    SOCKET dataSocket{};
    std::string recvInfo;
    std::string nextInfo;
    std::string currentDir{};

public:
    Client();

    ~Client();

    int connectServer();

    int disconnect();

    int changeDir(const std::string &tardir);

    int login(const QString &ip_addr, int port, const QString &username, const QString &password);

    int downFile(std::string remoteName, std::string localDir);

    int upFile(const std::string &localName);

    int deleteFile(const std::string &fname);

    int deleteDir(const std::string &dname);

    int rename(const std::string &src, const std::string &dst);

    int mkDir(const std::string &name);

    FTPInfoThread *infoThread;
    std::string pwd;
    std::vector<FileInfo> fileInfoList;
    int _port;
};

#endif // AWSMOCK_QT_UI_LOW_LEVEL_CLIENT_H
