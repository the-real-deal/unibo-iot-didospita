import { DelimiterParser, type SerialPort } from "serialport"
import { findSerialDevice, openSerialPort } from "../../serial/index.js"

const parser = new DelimiterParser({ delimiter: "\n", includeDelimiter: false })
parser.on("data", (chunk: Buffer) => {
  console.log("Serial hello:", chunk.toString())
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
