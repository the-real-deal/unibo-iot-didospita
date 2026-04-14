import gui.api.DroneController;
import gui.api.PanelView;
import gui.impl.DroneControllerImpl;
import gui.view.PanelViewImpl;
import serial.impl.SerialCommChannel;

public class Main {
    public static void main(String[] args) throws Exception {
        try (var serialMessageService = new SerialCommChannel("/dev/ttyACM0", 9600)) {
            PanelView mainView = new PanelViewImpl();
            DroneController controller = new DroneControllerImpl(serialMessageService, mainView);
            controller.setView(mainView);
            mainView.setController(controller);

            while (true) {
                controller.updateViewStatus();
            }
        }
    }
}
