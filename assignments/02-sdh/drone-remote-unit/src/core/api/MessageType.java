package core.api;

public enum MessageType {
  SERIAL_SYNC("SERIAL SYNC"),
  REQUEST_LANDING("REQUEST LANDING"),
  REQUEST_TAKEOFF("REQUEST TAKEOFF"),
  STATE("STATE"),
  DISTANCE("DISTANCE");

  private final String displayName;

  MessageType(String displayName) {
    this.displayName = displayName;
  }

  public String getDisplayName() {
    return this.displayName;
  }

  public static MessageType fromDisplayName(String displayName) {
    for (MessageType mt : values()) {
      if (mt.displayName.equals(displayName)) {
        return mt;
      }
    }
    return null;
  }

  @Override
  public String toString() {
    return this.displayName;
  }
}
