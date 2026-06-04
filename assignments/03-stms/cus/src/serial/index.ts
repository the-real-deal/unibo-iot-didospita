import { SerialPort } from "serialport"
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

export function openSerialPort(path: string, baudRate: number): SerialPort {
  const serialPort = new SerialPort({ path, baudRate, autoOpen: false })
  try {
    serialPort.open()
    console.debug("Successfully opened serial port")
  } catch {
    throw new Error(`Failed to open the serial port: ${serialPort.path}`)
  }
  return serialPort
}
