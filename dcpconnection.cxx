#include "dcpconnection.h"

#include <stdio.h> // fprintf, stderr
#include <netinet/in.h> // htons

#define BUF_MAX_SIZE 65536

DCPConnection::DCPConnection(QObject *parent) :
    QObject(parent)
{
    sock = new QTcpSocket(this);

    bufsize = nextsize = 0;
    buffer = new char[BUF_MAX_SIZE];
    initialMsg = NULL;

    connect(sock, SIGNAL(readyRead()), this, SLOT(dataReady()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
    connect(sock, SIGNAL(connected()), this, SLOT(connected()));
}

void DCPConnection::connectTo(QString server, QString handle, QString passphrase, QString client)
{
    sock->connectToHost(server, 7266);

    QMultiHash<QString, QString> signon;
    signon.insert("handle", handle);
    signon.insert("password", passphrase);
    signon.insert("client-name", client);
    signon.insert("client-ver", "Gilligan");
    signon.insert("options", "*");
    initialMsg = new DCPMessage("*", "*", "SIGNON", signon);
}

void DCPConnection::disconnectFrom()
{
    sock->disconnectFromHost();
}

void DCPConnection::connected()
{
    emit networkConnected();
    if(initialMsg != NULL)
    {
        this->sendMessage(initialMsg);
        initialMsg = NULL;
    }
}

void DCPConnection::sendMessage(DCPMessage *message)
{
    char *buffer = new char[1370];
    quint16 len = message->toBytes(&buffer, 1370);

    if(len == 0)
    {
        fprintf(stderr, "oh shit\n");
        return;
    }

    sock->write(buffer, len);
    delete[] buffer;

    delete message;
}

void DCPConnection::dataReady()
{
    if(bufsize == 0)
    {
        if(sock->bytesAvailable() < (int)sizeof(quint16))
            return;     // We can't even get the next packet's size out of it.

        bufsize = sock->read(buffer, (qint64)sizeof(quint16));
        nextsize = htons( *(unsigned short *)buffer );  // XXX dear god the hax
        fprintf(stderr, "next size is %d\n", nextsize);
    }

    bufsize += sock->read(buffer + bufsize, nextsize - bufsize);

    if(bufsize >= nextsize)
    {
        DCPMessage *message = DCPMessage::fromBytes(buffer, nextsize);
        if(message == NULL)
        {
            fprintf(stderr, "the message failed, oh well\n");
            return;
        }
        emit messageReceived(message);
        delete message;
        memmove(buffer, buffer + nextsize, bufsize - nextsize);
        bufsize -= nextsize;
        if(bufsize > 1)
        {
            nextsize = htons( *(unsigned short *)buffer );  // XXX dear god the hax
        }
        else if(bufsize == 1)
        {
            abort();    // this is seriously better than any hax I could think up
        }
        // else it's 0 and will be handled above
    }
}

void DCPConnection::handleError(QAbstractSocket::SocketError socketError)
{
    emit errorReceived(socketError);
}

DCPConnection::~DCPConnection()
{
    delete[] buffer;
}
