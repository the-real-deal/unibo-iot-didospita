#include "message.hpp"

Message::Message(MessageType type, String content)
    : type(type), content(content) {}

Message::Message(MessageType type) : Message(type, String()) {}

MessageType Message::getType() { return this->type; }

const char *Message::getContent()
{
    auto contentStr = this->content.c_str();
    return contentStr;
}
