package gui.impl;

import gui.api.DroneController;
import gui.api.PanelView;
import serial.api.Message;
import serial.api.MessageService;
import serial.api.MessageType;
import serial.impl.MessageImpl;
import serial.impl.SerialCommChannel;

public class DroneControllerImpl implements DroneController {

    private MessageService serialManager;
    private PanelView managerView;
    private String serialPort;
    private int rate;

    public DroneControllerImpl(String sp, int rate, PanelView passedView) {
        this.serialPort = sp;
        this.rate = rate;
        this.managerView = passedView;
    }

    public void initialize() throws Exception {
        this.serialManager = new SerialCommChannel(this.serialPort, this.rate);
    }

    @Override
    public void sendMessage(Message msg) {
        String statusMsg;
        if (this.serialManager.send(msg)) {
            statusMsg = "Message sent!!!";
        } else {
            statusMsg = "Error in sending...";
        }
        this.managerView.displayMessage(statusMsg, "MESSAGE STATUS");
    }

    @Override
    public void setView(PanelView view) {
        this.managerView = view;
    }

    private Message receiveMsg() {
        Message msg = new MessageImpl(null);
        try {
            msg = this.serialManager.readMessage();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return msg;
    }

    public void updateViewStatus() {
        if (serialManager.messageAvailable()) {
            Message msg = receiveMsg();
            assert (msg.getType() != null && msg.getContent().isEmpty());
            if (msg.getType() == MessageType.STATE && msg.getContent() != "ALARM" && msg.getContent() != "NORMAL") {
                managerView.updateDroneStatus(msg.getContent());
            }
            if (msg.getType() == MessageType.STATE && msg.getContent() == "ALARM" || msg.getContent() == "NORMAL") {
                managerView.updateHangarStatus(msg.getContent());
            }
            if (msg.getType() == MessageType.DISTANCE) {
                managerView.updateDistance(msg.getContent());
            }
        }
    }
}
