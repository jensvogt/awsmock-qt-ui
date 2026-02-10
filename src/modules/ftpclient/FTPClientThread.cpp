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
    const size_t num = curClient->fileInfoList.size();

    QString type, size, name;
    std::vector<std::string> eachrow;

    // First directories
    for (int i = 0; i < num; i++) {
        if (FileInfo fileInfo = curClient->fileInfoList[i]; fileInfo.type == "directory") {
            emit emitFileListItem(fileInfo);
        }
    }

    // Then files
    for (int i = 0; i < num; i++) {
        if (FileInfo fileInfo = curClient->fileInfoList[i]; fileInfo.type != "directory") {
            emit emitFileListItem(fileInfo);
        }
    }
}

