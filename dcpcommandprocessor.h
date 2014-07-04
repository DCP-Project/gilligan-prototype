#ifndef DCPCOMMANDPROCESSOR_H
#define DCPCOMMANDPROCESSOR_H

#include <QObject>
#include <QString>
#include "dcpconnection.h"
#include "dcpmessage.h"

class DCPCommandProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DCPCommandProcessor(DCPConnection *conn, QObject *parent = 0);

    void sendMessage(DCPMessage *message);

signals:
    void groupJoined(QString group, DCPMessage *message);
    void groupLeft(QString group, DCPMessage *message);
    void groupMessageReceived(QString group, DCPMessage *message);

    void privateMessageReceived(QString source, DCPMessage *message);

    void pingReceived(DCPMessage *message);

public slots:
    virtual void rawMessageReceived(DCPMessage *message);

    void defaultPingReceived(DCPMessage *message);

protected:
    DCPConnection *connection;
};

#endif // DCPCOMMANDPROCESSOR_H
