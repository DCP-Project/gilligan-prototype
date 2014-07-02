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

private:
    quint16 length;
public:
    const QString source;
    const QString dest;
    const QString command;
    const QMultiHash<QString, QString> params;

private:
    char *bytes;
    bool bytes_is_dirty;
};

#endif // DCPMESSAGE_H
