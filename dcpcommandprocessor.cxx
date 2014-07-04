#include "dcpcommandprocessor.h"

DCPCommandProcessor::DCPCommandProcessor(DCPConnection *conn, QObject *parent) :
    QObject(parent)
{
    this->connection = conn;
}

void DCPCommandProcessor::sendMessage(DCPMessage *message)
{
    connection->sendMessage(message);
    if(message->command.toLower() == "message")
    {
        DCPMessage *msg = new DCPMessage(connection->handle(),
                                         message->dest,
                                         message->command,
                                         message->params);
        rawMessageReceived(msg);    // lazy.  pretend we received this.
        delete msg;
    }
}

void DCPCommandProcessor::defaultPingReceived(DCPMessage *ping)
{
    DCPMessage *msg = new DCPMessage("*",
                                     ping->source,
                                     "pong",
                                     ping->params);

    this->sendMessage(msg);
    delete msg;
}

void DCPCommandProcessor::rawMessageReceived(DCPMessage *message)
{
    if(message->command.toLower() == "ping")
    {
        emit pingReceived(message);
    }
    else if(message->command.toLower() == "group-enter")
    {
        if(message->source == connection->handle())
        {
            emit groupJoined(message->dest, message);
        }
        else
        {
            emit groupMessageReceived(message->dest, message);
        }
    }
    else if(message->command.toLower() == "group-info" ||
            message->command.toLower() == "group-names")
    {
        emit groupMessageReceived(message->dest, message);
    }
    else if(message->command.toLower() == "message")
    {
        if(message->dest[0] == '#')
        {
            emit groupMessageReceived(message->dest, message);
        }
        else
        {
            emit privateMessageReceived(message->source, message);
        }
    }
}
