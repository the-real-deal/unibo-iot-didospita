import express, { Express } from "express"
import http, { Server } from "http"
import { SystemState, SystemStateManager } from "../../core/state.js"
import { parseEnum } from "../../utils/enum.js"
import { DoorManager } from "../../core/door.js"
import { WaterMonitor } from "../../core/water.js"
import { EventStream } from "../../http/sse.js"

function successRes(value: any) {
  return { success: true, data: value }
}

function errorRes(error: string) {
  return { success: false, data: error }
}

function setup(
  app: Express,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
) {
  app.use(express.json())

  app.head("/", (req, res) => {
    return res.status(200).end()
  })

  app.post("/state", (req, res) => {
    const bodyState = req.body.state
    if (typeof bodyState !== "string") {
      return res.status(400).send(errorRes("Invalid field: state"))
    }
    const state = parseEnum(SystemState, bodyState)
    if (state === null || state === SystemState.Unconnected) {
      return res.status(400).send(errorRes("Invalid state"))
    }

    systemStateManager.registerSystemState(state)
    return res.status(200).send(successRes(state))
  })

  app.post("/door/opening", (req, res) => {
    const bodyValue = req.body.value
    if (typeof bodyValue !== "number") {
      return res.status(400).send(errorRes("Invalid field: value"))
    }
    const percentage = Number(bodyValue)
    if (isNaN(percentage) || percentage < 0 || percentage > 1) {
      return res.status(400).send(errorRes("Invalid value"))
    }

    doorManager.registerDoorPercentage(percentage)
    return res.status(200).send(successRes(percentage))
  })

  const waterLevelEventStream = new EventStream()

  app.get("/water/level/events", (req, res) => {
    waterLevelEventStream.registerClient(req, res)
  })

  waterMonitor.on("new", (e) => {
    waterLevelEventStream.sendEvent({ data: e })
  })

  const systemStateEventStream = new EventStream()

  app.get("/state/events", (req, res) => {
    systemStateEventStream.registerClient(req, res)
  })

  systemStateManager.on("changed", (e) => {
    systemStateEventStream.sendEvent({ data: e })
  })

  const doorEventStream = new EventStream()

  app.get("/door/opening/events", (req, res) => {
    doorEventStream.registerClient(req, res)
  })

  doorManager.on("changed", (e) => {
    doorEventStream.sendEvent({ data: e })
  })
}

export async function startHTTPServer(
  hostname: string,
  port: number,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
): Promise<Server> {
  return new Promise((resolve, reject) => {
    try {
      const app = express()
      setup(app, waterMonitor, systemStateManager, doorManager)
      const server = http.createServer(app)
      server.listen(port, hostname)
      resolve(server)
    } catch (err) {
      reject(err)
    }
  })
}
