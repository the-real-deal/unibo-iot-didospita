package core.api;

public enum HangarState {
  INSIDE("DRONE INSIDE"),
  OUTSIDE("DRONE OUT"),
  PREALARM("PREALARM"),
  ALARM("ALARM"),
  TAKEOFF("TAKE OFF"),
  LANDING("LANDING");

  private final String displayName;

  HangarState(String displayName) {
    this.displayName = displayName;
  }

  public String getDisplayName() {
    return this.displayName;
  }

  public static HangarState fromDisplayName(String displayName) {
    for (HangarState mt : values()) {
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
