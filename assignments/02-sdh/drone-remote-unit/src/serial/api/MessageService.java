package serial.api;

public interface MessageService {
  Message readMessage() throws InterruptedException;
  boolean messageAvailable();
  boolean send(Message message);
}
