#include "conversationwidget.h"

#include "utility.h"

#include <QSplitter>
#include <QVBoxLayout>

#include <stdio.h> // fprintf, stderr

ConversationWidget::ConversationWidget(QString name, bool is_group, QWidget *parent) :
    name(name), group(is_group), QWidget(parent)
{
    nameLabel = new QLabel;
    nameLabel->setText(name);
    nameLabel->setStyleSheet("font-weight: bold;");
    nameLabel->setFixedHeight(25);

    convoEdit = new QTextEdit;
    convoEdit->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(nameLabel);

    if(group)
    {
        nameList = new QListWidget;

        QSplitter *split = new QSplitter;
        split->addWidget(convoEdit);
        split->addWidget(nameList);

        layout->addWidget(split);
    }
    else
    {
        layout->addWidget(convoEdit);
    }

    setLayout(layout);
}

void ConversationWidget::messageReceived(QString dontcare, DCPMessage *message)
{
    if(dontcare != name)
    {
        fprintf(stderr, "received a message that we probably should not have\n"
                "source = %s, us = %s\n",
                dontcare.toUtf8().constData(),
                name.toUtf8().constData());
        return;
    }

    this->messageReceived(message);
}

void ConversationWidget::messageReceived(DCPMessage *message)
{
    if(message->command != "message")
    {
        fprintf(stderr, "not handling non-message message yet, sorry\n");
        return;
    }

    convoEdit->insertHtml(prettyMessage(message));
}
