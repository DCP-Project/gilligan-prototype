#include "utility.h"

QString messageRepr(DCPMessage *message, bool html)
{
    QString repr = "";

    if(html) repr += "<span class=\"message\">";

    /* source */
    if(html) repr += "<span class=\"source\"> &lt;";
    else repr += "<";

    repr += message->source;

    if(html) repr += "&gt;</span> ";
    else repr += "> ";


    /* dest */
    if(html) repr += "<span class=\"destination\">";

    if(html) repr += "&rarr; ";
    else repr += "-> ";

    repr += message->dest + " ";

    if(html) repr += "</span>";


    /* command */
    if(html) repr += "<span class=\"command\">";
    repr += message->command + " ";
    if(html) repr += "</span>";


    /* params */
    if(html) repr += "<span class=\"params\">";
    repr += "{";

    QListIterator<QString> key_iterator(message->params.keys());
    while(key_iterator.hasNext())
    {
        QString key = key_iterator.next();

        if(html) repr += "<span class=\"param\"><span class=\"key\">";
        repr += key;
        if(html) repr += "</span>";
        repr += ": [";

        QListIterator<QString> value_iterator(message->params.values(key));
        while(value_iterator.hasNext())
        {
            QString value = value_iterator.next();

            if(html) repr += "<span class=\"value\">";
            repr += value;
            if(html) repr += "</span>";

            if(value_iterator.hasNext()) repr += ", ";
        }

        repr += "]";
        if(html) repr += "</span>";

        if(key_iterator.hasNext()) repr += ", ";
    }

    repr += "}";

    if(html) repr += "</span><br>";
    repr += "\n";

    return repr;
}