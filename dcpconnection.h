#ifndef DCPCONNECTION_H
#define DCPCONNECTION_H

#include <QObject>
#include <QSslSocket>
#include "dcpmessage.h"

class DCPConnection : public QObject
{
    Q_OBJECT

public:
    explicit DCPConnection(QObject *parent = 0);
    ~DCPConnection();

    void sendMessage(DCPMessage *message);
    void connectTo(QString server, QString handle, QString passphrase, QString client);
    void registerAndConnect(QString server, QString handle, QString passphrase,
                            QString client, QString gecos);
    void disconnectFrom();

    const QString handle();

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
    QSslSocket *sock;
    char *buffer;  quint16 bufsize;  quint16 nextsize;
    DCPMessage *initialMsg;

    QString myHandle;
};

#endif // DCPCONNECTION_H
