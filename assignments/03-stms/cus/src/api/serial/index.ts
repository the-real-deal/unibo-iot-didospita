import { DelimiterParser, SerialPort } from "serialport"
import type { PortInfo } from "@serialport/bindings-interface"
import { EnvVarType, getEnv } from "../../utils/env"

const SERIAL_BAUD_RATE = getEnv("SERIAL_BAUD_RATE", EnvVarType.Number) ?? 9600
const SERIAL_DELIMITER = getEnv("SERIAL_DELIMITER", EnvVarType.String) ?? "\n"

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
  const serialPort = new SerialPort({
    path,
    baudRate,
    autoOpen: false,
  })
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

interface SerialConnectionOptions {
  baudRate?: number
  delimiter?: string
}

export async function connectToSerial(
  path: string,
  options: SerialConnectionOptions = {},
): Promise<SerialPort> {
  let { baudRate, delimiter } = {
    ...{
      baudRate: SERIAL_BAUD_RATE,
      delimiter: SERIAL_DELIMITER,
    },
    ...options,
  }
  const serialPort = openSerialPort(path, baudRate)
  attachParser(serialPort, delimiter)
  return serialPort
}
