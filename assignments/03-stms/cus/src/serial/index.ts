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
  console.debug("Available ports:", availablePorts)
  const portInfo = availablePorts.find(portHasDevice)
  return portInfo
}
