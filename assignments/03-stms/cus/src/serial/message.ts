import { DelimiterParser, SerialPort } from "serialport"

export enum SerialMessageType {
  Log = "LOG",
  State = "STATE",
  Door = "DOOR",
}

export interface SerialMessage {
  type: SerialMessageType
  payload: string
}

export type SerialMessageCallback = (
  message: SerialMessage,
) => SerialMessage | void

type SerialMessagesCallbacksMap = {
  [messageType in SerialMessageType]?: SerialMessageCallback
}

export type SerialMessagesServerOnCloseCallback = () => void

const SERIAL_MESSAGE_DELIMITER = ":"

export class SerialMessagesServer {
  serialPort: SerialPort
  callbacks: SerialMessagesCallbacksMap
  onCloseFn: SerialMessagesServerOnCloseCallback

  constructor(path: string, baudRate: number) {
    this.serialPort = new SerialPort({ path, baudRate, autoOpen: false })
    this.callbacks = {}
    this.onCloseFn = () => {}
  }

  private parseSerialMessage(
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

  private isSerialMessageType(value: string): value is SerialMessageType {
    return (Object.values(SerialMessageType) as string[]).includes(value)
  }

  path(): string {
    return this.serialPort.path
  }

  onClose(fn: SerialMessagesServerOnCloseCallback) {
    this.onCloseFn = fn
  }

  onMessage(type: SerialMessageType, fn: SerialMessageCallback) {
    this.callbacks[type] = fn
  }

  sendMessage(
    message: SerialMessage,
    separator: string = SERIAL_MESSAGE_DELIMITER,
  ) {
    this.serialPort.write(`${message.type}${separator}${message.payload}\n`)
    this.serialPort.flush()
  }

  async start(): Promise<void> {
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
          this.sendMessage(response)
        }
      }
    })
    this.serialPort.on("close", (_) => this.onCloseFn())

    return new Promise((resolve, reject) => {
      console.debug("Opening serial port at:", this.serialPort.path)
      this.serialPort.open((err) => {
        if (err === null) {
          this.serialPort.flush(() => {
            this.serialPort.pipe(parser)
            console.debug("Successfully opened serial port")
            resolve()
          })
        } else {
          reject(err)
        }
      })
    })
  }
}
