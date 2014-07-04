#ifndef CONVERSATIONWIDGET_H
#define CONVERSATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>

#include "dcpmessage.h"

class ConversationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConversationWidget(QString name, bool is_group,
                                QWidget *parent = 0);

signals:

public slots:
    void messageReceived(QString dontcare, DCPMessage *message);
    void messageReceived(DCPMessage *message);

private:
    QString name;

    QLabel *nameLabel;
    QTextEdit *convoEdit;
    QListWidget *nameList;

    bool group;

    void append(QString html);
};

#endif // CONVERSATIONWIDGET_H
