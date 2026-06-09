import { DelimiterParser } from "serialport"

export enum SerialMessageType {
  Log = "LOG",
  SerialSync = "SERIAL_SYNC",
}

export interface SerialMessage {
  type: SerialMessageType
  payload: string
}

const SERIAL_MESSAGE_DELIMITER = ":"

function parseSerialMessage(
  buffer: Buffer,
  separator: string = SERIAL_MESSAGE_DELIMITER,
): SerialMessage {
  let message = buffer.toString("utf8").trim()
  if (message.endsWith("\r")) {
    message = message.substring(0, message.length - 1)
  }
  const delimiterSplit = message.split(separator, 2)
  const [type, payload] = delimiterSplit

  if (type === undefined || !isSerialMessageType(type)) {
    return { type: SerialMessageType.Log, payload: message }
  }
  return { type, payload: payload ?? "" }
}

function isSerialMessageType(value: string): value is SerialMessageType {
  return (Object.values(SerialMessageType) as string[]).includes(value)
}

export type SerialMessageCallback = (message: SerialMessage) => void

export type SerialMessagesCallbacksMap = {
  [messageType in SerialMessageType]?: SerialMessageCallback
}

export function serialMessageParser(
  callbacks: SerialMessagesCallbacksMap,
): NodeJS.WritableStream {
  const parser = new DelimiterParser({
    delimiter: "\n",
    includeDelimiter: false,
  })
  parser.on("data", (buffer: Buffer) => {
    const message = parseSerialMessage(buffer)
    const callback = callbacks[message.type]
    if (callback !== undefined) {
      callback(message)
    }
  })
  return parser
}
