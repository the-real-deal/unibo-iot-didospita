#include "message.hpp"

Message::Message(MessageType type, String content)
    : type(type), content(content) {}

MessageType Message::getType() { return this->type; }

String Message::getContent() { return this->content; }
