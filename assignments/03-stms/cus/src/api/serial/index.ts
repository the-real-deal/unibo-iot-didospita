import { DelimiterParser, SerialPort } from "serialport"
import { type PortInfo } from "@serialport/bindings-interface"

function portHasDevice(port: PortInfo) {
  return (
    port.serialNumber !== undefined ||
    port.vendorId !== undefined ||
    port.manufacturer !== undefined
  )
}

export async function findSerialDevice(): Promise<PortInfo | undefined> {
  const availablePorts = await SerialPort.list()
  const portInfo = availablePorts.find(portHasDevice)
  return portInfo
}

function openSerialPort(path: string, baudRate: number): SerialPort {
  const serialPort = new SerialPort({ path, baudRate, autoOpen: false })
  try {
    serialPort.open()
    console.debug("Successfully opened serial port")
  } catch {
    throw new Error(`Failed to open the serial port: ${serialPort.path}`)
  }
  return serialPort
}

function attachParser(serialPort: SerialPort, delimiter: string) {
  const parser = new DelimiterParser({ delimiter, includeDelimiter: false })
  parser.on("data", console.log)

  serialPort.pipe(parser)
}

export async function connectToSerial(
  path: string,
  delimiter: string,
  baudRate: number,
): Promise<SerialPort> {
  const serialPort = openSerialPort(path, baudRate)
  attachParser(serialPort, delimiter)
  return serialPort
}
