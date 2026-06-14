import { findSerialDevice } from "../../serial/index.js"
import { SerialMessagesServer, SerialMessageType } from "../../serial/message.js"

const serialServer = new SerialMessagesServer()
let serialSynced = false

serialServer.onClose(() => {
  serialSynced = false
})

serialServer.onMessage(SerialMessageType.SerialSync, (_) => {
  console.warn("Serial sync")
  serialSynced = true
})

serialServer.onMessage(SerialMessageType.Log, (message) => {
  if (serialSynced) {
    console.info("Serial log:", message.payload)
  }
})

export interface SerialPortStartOptions {
  path?: string
}

export async function startSerialServer(
  baudRate: number,
  options: SerialPortStartOptions = {},
): Promise<[SerialMessagesServer, string]> {
  let path: string
  if (options.path === undefined || options.path.length === 0) {
    const portInfo = await findSerialDevice()
    if (portInfo === undefined) {
      throw new Error("No serial device found")
    }
    console.debug("Serial device found at:", portInfo.path)
    path = portInfo.path
  } else {
    path = options.path
  }

  await serialServer.start(path, baudRate)
  return [serialServer, path]
}
