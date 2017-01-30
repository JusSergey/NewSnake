#include "TcpClient.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <err.h>
#include <fcntl.h>

TcpClient::TcpClient(const std::string &ip, u_short port) :
    TcpSocket(ip, port)
{

    int is = connect(fd, (sockaddr*)&sock, sizeof(sock));

    if (is < 0)
        err(is, "connect()");

    unlockFD(fd);

    initThread();
}

TcpClient::~TcpClient()
{

}

void TcpClient::loop()
{
    if (timeReceiver > toMsec(UpdateRecver)) {
        loopReceiver();
        timeReceiver = 0;
    }
    if (timeSender > toMsec(UpdateSender)) {
        loopSender();
        timeSender = 0;
    }
}

void TcpClient::loopReceiver()
{
    if (recv(fd, buffer, 1024, MSG_NOSIGNAL) > 0)
        read();
}

void TcpClient::loopSender()
{
    send(fd, msgToSend.c_str(), msgToSend.size()+1, MSG_NOSIGNAL);
}

std::string TcpClient::getMsgToSend() const
{
    return msgToSend;
}

void TcpClient::setMsgToSend(const std::string &value)
{
    msgToSend = value;
}
