package core.impl;

import core.api.Message;
import core.api.MessageType;

public class MessageImpl implements Message {
    private MessageType type;
    private String content;

    public MessageImpl(MessageType type) {
        this(type, "");
    }

    public MessageImpl(MessageType type, String content) {
        this.type = type;
        this.content = content;
    }

    @Override
    public MessageType getType() {
        return this.type;
    }

    @Override
    public String getContent() {
        return this.content;
    }

}