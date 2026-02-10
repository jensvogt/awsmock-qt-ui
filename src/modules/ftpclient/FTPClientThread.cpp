#include <modules/ftpclient/FTPClientThread.h>

FTPClientThread::FTPClientThread() : task(), client(nullptr) {
    for (int i = 0; i < 5; i++)
        arglist.push_back("");
    curClient = new Client();
}

FTPClientThread::~FTPClientThread() {
    delete curClient;
}

void FTPClientThread::run() {
    switch (task) {
        case TConnect:
            if (!curClient->connectServer())
                emit emitSuccess();
            flushList();
            break;
        case TDisconnect:
            curClient->disconnect();
            break;
        case TCd:
            curClient->changeDir(arglist[0]);
            flushList();
            break;
        case TDown:
            curClient->downFile(arglist[0], arglist[1]);
            break;
        case TUp:
            curClient->upFile(arglist[0]);
            flushList();
            break;
        case TDele:
            curClient->deleteFile(arglist[0]);
            flushList();
            break;
        case TRmd:
            curClient->deleteDir(arglist[0]);
            flushList();
            break;
        case TRename:
            curClient->rename(arglist[0], arglist[1]);
            flushList();
            break;
        case TMkd:
            curClient->mkDir(arglist[0]);
            flushList();
            break;
        default:
            break;
    }
}

void FTPClientThread::stop() {
    isInterruptionRequested();
    quit();
    wait();
}

void FTPClientThread::flushList() {
    emit emitClearList();
    const int num = curClient->filelist.size();
    QString type, size, name;
    std::vector<std::string> eachrow;
    for (int i = 0; i < num; i++) {
        eachrow = curClient->filelist[i];
        if (eachrow[8] == "..") {
            emit emitListItem("d", "", "..");
            continue;
        }
        type = QString::fromStdString(eachrow[0].substr(0, 1));
        if (type == "-")
            continue;
        size = QString::fromStdString(eachrow[4]);
        name = QString::fromStdString(eachrow[8]);
        emit emitListItem(type, size, name);
    }
    for (int i = 0; i < num; i++) {
        eachrow = curClient->filelist[i];
        if (eachrow[8] == "..") {
            continue;
        }
        type = QString::fromStdString(eachrow[0].substr(0, 1));
        if (type == "d")
            continue;
        size = QString::fromStdString(eachrow[4]);
        name = QString::fromStdString(eachrow[8]);
        emit emitListItem(type, size, name);
    }
}

