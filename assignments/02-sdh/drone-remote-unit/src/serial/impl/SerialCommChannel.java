package serial.impl;

import java.util.concurrent.*;
import jssc.*;
import serial.api.Message;
import serial.api.MessageService;
import serial.api.MessageType;

/**
 * Comm channel implementation based on serial port.
 * 
 * @author aricci
 *
 */
public class SerialCommChannel implements MessageService, SerialPortEventListener {

	private static final char  MESSAGE_DELIMITER = '|';
	private SerialPort serialPort;
	private BlockingQueue<Message> queue;
	private Message currentMsg;
	private StringBuffer decoderMsg = new StringBuffer("");
	private int delimiters = 0;
	
	public SerialCommChannel(String port, int rate) throws Exception {
		queue = new ArrayBlockingQueue<Message>(100);

		serialPort = new SerialPort(port);
		serialPort.openPort();

		serialPort.setParams(rate,
		                         SerialPort.DATABITS_8,
		                         SerialPort.STOPBITS_1,
		                         SerialPort.PARITY_NONE);

		serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | 
		                                  SerialPort.FLOWCONTROL_RTSCTS_OUT);

		// serialPort.addEventListener(this, SerialPort.MASK_RXCHAR);
		serialPort.addEventListener(this);
	}

	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	public void close() {
		try {
			if (serialPort != null) {
				serialPort.removeEventListener();
				serialPort.closePort();
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	private Message decoderSerialMessage(StringBuffer message) {
		assert(message.length() >= 3);            // start, type delimiter, terminator
		assert(message.toString().charAt(0) == MESSAGE_DELIMITER);  // start with delimiter
		int typeDelimiterIndex = message.toString().indexOf(MESSAGE_DELIMITER);
		assert(typeDelimiterIndex != -1);
		int terminatorIndex = message.toString().indexOf(MESSAGE_DELIMITER, typeDelimiterIndex + 1);
		assert(terminatorIndex == ((int)message.length()) - 1);
		MessageType type = MessageType.valueOf(message.substring(0, typeDelimiterIndex));
		String content = message.substring(typeDelimiterIndex, terminatorIndex);
		return new MessageImpl(type, content);
	}

	public void serialEvent(SerialPortEvent event) {
		/* if there are bytes received in the input buffer */
		if (event.isRXCHAR()) {
            try {
					this.decoderMsg = new StringBuffer();
					String msg = serialPort.readString(event.getEventValue());
            		
					for (char ch : msg.toCharArray()) {
						if (ch == MESSAGE_DELIMITER) {
							this.delimiters++;
						}
						if (this.delimiters > 0){
							decoderMsg.append(ch); 
						}
						if (this.delimiters == 3) {
							this.queue.add(decoderSerialMessage(decoderMsg));
							this.decoderMsg = new StringBuffer();
							this.delimiters = 0;
						}
					}
					this.currentMsg = this.readMessage();			
            } catch (Exception ex) {
            		ex.printStackTrace();
                System.out.println("Error in receiving string from COM-port: " + ex);
            }
        }
	}

	@Override
	public Message readMessage() throws InterruptedException {
		return this.currentMsg =  messageAvailable() ? this.queue.take() : null;
	}

	@Override
	public boolean messageAvailable() {
		return this.currentMsg != null;
	}

	@Override
	public boolean send(Message message) {
		
		char[] array = ((MESSAGE_DELIMITER + message.getType().toString() + MESSAGE_DELIMITER + message.getContent() + MESSAGE_DELIMITER)).toCharArray();
		byte[] bytes = new byte[array.length];
		for (int i = 0; i < array.length; i++){
			bytes[i] = (byte) array[i];
		}
		try {
			synchronized (serialPort) {
				serialPort.writeBytes(bytes);
			}
		} catch(Exception ex){
			ex.printStackTrace();
			return false;
		}
		return true;
	}
}
