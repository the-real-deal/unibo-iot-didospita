export enum SerialMessageType {
  Log = "LOG",
  SerialSync = "SERIAL_SYNC",
}

export interface SerialMessage {
  type: SerialMessageType
  payload?: string
}

const SERIAL_MESSAGE_DELIMITER = ":"

export function parseSerialMessage(
  buffer: Buffer,
  separator: string = SERIAL_MESSAGE_DELIMITER,
): SerialMessage {
  const message = buffer.toString("utf8")
  const delimiterSplit = message.split(separator)
  const [type, payload] = delimiterSplit

  if (type === undefined || payload === undefined || !isSerialMessageType(type)) {
    return { type: SerialMessageType.Log, payload: message }
  }
  return { type, payload }
}

function isSerialMessageType(value: string): value is SerialMessageType {
  return (Object.values(SerialMessageType) as string[]).includes(value)
}
