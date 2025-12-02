package gui.api;

public interface PanelView {
    /**
     * A method that is invoked to display a specific message passed from the controller in response to certain actions.
     * @param messagge to be displayed.
     * @param title of the messagge window.
     */
    void displayMessage(String messagge, String title); 
    void setController(DroneController controller);
}
