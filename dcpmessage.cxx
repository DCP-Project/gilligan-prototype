#include "dcpmessage.h"

#include <netinet/in.h> // ntohs
#include <QtDebug>

DCPMessage::DCPMessage(QString src, QString dst, QString cmd,
                       QMultiHash<QString, QString> p) :
    source(src), dest(dst), command(cmd), params(p)
{
}

DCPMessage *DCPMessage::fromBytes(char *bytes, quint16 byte_len)
{
    QString src, dst, cmd;
    quint16 len;
    char *next;
    DCPMessage *msg;

    len = ntohs(*(unsigned short *)bytes);
    if(len > 1400 || len != byte_len)
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

    next += cmd.length() + 1;

    QMultiHash<QString, QString> params;
    QString key, val;
    bool is_key = true;

    while((next - bytes) + 2 < len)
    {
        if(is_key)
        {
            key = QString(next);
            next += key.length() + 1;
            is_key = false;
        } else {
            val = QString(next);
            next += val.length() + 1;
            is_key = true;

            params.insert(key, val);
        }
    }

    msg = new DCPMessage(src, dst, cmd, params);

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
        Q_ASSERT(next > 0);
        IF_TOO_BIG_GO_AWAY;
        memcpy(buffer, iterate.key().toUtf8().constData(), next);
        buffer += next;
        *buffer++ = '\0';

        next = iterate.value().length();
        Q_ASSERT(next > 0);
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
