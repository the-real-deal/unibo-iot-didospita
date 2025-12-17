import gui.api.DroneController;
import gui.api.PanelView;
import gui.impl.DroneControllerImpl;
import gui.view.PanelViewImpl;

public class Main {
    public static void main(String[] args) throws Exception {
        PanelView mainView = new PanelViewImpl();
        DroneController controller = new DroneControllerImpl("/dev/ttyACM0", 9600, mainView);
        controller.setView(mainView);
        controller.initialize();
        mainView.setController(controller);

        while (true) {
            controller.updateViewStatus();
        }
    }
}
