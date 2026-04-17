package core.api;

public interface Message {
    MessageType getType();
    String getContent();
}
