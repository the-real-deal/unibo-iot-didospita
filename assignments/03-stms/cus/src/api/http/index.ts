import express, { Express } from "express"
import http, { Server } from "http"
import { SystemState, SystemStateManager } from "../../core/state.js"
import { parseEnum } from "../../utils/enum.js"
import { DoorManager } from "../../core/door.js"
import { WaterMonitor } from "../../core/water.js"
import { EventsStream } from "../../http/sse.js"
import { errorRes, successRes } from "../../http/utils.js"
import cors from "cors"

function setup(
  app: Express,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
) {
  app.use(express.json())
  app.use(cors())

  app.head("/", (_, res) => {
    return res.status(200).end()
  })

  app.post("/state", (req, res) => {
    console.info("HTTP STATE BODY:", req.body)
    const bodyState = req.body.state
    if (typeof bodyState !== "string") {
      return res.status(400).send(errorRes("Invalid field: state"))
    }
    const state = parseEnum(SystemState, bodyState)
    if (state === null || state === SystemState.Unconnected) {
      return res.status(400).send(errorRes("Invalid value"))
    }

    if (systemStateManager.getState() === SystemState.Unconnected) {
      return res.status(400).send(errorRes("Invalid state"))
    }
    systemStateManager.registerSystemState(state)
    return res.status(200).send(successRes({ state }))
  })

  app.post("/door/opening", (req, res) => {
    console.info("HTTP DOOR BODY:", req.body)
    const bodyPercentage = req.body.percentage
    if (typeof bodyPercentage !== "number") {
      return res.status(400).send(errorRes("Invalid field: percentage"))
    }
    const percentage = Number(bodyPercentage)
    if (isNaN(percentage) || percentage < 0 || percentage > 1) {
      return res.status(400).send(errorRes("Invalid value"))
    }

    if (systemStateManager.getState() !== SystemState.Manual) {
      return res.status(400).send(errorRes("Invalid state"))
    }
    doorManager.registerDoorPercentage(percentage)
    return res.status(200).send(successRes({ percentage }))
  })

  app.get("/snapshot", (_, res) => {
    const state = systemStateManager.getState()
    const percentage = doorManager.getOpeningPercentage()
    const level = waterMonitor.getLevel()
    return res.status(200).send(
      successRes({
        state,
        waterLevel: level,
        doorOpening: percentage,
      }),
    )
  })

  const eventsStream = new EventsStream()
  app.get("/events", (req, res) => eventsStream.registerClient(req, res))

  waterMonitor.on("new", (e) => {
    eventsStream.sendEvent({ event: "water/level", data: e.level })
  })

  systemStateManager.on("changed", (state) => {
    eventsStream.sendEvent({ event: "state", data: state })
  })

  doorManager.on("changed", (e) => {
    eventsStream.sendEvent({ event: "door/opening", data: e.percentage })
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
