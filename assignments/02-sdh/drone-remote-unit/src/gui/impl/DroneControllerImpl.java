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
        // Don't poll - messages are processed in serialEvent callback
        // Just check if there are messages in the queue
        while (serialManager.messageAvailable()) {
            Message msg = receiveMsg();
            if (msg == null || msg.getType() == null) {
                break;
            }
            if (msg.getType() == MessageType.STATE) {
                String content = msg.getContent();
                if (content.equals("ALARM") || content.equals("PREALARM")) {
                    managerView.updateHangarStatus(content);
                } else {
                    managerView.updateDroneStatus(content);
                    if (content.equals("DRONE INSIDE") || content.equals("DRONE OUTSIDE")) {
                        managerView.updateHangarStatus("NORMAL");
                    }
                }
            }
            if (msg.getType() == MessageType.DISTANCE) {
                managerView.updateDistance(msg.getContent());
            }
        }
    }
}
