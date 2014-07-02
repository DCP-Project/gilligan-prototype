#include "dcpmessage.h"

#include <netinet/in.h> // ntohs

DCPMessage::DCPMessage(QString src, QString dst, QString cmd,
                       QMultiHash<QString, QString> p) :
    source(src), dest(dst), command(cmd), params(p)
{
    bytes = NULL;
    bytes_is_dirty = true;
}

DCPMessage *DCPMessage::fromBytes(char *bytes, quint16 byte_len)
{
    QString src, dst, cmd;
    quint16 len;
    char *next;
    DCPMessage *msg;

    len = ntohs(*(unsigned short *)bytes);
    if(len > 1370 || len != byte_len)
    {
        return NULL;    // Bad.
    }

    next = bytes + 3;
    src = QString(next);

    next += src.length() + 1;
    if((next - bytes) > len) return NULL;
    dst = QString(next);

    next += dst.length() + 1;
    if((next - bytes) > len) return NULL;
    cmd = QString(next);

    msg = new DCPMessage(src, dst, cmd, QMultiHash<QString, QString>());

    return msg;
}

#define IF_TOO_BIG_GO_AWAY if(((buffer - *bytes) + next) > max) return 0

quint16 DCPMessage::toBytes(char **bytes, quint16 max)
{
    char *buffer = *bytes;
    int next = 0;

    buffer += 2;
    *buffer++ = '\0';

    next = this->source.length();
    IF_TOO_BIG_GO_AWAY;
    memcpy(buffer, this->source.toUtf8().constData(), next);
    buffer += next;
    *buffer++ = '\0';

    next = this->dest.length();
    IF_TOO_BIG_GO_AWAY;
    memcpy(buffer, this->dest.toUtf8().constData(), next);
    buffer += next;
    *buffer++ = '\0';

    next = this->command.length();
    IF_TOO_BIG_GO_AWAY;
    memcpy(buffer, this->command.toUtf8().constData(), next);
    buffer += next;
    *buffer++ = '\0';

    QHashIterator<QString, QString> iterate(this->params);
    while(iterate.hasNext())
    {
        iterate.next();

        next = iterate.key().length();
        IF_TOO_BIG_GO_AWAY;
        memcpy(buffer, iterate.key().toUtf8().constData(), next);
        buffer += next;
        *buffer++ = '\0';

        next = iterate.value().length();
        IF_TOO_BIG_GO_AWAY;
        memcpy(buffer, iterate.value().toUtf8().constData(), next);
        buffer += next;
        *buffer++ = '\0';
    }

    // pad
    *buffer++ = '\0';

    quint16 len = buffer - *bytes;
    *(unsigned short *)*bytes = ntohs(len);

    return len;
}
