/*
 * Copyright (C) 2014-2015 Symeon Huang <hzwhuang@gmail.com>
 *
 * This file is part of the libQtShadowsocks.
 *
 * libQtShadowsocks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libQtShadowsocks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <signal.h>
#include <iostream>
#include "client.h"
#include "utils.h"
#include "qtservice.h"

using namespace QSS;

static void onSIGINT_TERM(int sig)
{
    if (sig == SIGINT || sig == SIGTERM) qApp->quit();
}

class ShadowsocksService : public QtService<QCoreApplication>
{
public:
    ShadowsocksService(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "ShadowsocksDaemon")
    {
        setServiceDescription("Shadowsocks Daemon");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }

protected:
    void start()
    {
        QCoreApplication *app = application();
        //app->setApplicationName("Shadowsocks-libQtShadowsocks");
        //app->setApplicationVersion(Common::version());

        client_ = new Client();

        /*QCommandLineParser parser;
        parser.addHelpOption();
        parser.addVersionOption();
        QCommandLineOption configFile("c",
                                      "specify config.json file.",
                                      "config_file",
                                      "config.json");
        QCommandLineOption serverAddress("s",
                                  "host name or IP address of your remote server.",
                                  "server_address");
        QCommandLineOption serverPort("p", "port number of your remote server.",
                                      "server_port");
        QCommandLineOption localAddress("b",
                                  "local address to bind. ignored in server mode.",
                                  "local_address",
                                  "127.0.0.1");
        QCommandLineOption localPort("l",
                      "port number of your local server. ignored in server mode.",
                      "local_port");
        QCommandLineOption password("k",
                                    "password of your remote server.",
                                    "password");
        QCommandLineOption encryptionMethod("m",
                                            "encryption method.",
                                            "method");
        QCommandLineOption timeout("t",
                                   "socket timeout in seconds.",
                                   "timeout");
        QCommandLineOption http(
                    QStringList() << "H" << "http-proxy",
                    "run in HTTP(S) proxy mode. ignored in server mode.");
        QCommandLineOption serverMode(
                    QStringList() << "S" << "server-mode",
                    "run as shadowsocks server.");
        QCommandLineOption testSpeed(
                    QStringList() << "T" << "speed-test",
                    "test encrypt/decrypt speed.");
        QCommandLineOption debug(
                    QStringList() << "D" << "debug",
                    "debug-level log.");
        QCommandLineOption autoBan("autoban",
                    "automatically ban IPs that send malformed header. "
                    "ignored in local mode.");
        QCommandLineOption logFile(
                    QStringList() << "f" << "logfile",
                    "put log messages into <file> (append if file exists).", "file");
        parser.addOption(configFile);
        parser.addOption(serverAddress);
        parser.addOption(serverPort);
        parser.addOption(localAddress);
        parser.addOption(localPort);
        parser.addOption(password);
        parser.addOption(encryptionMethod);
        parser.addOption(timeout);
        parser.addOption(http);
        parser.addOption(serverMode);
        parser.addOption(testSpeed);
        parser.addOption(debug);
        parser.addOption(autoBan);
        parser.addOption(logFile);
        parser.process(*app);

        Utils::debugEnabled = parser.isSet(debug);
        Utils::initLogFile(parser.value(logFile));

        if (!client_->readConfig(parser.value(configFile))) {
            client_->setup(parser.value(serverAddress),
                    parser.value(serverPort),
                    parser.value(localAddress),
                    parser.value(localPort),
                    parser.value(password),
                    parser.value(encryptionMethod),
                    parser.value(timeout),
                    parser.isSet(http));
        }
        client_->setAutoBan(parser.isSet(autoBan));

        //command-line option has a higher priority to make H, S, T consistent
        if (parser.isSet(http)) {
            client_->setHttpMode(true);
        }

        if (!client_->start(parser.isSet(serverMode))) {
            std::cerr << "Cannot start client" << std::endl;
        }*/

        if (!client_->readConfig("config.json")) {
            client_->setup("",
                    "",
                    "127.0.0.1",
                    "",
                    "",
                    "",
                    "",
                    false);
        }
        if (!client_->start(false)) {
            std::cerr << "Cannot start client" << std::endl;
        }
    }

    void pause()
    {
    }

    void resume()
    {
    }

private:
    Client *client_ = nullptr;
};


int main(int argc, char *argv[])
{
    qInstallMessageHandler(Utils::messageHandler);

    ShadowsocksService service(argc, argv);

    signal(SIGINT, onSIGINT_TERM);
    signal(SIGTERM, onSIGINT_TERM);

    return service.exec();
}
