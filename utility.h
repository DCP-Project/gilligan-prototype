#ifndef UTILITY_H
#define UTILITY_H

#include "dcpmessage.h"
#include <QString>

QString messageRepr(DCPMessage *message, bool html = true);
QString prettyMessage(DCPMessage *message);

#endif // UTILITY_H
