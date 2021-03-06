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
    else if(message->command.toLower() == "join")
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
    else if(message->command.toLower() == "metadata" ||
	    message->command.toLower() == "members")
    {
        emit groupMessageReceived(message->source, message);
    }
    else if(message->command.toLower() == "message")
    {
        if(message->dest[0] == '#')
        {
            emit groupMessageReceived(message->dest, message);
        }
        else
        {
            QString convo;
            if(message->source == connection->handle())
                convo = message->dest;
            else
                convo = message->source;

            emit privateMessageReceived(convo, message);
        }
    }
}
