package gui.api;

import core.api.Message;

public interface DroneController {
    /**
     * A method that is invoked to display a specific message passed from the
     * controller in response to certain actions.
     * 
     * @param mesg the message to send.
     */
    void sendMessage(Message msg);

    void setView(PanelView view);

    void updateViewStatus();

}
