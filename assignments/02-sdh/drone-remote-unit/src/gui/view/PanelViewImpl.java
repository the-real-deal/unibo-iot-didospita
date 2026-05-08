package gui.view;

import javax.swing.*;

import core.api.HangarState;
import core.api.MessageType;
import core.impl.MessageImpl;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import gui.api.DroneController;
import gui.api.PanelView;

public class PanelViewImpl extends JFrame implements PanelView {
    private DroneController controller;
    private final JPanel myJPanel = new JPanel();

    private JLabel lblStatusDroneVal;
    private JLabel lblStatusHangarVal;
    private JLabel lblDistanceVal;

    private JButton btnLanding;
    private JButton btnTakeoff;

    public PanelViewImpl() {
        setTitle("Drone Control Station");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());

        initUI();

        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void initUI() {
        JPanel infoPanel = new JPanel();
        infoPanel.setLayout(new GridLayout(3, 2, 10, 10));
        infoPanel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        JLabel lblStatusDrone = new JLabel("Status Drone:");
        lblStatusDrone.setFont(new Font("Arial", Font.BOLD, 14));
        lblStatusDroneVal = new JLabel("IDLE");

        JLabel lblStatusHangar = new JLabel("Status Hangar:");
        lblStatusHangar.setFont(new Font("Arial", Font.BOLD, 14));
        lblStatusHangarVal = new JLabel("NORMAL");

        JLabel lblDistance = new JLabel("Distanza da terra:");
        lblDistance.setFont(new Font("Arial", Font.BOLD, 14));
        lblDistanceVal = new JLabel("-");

        infoPanel.add(lblStatusDrone);
        infoPanel.add(lblStatusDroneVal);
        infoPanel.add(lblStatusHangar);
        infoPanel.add(lblStatusHangarVal);
        infoPanel.add(lblDistance);
        infoPanel.add(lblDistanceVal);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 20, 10));

        btnTakeoff = new JButton("Takeoff Phase");
        btnLanding = new JButton("Landing Phase");

        btnTakeoff.setBackground(new Color(144, 238, 144));
        btnLanding.setBackground(new Color(255, 182, 193));

        btnTakeoff.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                var msg = new MessageImpl(MessageType.REQUEST_TAKEOFF);
                buttonsSetEnabled(false);
                if (!controller.sendMessage(msg)) {
                    displayWarning("Error in sending message " + msg);
                }
                buttonsSetEnabled(true);
            }
        });

        btnLanding.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                var msg = new MessageImpl(MessageType.REQUEST_LANDING);
                buttonsSetEnabled(false);
                if (!controller.sendMessage(msg)) {
                    displayWarning("Error in sending message " + msg);
                }
                buttonsSetEnabled(true);
            }
        });

        buttonPanel.add(btnTakeoff);
        buttonPanel.add(btnLanding);

        add(infoPanel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);
    }

    public void displayWarning(String warning) {
        JOptionPane.showMessageDialog(myJPanel, warning, null, JOptionPane.WARNING_MESSAGE);
    }

    @Override
    public void setController(DroneController controller) {
        this.controller = controller;
    }

    private void buttonsSetEnabled(boolean enable) {
        btnTakeoff.setEnabled(enable);
        btnLanding.setEnabled(enable);
    }

    @Override
    public void updateHangarState(HangarState state) {
        switch (state) {
            case ALARM:
                lblStatusHangarVal.setText("ALARM");
                buttonsSetEnabled(false);
                break;
            case INSIDE:
                lblStatusDroneVal.setText("REST");
                lblStatusHangarVal.setText("NORMAL");
                lblDistanceVal.setText("-");
                buttonsSetEnabled(true);
                break;
            case LANDING:
                lblStatusDroneVal.setText("LANDING");
                lblStatusHangarVal.setText("NORMAL");
                buttonsSetEnabled(false);
                break;
                case OUTSIDE:
                lblStatusDroneVal.setText("OPERATING");
                lblStatusHangarVal.setText("NORMAL");
                lblDistanceVal.setText("-");
                buttonsSetEnabled(true);
                break;
            case PREALARM:
                buttonsSetEnabled(false);
                break;
            case TAKEOFF:
                lblStatusDroneVal.setText("TAKING OFF");
                lblStatusHangarVal.setText("NORMAL");
                buttonsSetEnabled(false);
                break;
        }
    }

    @Override
    public void updateDistance(String distance) {
        lblDistanceVal.setText(distance);
    }

}
