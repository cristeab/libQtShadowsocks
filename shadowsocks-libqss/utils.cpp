#include <QtShadowsocks>
#include <QDateTime>
#include <QTime>
#include <iostream>
#include "utils.h"

bool Utils::debugEnabled = false;
QTextStream Utils::textStream;
QFile Utils::logFile;
QMutex Utils::logMutex;
bool Utils::isValidStream = false;

void Utils::initLogFile(const QString& logFilename)
{
    if (logFilename.isEmpty()) {
        return;
    }
    logFile.setFileName(logFilename);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cerr << "Cannot open " << logFilename.toStdString() << " for writing" << std::endl;
        return;
    }
    textStream.setDevice(&logFile);
    isValidStream = true;
}

void Utils::testSpeed(const std::string &method, uint32_t data_size_mb)
{
    const std::string test(1024 * 32, '#');//32KB
    uint32_t loops = 32 * data_size_mb;
    QSS::Encryptor enc(method, "barfoo!");

    QTime startTime = QTime::currentTime();

    //encrypt per 1MB to reduce memory usage during the test
    for (uint32_t i = 0; i < loops; ++i) {
        enc.encrypt(test);
    }

    std::cout << "Encrypt Method      : " << method
              << "\nDatagram size       : " << data_size_mb << "MB\n"
              << "Time used to encrypt: "
              << startTime.msecsTo(QTime::currentTime()) << "ms\n" << std::endl;
}

void Utils::testSpeed(uint32_t data_size_mb)
{
    std::vector<std::string> allMethods = QSS::Cipher::supportedMethods();
    std::sort(allMethods.begin(), allMethods.end());
    for (const auto& method : allMethods) {
        testSpeed(method, data_size_mb);
    }
}

void Utils::messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QMutexLocker locker(&logMutex);
    const QString timestamp =
            QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    switch(type) {
    case QtDebugMsg:
        if (Utils::debugEnabled) {
            std::cout << timestamp.toStdString() << " DEBUG: " << msg.toStdString() << std::endl;
        }
        break;
    case QtInfoMsg:
        std::cout << timestamp.toStdString() << " INFO: " << msg.toStdString() << std::endl;
        break;
    case QtWarningMsg:
        std::cerr << timestamp.toStdString() << " WARN: " << msg.toStdString() << std::endl;
        break;
    case QtCriticalMsg:
        std::cerr << timestamp.toStdString() << " ERROR: " << msg.toStdString() << std::endl;
        break;
    case QtFatalMsg:
        std::cerr << timestamp.toStdString() << " FATAL: " << msg.toStdString() << std::endl;
        abort();
    }

    if (isValidStream) {
        switch(type) {
        case QtDebugMsg:
            if (Utils::debugEnabled) {
                textStream << timestamp << " DEBUG: " << msg;
                endl(textStream);
            }
            break;
        case QtInfoMsg:
            textStream << timestamp << " INFO: " << msg;
            endl(textStream);
            break;
        case QtWarningMsg:
            textStream << timestamp << " WARN: " << msg;
            endl(textStream);
            break;
        case QtCriticalMsg:
            textStream << timestamp << " ERROR: " << msg;
            endl(textStream);
            break;
        }
    }
}
