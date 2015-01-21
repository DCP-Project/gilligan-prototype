#ifndef DCPMESSAGE_H
#define DCPMESSAGE_H

#include <QString>
#include <QMultiHash>

class DCPMessage
{
public:
    DCPMessage(QString source, QString dest, QString command,
               QMultiHash<QString, QString> params);

    static DCPMessage *fromBytes(char *bytes, quint16 byte_len);

    quint16 toBytes(char **bytes, quint16 max);

public:
    const QString source;
    const QString dest;
    const QString command;
    const QMultiHash<QString, QString> params;
};

#endif // DCPMESSAGE_H
