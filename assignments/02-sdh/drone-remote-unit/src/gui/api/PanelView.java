package gui.api;

import core.api.HangarState;

public interface PanelView {
    void setController(DroneController controller);
    void updateHangarState(HangarState state);
    void updateDistance(String distance);
}
