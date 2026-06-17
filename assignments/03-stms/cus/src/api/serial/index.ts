import { DoorManager } from "../../core/door.js"
import { SystemState, SystemStateManager } from "../../core/state.js"
import { WaterMonitor } from "../../core/water.js"
import { findSerialDevice } from "../../serial/index.js"
import {
  SerialMessagesServer,
  SerialMessageType,
} from "../../serial/message.js"
import { parseEnum } from "../../utils/enum.js"

const SAFE_LEVEL_DOOR_OPEN_PERC = 0
const DANGEROUS_LEVEL_DOOR_OPEN_PERC = 0.5
const CRITICAL_LEVEL_DOOR_OPEN_PERC = 1.0

function setup(
  serialServer: SerialMessagesServer,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
) {
  serialServer.onMessage(SerialMessageType.Log, (message) => {
    console.info("Serial log:", message.payload)
  })

  serialServer.onMessage(SerialMessageType.State, (message) => {
    const state = parseEnum(SystemState, message.payload.toString())
    if (state === null) {
      return
    }
    systemStateManager.registerSystemState(state)
  })

  systemStateManager.on("changed", (state) => {
    serialServer.sendMessage({ type: SerialMessageType.State, payload: state })
  })

  doorManager.on("changed", (e) => {
    serialServer.sendMessage({
      type: SerialMessageType.Door,
      payload: e.percentage.toPrecision(2),
    })
  })

  waterMonitor.on("safe", (_) => {
    console.debug("SERIAL SAFE EVENT")
    serialServer.sendMessage({
      type: SerialMessageType.Door,
      payload: SAFE_LEVEL_DOOR_OPEN_PERC.toString(),
    })
  })

  waterMonitor.on("danger", (_) => {
    console.debug("SERIAL DANGER EVENT")
    serialServer.sendMessage({
      type: SerialMessageType.Door,
      payload: DANGEROUS_LEVEL_DOOR_OPEN_PERC.toString(),
    })
  })

  waterMonitor.on("critical", (_) => {
    console.debug("SERIAL CRITICAL EVENT")
    serialServer.sendMessage({
      type: SerialMessageType.Door,
      payload: CRITICAL_LEVEL_DOOR_OPEN_PERC.toString(),
    })
  })
}

export interface SerialPortStartOptions {
  path?: string
}

export async function startSerialServer(
  baudRate: number,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
  options: SerialPortStartOptions = {},
): Promise<SerialMessagesServer> {
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

  const serialServer = new SerialMessagesServer(path, baudRate)
  setup(serialServer, waterMonitor, systemStateManager, doorManager)
  await serialServer.start()
  return serialServer
}
