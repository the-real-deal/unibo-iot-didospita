package gui.view;

import javax.swing.*;
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
    }

    private void initUI() {
        JPanel infoPanel = new JPanel();
        infoPanel.setLayout(new GridLayout(3, 2, 10, 10)); 
        infoPanel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20)); 

        JLabel lblStatusDrone = new JLabel("Status Drone:");
        lblStatusDrone.setFont(new Font("Arial", Font.BOLD, 14));
        lblStatusDroneVal = new JLabel("IDLE"); 
        lblStatusDroneVal.setForeground(Color.BLUE);

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
                controller.sendMessage("TAKEOFF");
                
                setDroneStatus("TAKEOFF");
                setHangarStatus(controller.receiveMsg());
            }
        });

        btnLanding.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                controller.sendMessage("LANDING");
                
                setDroneStatus("LANDING");
                setHangarStatus(controller.receiveMsg());
                setDistance(controller.receiveMsg());
            }
        });

        buttonPanel.add(btnTakeoff);
        buttonPanel.add(btnLanding);

        add(infoPanel, BorderLayout.CENTER);
        add(buttonPanel, BorderLayout.SOUTH);
    }

    private void setDroneStatus(String status) {
        lblStatusDroneVal.setText(status);
    }

    private void setHangarStatus(String status) {
        lblStatusHangarVal.setText(status);
    }

    private void setDistance(String distance) {
        lblDistanceVal.setText(distance);
    }


    @Override
    public void displayMessage(String messagge, String title) {
        JOptionPane.showMessageDialog(myJPanel, messagge, title, JOptionPane.INFORMATION_MESSAGE);
    }

    @Override
    public void setController(DroneController controller) {
        this.controller = controller;
    }



}
