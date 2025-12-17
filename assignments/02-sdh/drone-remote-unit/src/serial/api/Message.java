package serial.api;

public interface Message {
    MessageType getType();
    String getContent();
}
