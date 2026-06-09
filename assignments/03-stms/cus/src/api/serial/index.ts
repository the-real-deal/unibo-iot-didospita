import { DelimiterParser, type SerialPort } from "serialport"
import { findSerialDevice, openSerialPort } from "../../serial/index.js"
import { serialMessageParser } from "../../serial/message.js"

const parser = serialMessageParser({
  "SERIAL_SYNC": (_) => console.warn("Serial sync"),
  "LOG": (message) => console.info("Serial log:", message.payload),
})

export interface SerialPortStartOptions {
  path?: string
}

export async function startSerialPort(
  baudRate: number,
  options: SerialPortStartOptions = {},
): Promise<SerialPort> {
  let path: string
  if (options.path === undefined) {
    const portInfo = await findSerialDevice()
    if (portInfo === undefined) {
      throw new Error("No serial device found")
    }
    console.debug("Serial device found at:", portInfo.path)
    path = portInfo.path
  } else {
    path = options.path
  }

  const serialPort = openSerialPort(path, baudRate)
  serialPort.pipe(parser)
  console.debug("Attached serial port messages parser")
  return serialPort
}
