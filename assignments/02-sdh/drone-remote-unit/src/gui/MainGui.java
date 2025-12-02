package gui;

import gui.api.DroneController;
import gui.api.PanelView;
import gui.impl.DroneControllerImpl;
import gui.view.PanelViewImpl;

public class MainGui {
    public static void main(String[] args) {
        PanelView mainView = new PanelViewImpl();
        DroneController controller = new DroneControllerImpl(null, 0, mainView);
        controller.setView(mainView);
        mainView.setController(controller);
    }
}
