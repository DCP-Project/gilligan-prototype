#include "dcpconnection.h"

#include <stdio.h> // fprintf, stderr
#include <netinet/in.h> // htons

#define BUF_MAX_SIZE 65536

DCPConnection::DCPConnection(QObject *parent) :
    QObject(parent)
{
    sock = new QSslSocket(this);
    /****
     * !!!!! XXX TODO BAD !!!!! *
     * this will just get the certificate from the server and not care whether
     * it is valid or not.  THIS WILL ALLOW EASY DNS-BASED SPOOFING ATTACKS!
     * DO NOT EVER EVER EVER EVER USE CODE WITH ::QueryPeer IN PRODUCTION!
     * I MEAN IT
     * REWRITE THIS BEFORE MAKING A PRODUCTION CLIENT
     * no really
     * I mean it.
     ****/
    sock->setPeerVerifyMode(QSslSocket::QueryPeer);
#if QT_VERSION >= 0x050000
    sock->setProtocol(QSsl::TlsV1_2);
#endif

    bufsize = nextsize = 0;
    buffer = new char[BUF_MAX_SIZE];
    initialMsg = NULL;

    connect(sock, SIGNAL(readyRead()), this, SLOT(dataReady()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
    connect(sock, SIGNAL(encrypted()), this, SLOT(connected()));
}

void DCPConnection::connectTo(QString server, QString handle, QString passphrase, QString client)
{
    QMultiHash<QString, QString> signon;
    signon.insert("handle", handle);
    signon.insert("password", passphrase);
    signon.insert("client-name", client);
    signon.insert("client-ver", "Gilligan");
    signon.insert("options", "*");
    if(initialMsg != NULL)
    {
        delete initialMsg;
    }
    initialMsg = new DCPMessage("*", "*", "SIGNON", signon);

    this->myHandle = handle;

    sock->connectToHostEncrypted(server, 7266);
}

void DCPConnection::registerAndConnect(QString server, QString handle,
                                       QString passphrase, QString client,
                                       QString gecos)
{
    QMultiHash<QString, QString> params;
    params.insert("handle", handle);
    params.insert("password", passphrase);
    params.insert("client-name", client);
    params.insert("client-ver", "Gilligan");
    params.insert("gecos", gecos);
    params.insert("options", "*");
    if(initialMsg != NULL)
    {
        delete initialMsg;
    }
    initialMsg = new DCPMessage("*", "*", "REGISTER", params);

    this->myHandle = handle;

    sock->connectToHostEncrypted(server, 7266);
}

const QString DCPConnection::handle()
{
    return this->myHandle;
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
        delete initialMsg;
        initialMsg = NULL;
    }
}

void DCPConnection::sendMessage(DCPMessage *message)
{
    char *buffer = new char[1400];
    quint16 len = message->toBytes(&buffer, 1400);

    if(len == 0)
    {
        fprintf(stderr, "Failed to convert the message to a raw value\n");
        return;
    }

    sock->write(buffer, len);
    delete[] buffer;
}

void DCPConnection::dataReady()
{
    while(sock->bytesAvailable() > 0)
    {
        if(bufsize == 0)
        {
            if(sock->bytesAvailable() < (int)sizeof(quint16))
                return;     // We can't even get the next packet's size out of it.

            bufsize = sock->read(buffer, (qint64)sizeof(quint16));
            nextsize = htons( *(unsigned short *)buffer );  // XXX dear god the hax
        }

        bufsize += sock->read(buffer + bufsize, nextsize - bufsize);

        if(bufsize >= nextsize)
        {
            DCPMessage *message = DCPMessage::fromBytes(buffer, nextsize);
            if(message == NULL)
            {
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
                if(sock->bytesAvailable() > 0)
                {
                    bufsize += sock->read(buffer + bufsize, 1);
                } else {
                    abort();    // this is seriously better than any hax I could think up
                }
            }
            // else it's 0 and will be handled above
        }
        else
        {
            break;
        }
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
