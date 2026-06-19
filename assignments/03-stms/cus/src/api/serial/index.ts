import { DoorManager } from "../../core/door.js"
import { SystemState, SystemStateManager } from "../../core/state.js"
import { WaterManager } from "../../core/water.js"
import { findSerialDevice } from "../../serial/index.js"
import {
  SerialMessagesServer,
  SerialMessageType,
} from "../../serial/message.js"
import { parseEnum } from "../../utils/enum.js"

const SAFE_LEVEL_DOOR_OPEN_PERC = 0
const DANGER_LEVEL_DOOR_OPEN_PERC = 0.5
const CRITICAL_LEVEL_DOOR_OPEN_PERC = 1.0

function setup(
  serialServer: SerialMessagesServer,
  waterManager: WaterManager,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
) {
  serialServer.onMessage(SerialMessageType.Log, (message) => {
    console.info("SERIAL LOG:", message.payload)
  })

  serialServer.onMessage(SerialMessageType.State, (message) => {
    const state = parseEnum(SystemState, message.payload.toString())
    if (state === null) {
      return
    }
    systemStateManager.registerSystemState(state)
  })

  serialServer.onMessage(SerialMessageType.Door, (message) => {
    const percentage = Number(message.payload.toString())
    if (isNaN(percentage) || percentage < 0 || percentage > 1) {
      return
    }
    doorManager.registerDoorPercentage(percentage)
  })

  systemStateManager.on("changed", (state) => {
    serialServer.sendMessage({ type: SerialMessageType.State, payload: state })
  })

  function sendDoorCommand(percentage: number) {
    serialServer.sendMessage({
      type: SerialMessageType.Door,
      payload: percentage.toPrecision(2),
    })
  }

  function sendEmergencyDoorCommand(percentage: number) {
    if (systemStateManager.getState() !== SystemState.Automatic) {
      return
    }
    sendDoorCommand(percentage)
  }

  doorManager.on("changed", (e) => {
    sendDoorCommand(e.percentage)
  })

  waterManager.on("safe", (_) => {
    sendEmergencyDoorCommand(SAFE_LEVEL_DOOR_OPEN_PERC)
  })

  waterManager.on("danger", (_) => {
    sendEmergencyDoorCommand(DANGER_LEVEL_DOOR_OPEN_PERC)
  })

  waterManager.on("critical", (_) => {
    sendEmergencyDoorCommand(CRITICAL_LEVEL_DOOR_OPEN_PERC)
  })
}

export interface SerialPortStartOptions {
  path?: string
}

export async function startSerialServer(
  baudRate: number,
  waterManager: WaterManager,
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
  setup(serialServer, waterManager, systemStateManager, doorManager)
  await serialServer.start()
  return serialServer
}
