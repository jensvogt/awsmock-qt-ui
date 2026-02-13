
// Qt includes
#include <QHostInfo>
#include <QRegularExpression>

// Awsmock includes
#include <utils/Logging.h>
#include <modules/ftpclient/FTPLowLevelClient.h>

Client::Client() {
    infoThread = new FTPInfoThread;
}

Client::~Client() {
    closesocket(dataSocket);
    closesocket(controlSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    delete infoThread;
    delete[] buf;
    delete[] databuf;
}

int Client::login(const QString &ip_addr, const int port, const QString &username, const QString &password) {
    _username = username.toStdString();
    _password = password.toStdString();
    _port = port;
    if (const QRegularExpression regex(R"(^(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}$)"); regex.match(ip_addr).hasMatch()) {
        _ip_addr = ip_addr.toStdString();
    } else {
        if (const QHostInfo hostInfo = QHostInfo::fromName(ip_addr); hostInfo.error() == QHostInfo::NoError) {
            for (const QHostAddress &addr: hostInfo.addresses()) {
                _ip_addr = addr.toString().toStdString();
            }
        } else {
            logError << "DNS error:" << hostInfo.errorString();
        }
    }
    return 0;
}

int Client::connectServer() {
    int ret;

#ifdef _WIN32
    WSADATA dat;
    // Initialization is very important.
    if (WSAStartup(MAKEWORD(2, 2), &dat) != 0) //Windows Sockets Asynchronous启动
    {
        FTPInfoThread::instance().sendInfo("Init failed!");
        return -1;
    }
#endif

    // Create a Socket
    controlSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    if (controlSocket == INVALID_SOCKET) {
        FTPInfoThread::instance().sendInfo("Creating control socket failed.");
        return -1;
    }
    // Construct server access parameter structure
    serverAddr.sin_family = AF_INET;
#ifdef _WIN32
    serverAddr.sin_addr.S_un.S_addr = inet_addr(_ip_addr.c_str()); //address
#else
    serverAddr.sin_addr.s_addr = inet_addr(_ip_addr.c_str()); //address
#endif
    serverAddr.sin_port = htons(_port); // port
    memset(serverAddr.sin_zero, 0, sizeof(serverAddr.sin_zero));

    // Connect
    ret = connect(controlSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    if (ret == SOCKET_ERROR) {
        FTPInfoThread::instance().sendInfo("Control socket connection failed");
        return -1;
    }

    // Receive return status information
    Sleep(300);
    recvControl(220);

    // Username
    executeCmd("USER " + _username);
    recvControl(331);
    //executeFTPCmd(331, "USER", qstr2pch(username));                //331

    // Password
    executeCmd("PASS " + _password);
    recvControl(230);
    //executeFTPCmd(230, "PASS", qstr2pch(password));            //230

    listPwd();
    return 0;
}

int Client::disconnect() {
    executeCmd("QUIT");
    recvControl(221);
    _ip_addr, _username, _password, INFO = "";
    fileInfoList.clear();
    memset(buf, 0, BUFLEN);
    memset(databuf, 0, DATABUFLEN);
    closesocket(dataSocket);
    closesocket(controlSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

int Client::changeDir(const std::string &tardir) {
    memset(buf, 0, BUFLEN);
    executeCmd("CWD " + tardir);
    recvControl(250);
    intoPasv();
    changeCurrentDir(tardir);
    listPwd();
    return 0;
}

int Client::downFile(std::string remoteName, std::string localDir) {
    std::string localFile = localDir + "/" + remoteName;
    std::ofstream ofile;
    ofile.open(localFile, std::ios_base::binary);
    intoPasv();
    getFileSize(remoteName);
    executeCmd("RETR " + remoteName);
    recvControl(150);
    memset(databuf, 0, DATABUFLEN);
    int ret = recv(dataSocket, databuf, DATABUFLEN, 0);
    while (ret > 0) {
        ofile.write(databuf, ret);
        ret = recv(dataSocket, databuf, DATABUFLEN, 0);
    }
    ofile.close();
    closesocket(dataSocket);
    recvControl(226);
    return 0;
}

//private function---------------------------------------------------------
void Client::executeCmd(std::string cmd) const {
    if (cmd.substr(0, 4) == "PASS") {
        FTPInfoThread::instance().sendInfo("PASS ********");
    } else {
        FTPInfoThread::instance().sendInfo(cmd);
    }
    cmd += "\r\n";
    const int n = static_cast<int>(cmd.size());
    send(controlSocket, cmd.c_str(), n, 0);
}

int Client::recvControl(const int stateCode, std::string errorInfo) {
    if (errorInfo.size() == 1)
        errorInfo = "state code error!";
    if (nextInfo.empty()) {
        Sleep(50);
        memset(buf, 0, BUFLEN);
        recvInfo.clear();
        const size_t ssize = recv(controlSocket, buf, BUFLEN, 0);
        if (ssize == BUFLEN) {
            FTPInfoThread::instance().sendInfo("ERROR! Too long information too receive!");
            return -1;
        }
        buf[ssize] = '\0';
        const int t = getStateCode();
        recvInfo = buf;

        // JUNK
        if (const int temp = recvInfo.find("\r\n226"); temp >= 0) {
            nextInfo = recvInfo.substr(temp + 2);
        }
        // \JUNK
        FTPInfoThread::instance().sendInfo(recvInfo);
        if (t == stateCode)
            return 0;

        FTPInfoThread::instance().sendInfo(errorInfo);
        return -1;
    }
    recvInfo = nextInfo;
    nextInfo.clear();
    return 0;
}

// Retrieve the status code from the returned information.
int Client::getStateCode() const {
    int num = 0;
    char *p = buf;
    while (p != nullptr) {
        num = 10 * num + (*p) - '0';
        p++;
        if (*p == ' ' || *p == '-') {
            break;
        }
    }
    return num;
}

// From the returned message "227 Entering Passive Mode (182,18,8,37,10,25)". Data acquisition port
int Client::getPortNum() const {
    int num1 = 0, num2 = 0;

    char *p = buf;
    int cnt = 0;
    while (1) {
        if (cnt == 4 && (*p) != ',') {
            if (*p <= '9' && *p >= '0')
                num1 = 10 * num1 + (*p) - '0';
        }
        if (cnt == 5) {
            if (*p <= '9' && *p >= '0')
                num2 = 10 * num2 + (*p) - '0';
        }
        if ((*p) == ',') {
            cnt++;
        }
        p++;
        if ((*p) == ')') {
            break;
        }
    }
    return num1 * 256 + num2;
}

int Client::listPwd() {
    intoPasv();
    executeCmd("LIST -al");
    recvControl(150);
    memset(databuf, 0, DATABUFLEN);

    std::string fulllist;
    int ret = recv(dataSocket, databuf, DATABUFLEN - 1, 0);
    while (ret > 0) {
        databuf[ret] = '\0';
        fulllist += databuf;
        ret = recv(dataSocket, databuf, DATABUFLEN - 1, 0);
    }
    removeSpace(fulllist);

    fileInfoList.clear();
    std::vector<std::string> eachrow;

    FileInfo fileInfo;
    if (!currentDir.empty()) {
        fileInfo.name = "..";
        fileInfo.contentType = "folder";
        fileInfoList.push_back(fileInfo);
    }
    std::string item;
    int p = fulllist.find("\r\n");
    int lastp = 0;
    while (p >= 0) {
        eachrow.clear();
        std::string rawrow = fulllist.substr(lastp, p - lastp);

        int q = rawrow.find(' ');
        int lastq = 0;
        fileInfo.timestamp = {};
        for (int i = 0; i < 8; i++) {
            item = rawrow.substr(lastq, q - lastq);
            if (i == 0) {
                fileInfo.permissions = QString::fromStdString(item);
                if (fileInfo.permissions.startsWith("-")) {
                    fileInfo.contentType = "file";
                } else if (fileInfo.permissions.startsWith("d")) {
                    fileInfo.contentType = "folder";
                } else if (fileInfo.permissions.startsWith("l")) {
                    fileInfo.contentType = "link";
                } else if (fileInfo.permissions.startsWith("s")) {
                    fileInfo.contentType = "socket";
                } else if (fileInfo.permissions.startsWith("c")) {
                    fileInfo.contentType = "character device";
                } else if (fileInfo.permissions.startsWith("b")) {
                    fileInfo.contentType = "block device";
                } else if (fileInfo.permissions.startsWith("p")) {
                    fileInfo.contentType = "named pipe";
                }
            } else if (i == 2) {
                fileInfo.username = QString::fromStdString(item);
            } else if (i == 3) {
                fileInfo.groupname = QString::fromStdString(item);
            } else if (i == 4) {
                fileInfo.size = stol(item);
            } else if (i == 5 || i == 6 || i == 7) {
                fileInfo.timestamp += QString::fromStdString(item) + " ";
            }
            eachrow.push_back(item);
            lastq = q + 1;
            q = rawrow.find(' ', lastq);
        }
        item = rawrow.substr(lastq);
        eachrow.push_back(item);
        fileInfo.name = QString::fromStdString(item);
        fileInfo.timestamp = fileInfo.timestamp.trimmed();
        fileInfoList.emplace_back(fileInfo);

        lastp = p + 2;
        p = fulllist.find("\r\n", lastp);
    }
    closesocket(dataSocket);
    recvControl(226);
    return 0;
}

int Client::intoPasv() {
    //Switch to passive mode
    executeCmd("PASV");
    recvControl(227);
    //executeFTPCmd(227, "PASV");                //227

    // The returned information is in the format of h1,h2,h3,h4,p1,p2
    // where h1,h2,h3,h4 are the server addresses, and p1*256+p2 is the data port.
    const int dataPort = getPortNum();

    // Client data transmission socket
    dataSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    // Change the port value in the connection parameters
    serverAddr.sin_port = htons(dataPort);
    int ret = connect(dataSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    if (ret == SOCKET_ERROR) {
        return -1;
    }
    return 0;
}

int Client::getFileSize(const std::string &fname) {
    executeCmd("SIZE " + fname);
    recvControl(213);
    const char *p = buf;
    while (p != nullptr && *p != ' ') {
        p++;
    }
    p++;
    int num = 0;
    while (p != nullptr && *p != '\r') {
        num *= 10;
        num += (*p - '0');
        p++;
    }
    memset(buf, 0, BUFLEN);
    return num;
}

int Client::upFile(const std::string &localName) {
    // TODO:change to C++ style
    FILE *ifile = fopen(localName.c_str(), "rb");
    if (!ifile) {
        FTPInfoThread::instance().sendInfo("fail to open the file!");
        return -1;
    }

    //get file name
    const int p = static_cast<int>(localName.find_last_of("/"));
    const std::string localFileName = localName.substr(p + 1);

    intoPasv();
    executeCmd("STOR " + localFileName);
    recvControl(150, "Permission denied.");
    size_t total = 0;
    while (!feof(ifile)) {
        const size_t count = fread(databuf, 1, DATABUFLEN, ifile);
        send(dataSocket, databuf, count, 0);
        total += count;
    }
    FTPInfoThread::instance().sendInfo("File send: " + localName + " " + std::to_string(total) + " bytes");
    fclose(ifile);
    closesocket(dataSocket);
    recvControl(226);
    listPwd();
    return 0;
}

void Client::removeSpace(std::string &src) {
    int q;
    int p = static_cast<int>(src.find(' '));
    while (p >= 0) {
        for (q = p + 1; src[q] == ' '; q++) {
        }
        src.erase(p + 1, q - p - 1);
        p = static_cast<int>(src.find(' ', p + 1));
    }
}

int Client::deleteFile(const std::string &fname) {
    executeCmd("DELE " + fname);
    recvControl(250);
    listPwd();
    return 0;
}

int Client::deleteDir(const std::string &dname) {
    executeCmd("RMD " + dname);
    recvControl(250);
    listPwd();
    return 0;
}

int Client::rename(const std::string &src, const std::string &dst) {
    executeCmd("RNFR " + src);
    recvControl(350);
    executeCmd("RNTO " + dst);
    recvControl(250);
    listPwd();
    return 0;
}

int Client::mkDir(const std::string &name) {
    executeCmd("MKD " + name);
    recvControl(250);
    listPwd();
    return 0;
}

void Client::changeCurrentDir(const std::string &tardir) {
    if (tardir == "..") {
        currentDir = currentDir.substr(0, currentDir.find_last_of("/"));
    } else if (tardir == "/") {
        currentDir = "";
    } else {
        currentDir += "/" + tardir;
    }
}
