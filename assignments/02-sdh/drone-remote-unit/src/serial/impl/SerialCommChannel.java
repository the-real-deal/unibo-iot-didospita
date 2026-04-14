package serial.impl;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import serial.api.Message;
import serial.api.MessageService;
import serial.api.MessageType;

/**
 * Comm channel implementation based on serial port.
 * 
 * @author aricci
 *
 */
public class SerialCommChannel implements MessageService, SerialPortEventListener, AutoCloseable {

	private static final char MESSAGE_DELIMITER = '|';
	private static final char SERIAL_SYNC_BYTE = '\0';
	private SerialPort serialPort;
	private BlockingQueue<Message> queue;
	private Message currentMsg;
	private StringBuffer serialBuffer = new StringBuffer("");

	public SerialCommChannel(String port, int rate) throws Exception {
		queue = new ArrayBlockingQueue<Message>(100);

		serialPort = new SerialPort(port);
		serialPort.openPort();
		serialPort.setParams(rate,
				SerialPort.DATABITS_8,
				SerialPort.STOPBITS_1,
				SerialPort.PARITY_NONE);
		serialPort.addEventListener(this);
	}

	@Override
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

	public void serialEvent(SerialPortEvent event) {
		/* if there are bytes received in the input buffer */
		if (event.isRXCHAR()) {
			try {
				String msg = serialPort.readString(event.getEventValue());
				serialBuffer.append(msg);
				var serialSyncIndex = serialBuffer.toString().indexOf(SERIAL_SYNC_BYTE);
				if (serialSyncIndex != -1) {
					// reset
					serialBuffer.delete(0, serialSyncIndex + 1);
					this.queue.clear();
					this.currentMsg = null;
				}
			} catch (Exception ex) {
				ex.printStackTrace();
				System.err.println("Error in receiving string from COM-port: " + ex);
			}
		}
		while (true) {
			var lineEndIndex = serialBuffer.indexOf("\n");
			if (lineEndIndex == -1) {
				return;
			}
			var serialLineBuffer = new StringBuffer(serialBuffer.substring(0, lineEndIndex)); // exclude the newline
			serialBuffer.delete(0, lineEndIndex + 1);
			while (true) {
				var serialLine = serialLineBuffer.toString();
				var messageStartIndex = serialLine.indexOf(MESSAGE_DELIMITER);
				if (messageStartIndex == -1) {
					break;
				}
				int typeDelimiterIndex = serialLine.indexOf(MESSAGE_DELIMITER, messageStartIndex + 1);
				if (typeDelimiterIndex == -1) {
					break;
				};
				int terminatorIndex = serialLine.indexOf(MESSAGE_DELIMITER, typeDelimiterIndex + 1);
				if (terminatorIndex == - 1) {
					break;
				};
				var messageString = serialLine.substring(messageStartIndex, terminatorIndex + 1);
				System.out.println("Received message: " + messageString);
				var messageTypeDelimiterIndex = typeDelimiterIndex - messageStartIndex;
				var messageTerminatorIndex = terminatorIndex - messageStartIndex;
				String typeStr = messageString.substring(1, messageTypeDelimiterIndex);
				MessageType type = MessageType.fromDisplayName(typeStr);
				if (type == null) {
					System.err.println("Unknown message type: " + typeStr);
					break;
				}
				String content = messageString.substring(messageTypeDelimiterIndex + 1, messageTerminatorIndex);
				Message message = new MessageImpl(type, content);
				if (message != null) {
					this.queue.add(message);
				}
				serialLineBuffer.delete(messageStartIndex, terminatorIndex + 1);
			}
			if (serialLineBuffer.length() > 0) {
				System.out.println("LOG: " + serialLineBuffer.toString());
			}
			serialBuffer.delete(0, lineEndIndex + 2); // delete also the newline
		}

	}

	@Override
	public Message readMessage() throws InterruptedException {
		return this.currentMsg = messageAvailable() ? this.queue.take() : null;
	}

	@Override
	public boolean messageAvailable() {
		return this.currentMsg != null;
	}

	@Override
	public boolean send(Message message) {
		String messageString = MESSAGE_DELIMITER + message.getType().toString() + MESSAGE_DELIMITER
				+ message.getContent() + MESSAGE_DELIMITER;
		System.out.println("Sending message: " + messageString);
		try {
			synchronized (serialPort) {
				serialPort.writeString(messageString);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
			return false;
		}
		return true;
	}
}
