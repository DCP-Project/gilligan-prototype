#ifndef DCPCONNECTION_H
#define DCPCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include "dcpmessage.h"

class DCPConnection : public QObject
{
    Q_OBJECT

public:
    explicit DCPConnection(QObject *parent = 0);
    ~DCPConnection();

    void sendMessage(DCPMessage *message);
    void connectTo(QString server, QString handle, QString passphrase, QString client);
    void disconnectFrom();

signals:
    void networkConnected();
    void messageReceived(DCPMessage *message);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();

private slots:
    void connected();
    void dataReady();
    void handleError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *sock;
    char *buffer;  quint16 bufsize;  quint16 nextsize;
    DCPMessage *initialMsg;
};

#endif // DCPCONNECTION_H
