package serial.impl;

import serial.api.Message;
import serial.api.MessageType;

public class MessageImpl implements Message{
    private MessageType type;
    private String content;

    public MessageImpl(MessageType type){
        this.type = type;
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