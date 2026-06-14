import { DelimiterParser, SerialPort } from "serialport"

export enum SerialMessageType {
  Log = "LOG",
  SerialSync = "SERIAL_SYNC",
}

export interface SerialMessage {
  type: SerialMessageType
  payload: string
}

export type SerialMessageCallback = (
  message: SerialMessage,
) => SerialMessage | undefined

type SerialMessagesCallbacksMap = {
  [messageType in SerialMessageType]?: SerialMessageCallback
}

export type SerialMessagesServerOnCloseCallback = () => void

const SERIAL_MESSAGE_DELIMITER = ":"

export class SerialMessagesServer {
  callbacks: SerialMessagesCallbacksMap
  onCloseFn: SerialMessagesServerOnCloseCallback

  constructor() {
    this.callbacks = {}
    this.onCloseFn = () => {}
  }

  parseSerialMessage(
    buffer: Buffer,
    separator: string = SERIAL_MESSAGE_DELIMITER,
  ): SerialMessage {
    let message = buffer.toString("utf8").trim()
    if (message.endsWith("\r")) {
      message = message.substring(0, message.length - 1)
    }
    const delimiterSplit = message.split(separator, 2)
    const [type, payload] = delimiterSplit

    if (type === undefined || !this.isSerialMessageType(type)) {
      return { type: SerialMessageType.Log, payload: message }
    }
    return { type, payload: payload ?? "" }
  }

  isSerialMessageType(value: string): value is SerialMessageType {
    return (Object.values(SerialMessageType) as string[]).includes(value)
  }

  public onClose(fn: SerialMessagesServerOnCloseCallback) {
    this.onCloseFn = fn
  }

  public onMessage(type: SerialMessageType, fn: SerialMessageCallback) {
    this.callbacks[type] = fn
  }

  sendMessage(
    serialPort: SerialPort,
    message: SerialMessage,
    separator: string = SERIAL_MESSAGE_DELIMITER,
  ) {
    serialPort.write(`${message.type}${separator}${message.payload}\n`)
    serialPort.flush()
  }

  async start(path: string, baudRate: number): Promise<void> {
    const serialPort = new SerialPort({ path, baudRate, autoOpen: false })
    const parser = new DelimiterParser({
      delimiter: "\n",
      includeDelimiter: false,
    })
    parser.on("data", (buffer: Buffer) => {
      const message = this.parseSerialMessage(buffer)
      const callback = this.callbacks[message.type]
      if (callback !== undefined) {
        const response = callback(message)
        if (response !== undefined) {
          this.sendMessage(serialPort, response)
        }
      }
    })
    serialPort.on("close", (_) => this.onCloseFn())
    serialPort.pipe(parser)

    return new Promise((resolve, reject) => {
      console.debug("Opening serial port at:", serialPort.path)
      serialPort.open((err) => {
        if (err === null) {
          console.debug("Successfully opened serial port")
          resolve()
        } else {
          reject(err)
        }
      })
    })
  }
}
