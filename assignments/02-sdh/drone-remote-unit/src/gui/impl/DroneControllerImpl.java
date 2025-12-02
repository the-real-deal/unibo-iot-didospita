package gui.impl;

import gui.api.DroneController;
import gui.api.PanelView;
import serial.api.CommChannel;
import serial.impl.SerialCommChannel;

public class DroneControllerImpl implements DroneController{

    private CommChannel serialManager;
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
    public void sendMessage(String msg) {
        String statusMsg;
        if(this.serialManager.sendMsg(msg)){
            statusMsg = "Message sent!!!";
        }
        else {
            statusMsg = "Error in sending..."; 
        }
        this.managerView.displayMessage(statusMsg, "MESSAGE STATUS");
    }

    @Override
    public void setView(PanelView view) {
        this.managerView = view;
    }

    @Override
    public String receiveMsg() {
        String msg;
        try {
            msg = this.serialManager.receiveMsg();
        } catch (InterruptedException e) {
            msg = "ERROR";
            e.printStackTrace();
        }
        if(msg.isBlank()){
            msg = "ERROR";
        }
        return msg;
    }
    
}
