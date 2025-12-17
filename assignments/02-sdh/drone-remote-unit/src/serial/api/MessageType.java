package serial.api;

public enum MessageType {
  REQUEST_LANDING("REQUEST LANDING"),
  REQUEST_TAKEOFF("REQUEST TAKEOFF"),
  STATE("STATE"),
  DISTANCE("DISTANCE");

  private final String displayName;

  MessageType(String displayName) {
    this.displayName = displayName;
  }

  @Override
  public String toString() {
    return this.displayName;
  }
}
