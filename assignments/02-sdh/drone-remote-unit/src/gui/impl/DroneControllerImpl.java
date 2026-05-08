package gui.impl;

import core.api.HangarState;
import core.api.Message;
import core.api.MessageService;
import core.api.MessageType;
import gui.api.DroneController;
import gui.api.PanelView;

public class DroneControllerImpl implements DroneController {

    private MessageService messageService;
    private PanelView managerView;

    public DroneControllerImpl(MessageService messageService, PanelView passedView) {
        this.messageService = messageService;
        this.managerView = passedView;
    }

    @Override
    public boolean sendMessage(Message msg) {
        return this.messageService.send(msg);
    }

    @Override
    public void setView(PanelView view) {
        this.managerView = view;
    }

    public void updateViewStatus() throws InterruptedException {
        // Don't poll - messages are processed in serialEvent callback
        // Just check if there are messages in the queue
        while (this.messageService.messageAvailable()) {
            Message msg = this.messageService.readMessage();
            if (msg.getType() == MessageType.STATE) {
                String content = msg.getContent();
                HangarState state = HangarState.fromDisplayName(content);
                managerView.updateHangarState(state);
            }
            if (msg.getType() == MessageType.DISTANCE) {
                managerView.updateDistance(msg.getContent());
            }
        }
    }
}
