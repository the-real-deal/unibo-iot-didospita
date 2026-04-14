package gui.impl;

import gui.api.DroneController;
import gui.api.PanelView;
import serial.api.Message;
import serial.api.MessageService;
import serial.api.MessageType;
import serial.impl.MessageImpl;

public class DroneControllerImpl implements DroneController {

    private MessageService messageService;
    private PanelView managerView;

    public DroneControllerImpl(MessageService messageService, PanelView passedView) {
        this.messageService = messageService;
        this.managerView = passedView;
    }

    @Override
    public void sendMessage(Message msg) {
        String statusMsg;
        if (this.messageService.send(msg)) {
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
            msg = this.messageService.readMessage();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return msg;
    }

    public void updateViewStatus() {
        // Don't poll - messages are processed in serialEvent callback
        // Just check if there are messages in the queue
        while (this.messageService.messageAvailable()) {
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
